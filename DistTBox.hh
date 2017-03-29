#pragma once
#include "DistSTOServer.hh"
#include "Interface.hh"
#include "TWrapped.hh"

template <typename T, typename W = TWrapped<T> >
class DistTBox : public TObject {
public:
    typedef typename W::read_type read_type;
    typedef typename W::version_type version_type;

    DistTBox() {
    }

    template <typename... Args>
    explicit DistTBox(Args&&... args)
        : v_(std::forward<Args>(args)...) {
    }

    void do_rpc(DoRpcResponse& response, const DoRpcArgs& opargs) {
        switch(opargs.op) {
        case READ_OP:
            {
                // arguments: none
                // allocate space in return buffer
                response.value.resize(sizeof(T));
                T &obj = *(T *) (response.value.data());

                // TODO: we need to change TWrapped / TransProxy to not use a transaction,
                // so that we can use the usual read() method instead of this loop
                while (1) {
                    version_type v1 = vers_;
                    version_type v2;
                    fence();
                    obj = v_.access();
                    fence();
                    v2 = vers_;
                    if (v2.is_locked()) {
                        // abort if the object is locked instead of blocking
			response.success = false;
                        return;
                    }
                    if (v1 == v2) {
                        response.version = (int64_t) v1.value();
			response.success = true;
                        return;
                    }
                    relax_fence();
                }
                // unreachable
                break;
            }
        }
	response.success = false;
        return;
    }

    read_type read() const {
        auto item = Sto::item(this, 0);
        if (item.has_write()) {
            return item.template write_value<T>();
        } else if (Sto::server->is_local_obj(this)) {
            return v_.read(item, vers_);
        } else {
            int server = Sto::server->obj_reside_on(this);
            DoRpcResponse resp;
            DoRpcArgs args;
            args.objid = (int64_t) (TObject *) this;
            args.op = READ_OP;
            TThread::client(server)->do_rpc(resp, args);
            if (!resp.success)
                Sto::abort();
            item.add_read((version_type) resp.version);
            read_type &obj = *(T *) (resp.value.data());
            return obj;
        }
    }

    std::string get_write_value(TransItem& item) {
        std::string buf;
        buf.resize(sizeof(T));
        *(T*) buf.data() = std::move(item.write_value<T>());
        return buf;
    }

    void set_write_value(TransItem& item, Transaction& txn, std::string write_value) {
        const T &val = *(T*) write_value.data();
        TransProxy it(txn, item);
        it.template add_write<T>(val);
    }

    void write(const T& x) {
        Sto::item(this, 0).add_write(x);
    }

    void write(T&& x) {
        Sto::item(this, 0).add_write(std::move(x));
    }

    template <typename... Args>
    void write(Args&&... args) {
        Sto::item(this, 0).template add_write<T>(std::forward<Args>(args)...);
    }

    operator read_type() const {
        return read();
    }

    DistTBox<T, W>& operator=(const T& x) {
        write(x);
        return *this;
    }

    DistTBox<T, W>& operator=(T&& x) {
        write(std::move(x));
        return *this;
    }

    template <typename V>
    DistTBox<T, W>& operator=(V&& x) {
        write(std::forward<V>(x));
        return *this;
    }

    DistTBox<T, W>& operator=(const DistTBox<T, W>& x) {
        write(x.read());
        return *this;
    }

/*
    // valid on local objects only for now
    // this is not atomic either
    const T& nontrans_read(TransactionTid::type *version = nullptr) const {
        if (version != nullptr) {
            *version = vers_.value();
        }
        return v_.access();
    }
*/
    const T& nontrans_read() const {
        return v_.access();
    }


    T& nontrans_access() {
        return v_.access();
    }

    void nontrans_write(const T& x) {
        v_.access() = x;
    }

    void nontrans_write(T&& x) {
        v_.access() = std::move(x);
    }

    // transactional methods
    bool lock(TransItem& item, Transaction& txn) override {
        return txn.try_lock(item, vers_);
    }

    bool check(TransItem& item, Transaction& txn) override {
        return item.check_version(vers_, txn.threadid());
    }

    void install(TransItem& item, Transaction& txn) override {
        v_.write(std::move(item.template write_value<T>()));
        txn.set_version_unlock(vers_, item);
    }

    void unlock(TransItem&) override {
        vers_.unlock();
    }

    void print(std::ostream& w, const TransItem& item) const override {
        w << "{DistTBox<" << typeid(T).name() << "> " << (void*) this;
        if (item.has_read())
            w << " R" << item.read_value<version_type>();
        if (item.has_write())
            w << " =" << item.write_value<T>();
        w << "}";
    }

protected:
    version_type vers_;
    W v_;
};
