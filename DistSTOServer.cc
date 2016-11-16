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
    TransItem *titem;
    assert(_tuid_titems.find(tuid) == _tuid_titems.end());
    Transaction txn = Transaction(tuid);
    int index = 0;
    while (index < titems.size()) {
        titem = (TransItem *) titems[index].data();
        if (!titem->owner()->lock(*titem, txn))
		goto abort_lock;
	titem->__or_flags(TransItem::lock_bit);
        index++;
    }
    _tuid_titems[tuid] = std::move(titems);
    return 0;

abort_lock:
    while (index > 0) {
        index--;
        titem = (TransItem *) titems[index].data();
        titem->owner()->unlock(*titem);
        titem->owner()->cleanup(*titem, false);
    }
    _tuid_titems.erase(tuid);
    return -1;
}

// Used to check if read objects have been modified
bool DistSTOServer::check(const int32_t tuid, const std::vector<std::string> & titems, 
                          const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    TransItem *titem;
    Transaction txn = Transaction(tuid);
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        if (!titem->owner()->check(*titem, txn)
            && (!may_duplicate_items_ || !preceding_duplicate_read_[i])) {
            return false;
        }
    }
    return true;
}

// Used to update modified objects
void DistSTOServer::install(const int32_t tuid, const int64_t tid) { 
    TransItem *titem;
    assert(_tuid_titems.find(tuid) != _tuid_titems.end());
    std::vector<std::string> titems = _tuid_titems.find(tuid)->second;
    Transaction txn = Transaction(tuid, tid);
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        titem->owner()->install(*titem, txn);
	if (titem->needs_unlock())
            titem->owner()->unlock(*titem);
        titem->owner()->cleanup(*titem, true);
    }
    _tuid_titems.erase(tuid);
}

// Used to abort a transaction by unlocking modified objects and do some cleanup
void DistSTOServer::abort(const int32_t tuid) {
    TransItem *titem;
    assert(_tuid_titems.find(tuid) != _tuid_titems.end());
    std::vector<std::string> titems = _tuid_titems.find(tuid)->second;
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        titem->owner()->unlock(*titem);
        titem->owner()->cleanup(*titem, false);
    }
    _tuid_titems.erase(tuid);
}
