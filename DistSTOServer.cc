#include "DistSTOServer.hh"
#include "DistTBox.hh"

#include <cassert>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

typedef DistTBox<int64_t> box_type;

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

void DistSTOServer::read(std::string& _return, const int64_t objid) {
    // initial hack to avoid dealing with objids - assume it is a pointer to a TObject with type int64_t
    // eventually will need some sort of dynamic dispatch here
    box_type &tbox = *((box_type *) objid);
    _return.resize(sizeof(TransactionTid::type) + sizeof(box_type::read_type));
    TransactionTid::type version = 0;
    int64_t val = tbox.nontrans_read(&version);
    memcpy((void *) _return.data(), (void *) &version, sizeof(TransactionTid::type));
    memcpy((void *) _return.data() + sizeof(TransactionTid::type), (void *) &val, sizeof(box_type::read_type));
}

#define dprintf(...) printf(__VA_ARGS__)
// #define dprintf(format, ...)

// Used to lock modified objects. Return server version if success otherwise a negative value
int64_t DistSTOServer::lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read) {
    dprintf("lock %d objects ... ", version_ptrs.size());
    for (int i = 0; i < version_ptrs.size(); i++) {
        TransactionTid::type *version = (TransactionTid::type*) version_ptrs[i];
        unsigned n = 0;
        while (1) {
            if (TransactionTid::try_lock(*version, tuid))
                break;
            ++n;
# if STO_SPIN_EXPBACKOFF
            if (has_read[i] || n == STO_SPIN_BOUND_WRITE) {
                dprintf("fail\n");
                return -1;
            }
            if (n > 3)
                for (unsigned x = 1 << std::min(15U, n - 2); x; --x)
                    relax_fence();
# else
            if (has_read[i] || n == (1 << STO_SPIN_BOUND_WRITE)) {
                dprintf("fail\n");
                return -1;
            }
# endif
            relax_fence();
        }
    }
    dprintf("done\n");
    return 0;
}

bool DistSTOServer::check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    dprintf("check %d objects ... ", version_ptrs.size());

    TransactionTid::type *version_ptr;
    for (int i = 0; i < version_ptrs.size(); i++) {
        version_ptr = (TransactionTid::type*) version_ptrs[i];
        if (!TransactionTid::check_version(*version_ptr, old_versions[i])
           && (!may_duplicate_items_ || !preceding_duplicate_read_[i])) {
            return false;
        }
    }
    dprintf("done\n");
    return true;
}

void DistSTOServer::install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & write_objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values) {
    dprintf("install %d objects ... ", write_objids.size());
    if (write_objids.size() != written_values.size()) {
        dprintf("bug\n");
        return;
    }

    for (int i = 0; i < write_objids.size(); i++) {
        box_type &tbox = *((box_type *) write_objids[i]);
        if (written_values[i].size() != sizeof(box_type::read_type)) {
            dprintf("bug\n");
            return;
        }
        auto data = (box_type::read_type *) written_values[i].data();
        tbox.nontrans_write(*data);

        TransactionTid::type *version = (TransactionTid::type *) version_ptrs[i];
        TransactionTid::unlock(*version);
    }
    dprintf("done\n");
}

void DistSTOServer::abort(const int32_t tuid, const std::vector<int64_t> & version_ptrs) {
    dprintf("abort\n");
    for (int i = 0; i < version_ptrs.size(); i++) {
        TransactionTid::type *version = (TransactionTid::type *) version_ptrs[i];
        TransactionTid::unlock(*version);
    }
}
