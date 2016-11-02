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

    DistSTOServer(int id, int port);

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

    void read(std::string& _return, const int64_t objid);

    // Used to lock modified objects. Return server version if success otherwise a negative value
    int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);

    // Used to check if versions of read object have changed 
    bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, 
               const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);

    void install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & written_values);

    void abort(const int32_t tuid);

};

