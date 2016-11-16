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

    static const int READ_OP = 0;

    void do_rpc(std::string &_return, int64_t op, const std::vector<std::string> &opargs) {
        switch(op) {
        case READ_OP:
            {
                // arguments: none
                // return:
                // first sizeof(version_type) bytes are version
                // next sizeof(read_type) bytes are data
                // data is only valid if the version is unlocked

                // allocate space in return buffer
                _return.resize(sizeof(version_type) + sizeof(T));
                version_type &ver = *(version_type *) _return.data();
                T &obj = *(T *) (_return.data() + sizeof(version_type));

                // TODO: we need to change TWrapped / TransProxy to not use a transaction,
                // so that we can use the usual read() method instead of this loop
                while (1) {
                    version_type v = vers_;
                    fence();
                    obj = v_.access();
                    fence();
                    ver = vers_;
                    if (v == ver || ver.is_locked()) {
                        break;
                    }
                    relax_fence();
                }

                break;
            }
        }
    }

    read_type read() const {
        auto item = Sto::item(this, 0);
        if (item.has_write()) {
            return item.template write_value<T>();
        } else if (Sto::server->is_local_obj(this)) {
                return v_.read(item, vers_);
        } else {
		return v_.read(item, vers_);
	/*
            std::string buf;
            std::vector<std::string> args;
            int server = Sto::server->obj_reside_on(this);
            Sto::clients[server]->do_rpc(buf, (int64_t) this, READ_OP, args);

            version_type &ver = *(version_type *) buf.data();
            item.add_read(ver);
            read_type &obj = *(T *) (buf.data() + sizeof(version_type));
            return obj;
	*/
        }
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

    bool check(TransItem& item, Transaction&) override {
        return item.check_version(vers_);
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
