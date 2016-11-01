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

  DistSTOServer(int id, int port, bool create) {
    // Currently we can have at most 4 servers, need to change later
    assert( id >= 0 && id < 4);
    _id = id;
    if (create) {
    shared_ptr<DistSTOServer> handler(this);
    shared_ptr<TProcessor> processor(new DistSTOProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    _server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory); 
    }
  }

  // unique server ID assigned by the user
  int id() {
    return _id;
  }
  
  // run the server
  void serve() {
    _server->serve();
  }

  // return the server id that owns the object
  static int obj_reside_on(TObject *obj) {
    std::hash<TObject*> tobject_hash;
    return tobject_hash(obj) % Sto::total_servers;
  }

  // determines if the object is local
  bool is_local_obj(TObject *obj) {
    return obj_reside_on(obj) == _id;
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

  // Used to lock modified objects. Return server version if success otherwise a negative value
  int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read) {
    std::cout << "lock\n";    
    return 0;
  }

  bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & versions) {
    return false;
  }

  void install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & written_values) {
    printf("install\n");
  }

  void abort(const int32_t tuid) {
    printf("abort %d\n", tuid);
  }
};

