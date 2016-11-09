#include "DistSTOServer.hh"
#include "DistTBox.hh"

#include <cassert>

typedef DistTBox<int64_t> box_type;

void DistSTOServer::read(std::string& _return, const int64_t objid) {
/*
    // initial hack to avoid dealing with objids - assume it is a pointer to a TObject with type int64_t
    // eventually will need some sort of dynamic dispatch here
    box_type &tbox = *((box_type *) objid);
    _return.resize(sizeof(TransactionTid::type) + sizeof(box_type::read_type));
    TransactionTid::type version = 0;
    int64_t val = tbox.nontrans_read(&version);
    memcpy((void *) _return.data(), (void *) &version, sizeof(TransactionTid::type));
    memcpy((void *) _return.data() + sizeof(TransactionTid::type), (void *) &val, sizeof(box_type::read_type));
*/
}

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

// Used to lock modified objects. Return server version if success otherwise a negative value
int64_t DistSTOServer::lock(const int32_t tuid, const std::vector<std::string> & titems) {
    TransItem titem;
    Transaction txn = Transaction(tuid);
    int index = 0;
    while (index < titems.size()) {
        titem = *((TransItem *) titems[index].data());
        if (!titem.owner()->lock(titem, txn))
		goto abort_lock;
        index++;
    }
    return 0;

abort_lock:
    while (index > 0) {
        index--;
        titem = *((TransItem *) titems[index].data());
        titem.owner()->unlock(titem);
    }
    return -1;
}

// Used to check if versions of read object have changed 
bool DistSTOServer::check(const int32_t tuid, const std::vector<std::string> & titems, const bool may_duplicate_items_, 
           const std::vector<bool> & preceding_duplicate_read_) {
    TransItem titem;
    Transaction txn = Transaction(tuid);
    for (int i = 0; i < titems.size(); i++) {
        titem = *((TransItem *) titems[i].data());
        if (!titem.owner()->check(titem, txn)
            && (!may_duplicate_items_ || !preceding_duplicate_read_[i])) {
            return false;
        }
    }
    return true;
}
 
void DistSTOServer::install(const int32_t tuid, const int64_t tid, const int8_t state, const std::vector<std::string> & titems) {
    TransItem titem;
    Transaction txn = Transaction(tuid, state, tid);
    for (int i = 0; i < titems.size(); i++) {
        titem = *((TransItem *) titems[i].data());
        titem.owner()->install(titem, txn);
    }
}

void DistSTOServer::abort(const int32_t tuid, const std::vector<int64_t> & version_ptrs) {
    dprintf("abort\n");
    for (int i = 0; i < version_ptrs.size(); i++) {
        TransactionTid::type *version = (TransactionTid::type *) version_ptrs[i];
        TransactionTid::unlock(*version);
    }
}
