#include "DistSTOServer.hh"
#include "DistTBox.hh"

#include <cassert>

typedef DistTBox<int64_t> box_type;

void DistSTOServer::do_rpc(std::string& _return, const int64_t objid, const int64_t op, const std::vector<std::string> &opargs) {
    TObject &obj = *((TObject *) objid);
    obj.do_rpc(_return, op, opargs);
}

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

// Phase 1
// Used to lock modified objects. Return server version if success otherwise a negative value.
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

// Phase 2
// Used to check if read objects have been modified.
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

// Phase 3
// Used to update modified objects
void DistSTOServer::install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & write_values) {
    TransItem *titem;
    assert(_tuid_titems.find(tuid) != _tuid_titems.end());
    std::vector<std::string> titems = _tuid_titems.find(tuid)->second;
    Transaction txn = Transaction(tuid, tid);
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        titem->owner()->set_write_value(*titem, write_values[i]);
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
