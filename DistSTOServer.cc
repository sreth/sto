#include "DistSTOServer.hh"
#include "Transaction.hh"

#include <chrono>
#include <thread>
#include <cassert>

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

void DistSTOServer::notify() {
   _lock.lock();
   _connections++;
   _lock.unlock();
}

void DistSTOServer::broadcast() {
    for (int i = 0; i < Sto::total_servers; i++) {
        if (i != _id) {
            Sto::clients[i]->notify();
        }
    }
}

void DistSTOServer::wait() {
    DistSTOServer::broadcast();
    _lock.lock();
    while (_connections != Sto::total_servers - 1) {
        _lock.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(1));
        _lock.lock();
    }
    _connections = 0;
}

void DistSTOServer::do_rpc(std::string& _return, const int64_t objid, const int64_t op, const std::vector<std::string> & opargs) {
    TObject &obj = *((TObject *) objid);
    obj.do_rpc(_return, op, opargs);
}

// Phase 1
// Used to lock modified objects. If the objects are also read, perform checking (phase 2) as well.
// Return server version if success otherwise a negative value.
int64_t DistSTOServer::lock(const int32_t tuid, const std::vector<std::string> & titems, 
                            const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    TransItem *titem;

    _lock.lock();
    assert(_tuid_titems.find(tuid) == _tuid_titems.end());
    _lock.unlock();

    Transaction txn = Transaction(tuid);
    int rindex = 0; // index for preceding_duplicate_read_
    int index = 0;
    bool abort = false;
    while (index < titems.size()) {
        titem = (TransItem *) titems[index].data();
	// perform phase 1 for write objects
        if (!titem->owner()->lock(*titem, txn)) {
            abort = true;
            break;
        }
	titem->__or_flags(TransItem::lock_bit);
	// perform phase 2 for read-write objects
	if (titem->has_read()) {
            assert(rindex < preceding_duplicate_read_.size());
            if (!titem->owner()->check(*titem, txn)
                && (!may_duplicate_items_ || !preceding_duplicate_read_[rindex])) {
	        titem->owner()->unlock(*titem);
                titem->owner()->cleanup(*titem, false);
                abort = true;
                break;
	    } else {
                rindex++;
            }
        }
        index++;
    }
    if (abort) {
        while (index > 0) {
            index--;
            titem = (TransItem *) titems[index].data();
            titem->owner()->unlock(*titem);
            titem->owner()->cleanup(*titem, false);
        }

        _lock.lock();
        _tuid_titems.erase(tuid);
        _lock.unlock();
        return -1;
    }
    assert(rindex == preceding_duplicate_read_.size());

    _lock.lock();
    auto &new_titems = _tuid_titems[tuid]; // initializes the entry
    _lock.unlock();
    new_titems = std::move(titems);

    fence(); // fence is sufficient since we just acquired locks
    return (int64_t) Transaction::_TID;
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

    _lock.lock();
    assert(_tuid_titems.find(tuid) != _tuid_titems.end());
    auto &titems = _tuid_titems[tuid];
    _lock.unlock();

    Transaction txn = Transaction(tuid, tid);
    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        titem->owner()->set_write_value(*titem, txn, write_values[i]);
        titem->owner()->install(*titem, txn);
	if (titem->needs_unlock())
            titem->owner()->unlock(*titem);
        titem->owner()->cleanup(*titem, true);
    }

    _lock.lock();
    _tuid_titems.erase(tuid);
    _lock.unlock();

    TransactionTid::type new_tid, old_tid;
    do {
        old_tid = Transaction::_TID;
        new_tid = std::max(old_tid, tid + TransactionTid::increment_value);
    } while(!cmpxchg(&Transaction::_TID, old_tid, new_tid));
}

// Used to abort a transaction by unlocking modified objects and do some cleanup
void DistSTOServer::abort(const int32_t tuid) {
    TransItem *titem;

    _lock.lock();
    assert(_tuid_titems.find(tuid) != _tuid_titems.end());
    auto &titems = _tuid_titems[tuid];
    _lock.unlock();

    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        titem->owner()->unlock(*titem);
        titem->owner()->cleanup(*titem, false);
    }

    _lock.lock();
    _tuid_titems.erase(tuid);
    _lock.unlock();
}
