#include "DistSTOServer.hh"

DistSTOServer::DistSTOServer(int id, int port) {
    // Currently we can have at most 4 servers, need to change later
    assert( id >= 0 && id < 4);
    _id = id;
    shared_ptr<DistSTOServer> handler(this);
    shared_ptr<TProcessor> processor(new DistSTOProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    _server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory); 
}

typedef DistTBox<int64_t> box_type;

void DistSTOServer::read(std::string& _return, const int64_t objid) {
    // initial hack to avoid dealing with objids - assume it is a pointer to a TObject with type int64_t
    // eventually will need some sort of dynamic dispatch here
    box_type &tbox = *((box_type *) objid);
    _return.resize(sizeof(box_type::read_type));
    int64_t val = tbox.read();
    memcpy((void *) _return.data(), (void *) &val, sizeof(box_type::read_type));
}

// Used to lock modified objects. Return server version if success otherwise a negative value
int64_t DistSTOServer::lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read) {
    TransactionTid::type *version_ptr;
    for (int i = 0; i < version_ptrs.size(); i++) {
        version_ptr = (TransactionTid::type*) version_ptrs[i];
        unsigned n = 0;
        while (1) {
            if (TransactionTid::try_lock(*version_ptr, tuid))
                break;
            ++n;
# if STO_SPIN_EXPBACKOFF
            if (has_read[i] || n == STO_SPIN_BOUND_WRITE) {
                return -1;
            }
            if (n > 3)
                for (unsigned x = 1 << std::min(15U, n - 2); x; --x)
                    relax_fence();
# else
            if (has_read[i] || n == (1 << STO_SPIN_BOUND_WRITE)) {
                return -1;
            }
# endif
            relax_fence();
        } 
    }
    return 0;
}

// Used to check if versions of read object have changed 
bool DistSTOServer::check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, 
           const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    TransactionTid::type *version_ptr;
    for (int i = 0; i < version_ptrs.size(); i++) {
        version_ptr = (TransactionTid::type*) version_ptrs[i];
        if (!TransactionTid::check_version(*version_ptr, old_versions[i])
           && (!may_duplicate_items_ || !preceding_duplicate_read_[i])) {
            return false;
        }
    }
    return true;
}

void DistSTOServer::install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & written_values) {
    printf("install\n");
}

void DistSTOServer::abort(const int32_t tuid) {
    printf("abort %d\n", tuid);
}

