#include "DistSTOServer.hh"
#include "Transaction.hh"

#include <chrono>
#include <thread>
#include <cassert>

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

void DistSTOServer::do_rpc(DoRpcResponse& _return, const DoRpcArgs& opargs) {
    TObject &obj = *((TObject *) opargs.objid);
    obj.do_rpc(_return, opargs);
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
                if (titem->needs_unlock())
	            titem->owner()->unlock(*titem);
                if (titem->has_write())
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
            if (titem->needs_unlock())
                titem->owner()->unlock(*titem);
            if (titem->has_write()) 
                titem->owner()->cleanup(*titem, false);
        }
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

    TransactionTid::type new_tid, old_tid;
    do {
        old_tid = Transaction::_TID;
        new_tid = std::max(old_tid, (uint64_t) tid);
    } while(cmpxchg(&Transaction::_TID, old_tid, new_tid) != old_tid);

    fence();

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
        if (titem->has_write())
            titem->owner()->cleanup(*titem, true);
    }

    _lock.lock();
    _tuid_titems.erase(tuid);
    _lock.unlock();
}

// Used to abort a transaction by unlocking modified objects and do some cleanup
void DistSTOServer::abort(const int32_t tuid) {
    TransItem *titem;
    _lock.lock();
    if (_tuid_titems.find(tuid) == _tuid_titems.end()) {
        _lock.unlock();
        return;
    }
    auto &titems = _tuid_titems[tuid];
    _lock.unlock();

    for (int i = 0; i < titems.size(); i++) {
        titem = (TransItem *) titems[i].data();
        if (titem->needs_unlock())
            titem->owner()->unlock(*titem);
        if (titem->has_write())
            titem->owner()->cleanup(*titem, false);
    }

    _lock.lock();
    _tuid_titems.erase(tuid);
    _lock.unlock();
}

// Function used to hash address 
// Return a 32-bit value 
uint32_t DistSTOServer::murmur3_32(const char *key, uint32_t len, uint32_t seed) {
   static const uint32_t c1 = 0xcc9e2d51;
   static const uint32_t c2 = 0x1b873593;
   static const uint32_t r1 = 15;
   static const uint32_t r2 = 13;
   static const uint32_t m = 5;
   static const uint32_t n = 0xe6546b64;
 
   uint32_t hash = seed;
 
   const int nblocks = len / 4;
   const uint32_t *blocks = (const uint32_t *) key;
 
   const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
   uint32_t k1 = 0;
 
   int i;
   for (i = 0; i < nblocks; i++) {
      uint32_t k = blocks[i];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
 
      hash ^= k;
      hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
   }
   
   switch (len & 3) {
   case 3:
      k1 ^= tail[2] << 16;
   case 2:
      k1 ^= tail[1] << 8;
   case 1:
      k1 ^= tail[0];
 
      k1 *= c1;
      k1 = (k1 << r1) | (k1 >> (32 - r1));
      k1 *= c2;
      hash ^= k1;
   }
 
   hash ^= len;
   hash ^= (hash >> 16);
   hash *= 0x85ebca6b;
   hash ^= (hash >> 13);
   hash *= 0xc2b2ae35;
   hash ^= (hash >> 16);
 
   return hash;
}

// Hash a 64-bit address and return a 32-bit value 
uint32_t DistSTOServer::hash(uint64_t addr) {
   const char *key = (const char *) &addr;
   uint32_t len = sizeof(addr) / sizeof(char);
   return murmur3_32(key, len, 0);
}

// Below methods are used only for testing

void DistSTOServer::notify() {
    assert(Sto::server->id() == 0);
    _lock.lock();
    _nthreads++;
    _lock.unlock();
}

void DistSTOServer::advance() {
    assert(Sto::server->id() != 0);
    _lock.lock();
    _version++;
    _lock.unlock();
}

// Designate thread 0 of server 0 as the coordinator
void DistSTOServer::wait(int total_threads) {
    // advance thread's version
    TThread::advance();
    // if this thread is not the coordinator
    if (Sto::server->id()) {
        // inform the coordinator
        TThread::client(0)->notify();
        _lock.lock();
        // spin until server's version reaches this thread's version
        while (TThread::version() > _version) {
            _lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            _lock.lock();
        }
        _lock.unlock();
    } else {
        _lock.lock();
        _nthreads++;
        // if this thread is not the coordinator
        if (TThread::id()) {
            while (TThread::version() > _version) {
                _lock.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                _lock.lock();
            }
            _lock.unlock();
        } else {
            // this thread is the coordinator
            while (_nthreads < total_threads) {
                _lock.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                _lock.lock();
            }
            _nthreads -= total_threads;
            assert(_nthreads == 0);
            // tell threads on this server to proceed
            _version++;
            _lock.unlock();
            // tell threads on other servers to proceed
            for (int i = 1; i < Sto::total_servers; i++)
                TThread::client(i)->advance();
        }
    } 
}

// Echo the data back to the client
int64_t DistSTOServer::transmit(const std::string& data) {
    return 1;
}
