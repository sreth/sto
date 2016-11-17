#include "DistSTOServer.hh"

#include <cassert>

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

// Phase 1
// Used to lock modified objects. If the objects are also read, perform checking (phase 2) as well.
// Return server version if success otherwise a negative value.
int64_t DistSTOServer::lock(const int32_t tuid, const std::vector<std::string> & titems, 
                            const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    TransItem *titem;
    assert(_tuid_titems.find(tuid) == _tuid_titems.end());
    Transaction txn = Transaction(tuid);
    int rindex = 0; // index for preceding_duplicate_read_
    int index = 0;
    while (index < titems.size()) {
        titem = (TransItem *) titems[index].data();
	// perform phase 1 for write objects
        if (!titem->owner()->lock(*titem, txn))
		goto abort_lock;
	titem->__or_flags(TransItem::lock_bit);
	// perform phase 2 for read-write objects
	if (titem->has_read()) {
            assert(rindex < preceding_duplicate_read_.size());
            if (!titem->owner()->check(*titem, txn)
                && (!may_duplicate_items_ || !preceding_duplicate_read_[rindex])) {
	        titem->owner()->unlock(*titem);
                titem->owner()->cleanup(*titem, false);
                goto abort_lock;
	    } else {
                rindex++;
            }
        }
        index++;
    }
    assert(rindex == preceding_duplicate_read_.size());
    _tuid_titems[tuid] = std::move(titems);

    // TODO:: return server version 
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
// Used to check if read-only objects have been modified.
bool DistSTOServer::check(const int32_t tuid, const std::vector<std::string> & titems, 
                          const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    TransItem *titem;
    Transaction txn = Transaction(tuid);
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
	assert(!titem->has_write());
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
