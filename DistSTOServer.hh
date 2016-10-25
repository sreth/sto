#include "DistSTO.h"
#include "DistTBox.hh"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class DistSTOServer : virtual public DistSTOIf {

 private:
  int _id;
  TSimpleServer *_server;
 public:

  DistSTOServer(int id, int port) {
    _id = id;
    shared_ptr<DistSTOServer> handler(this);
    shared_ptr<TProcessor> processor(new DistSTOProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    _server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory); 
  }

  int id() {
    return _id;
  }

  void serve() {
    _server->serve();
  }

  typedef DistTBox<int64_t> box_type;

  void read(std::string& _return, const int64_t objid) {
    // initial hack to avoid dealing with objids - assume it is a pointer to a TObject with type int64_t
    // eventually will need some sort of dynamic dispatch here
    box_type &tbox = *((box_type *) objid);
    _return.resize(sizeof(box_type::read_type));
    int64_t val = tbox.read();
    memcpy((void *) _return.data(), (void *) &val, sizeof(box_type::read_type));
  }

  bool lock(const int32_t tuid, const std::vector<int64_t> & objids, const std::vector<int64_t> & keys) {
    // This tuid should not exist yet in tuid_objids map
    assert(Sto::tuid_objids_map.find(tuid) == Sto::tuid_objids_map.end());
    // record tuid for later use
    Sto::tuid_objids_map[tuid] = objids;
    // lock all modified objects
    for (auto objid : objids) {
      // this object should already be registered
      assert(Sto::objid_obj_map.find(objid) != Sto::objid_obj_map.end());
      // this object should be local
      assert(Sto::is_local_obj(objid));


#if STO_SORT_WRITESET
      //XXX: figure out how to deal with ver
      TransactionTid::lock(vers, threadid_);
      continue;
#else
        // This function will eventually help us track the commit TID when we
        // have no opacity, or for GV7 opacity.
        unsigned n = 0;
        while (1) {
            if (TransactionTid::try_lock(vers, threadid_))
                return true;
            ++n;
# if STO_SPIN_EXPBACKOFF
            if (item.has_read() || n == STO_SPIN_BOUND_WRITE) {
#  if STO_DEBUG_ABORTS
                abort_version_ = vers;
#  endif
                return false;
            }
            if (n > 3)
                for (unsigned x = 1 << std::min(15U, n - 2); x; --x)
                    relax_fence();
# else
            if (item.has_read() || n == (1 << STO_SPIN_BOUND_WRITE)) {
#  if STO_DEBUG_ABORTS
                abort_version_ = vers;
#  endif
                return false;
            }
# endif
            relax_fence();
        }
#endif
     


    }
  }

  bool check(const int32_t tuid, const std::vector<int64_t> & objids, const std::vector<int64_t> & versions) {
    return false;
  }

  void install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & written_values) {
    printf("install\n");
  }

  void abort(const int32_t tuid) {
    printf("abort %d\n", tuid);
  }
};

