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

    read_type read(TransactionTid::type *version = nullptr) const {
        auto item = Sto::item(this, 0);
        if (item.has_write()) {
            return item.template write_value<T>();
        } else {
            //if (Sto::server->is_local_obj(this)) {
                return v_.read(item, vers_);
/*
            } else {
                std::string buf;
                Sto::clients[Sto::server->obj_reside_on(this)]->read(buf, (int64_t) this);
                TransactionTid::type v = *(TransactionTid::type *) buf.data();
                if (version != nullptr)
                    *version = v;
                Sto::item(this, 0).add_read(v);
                return *(T *) (buf.data() + sizeof(TransactionTid::type));
            }
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
