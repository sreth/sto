#pragma once
#include "DistSTO.h"
#include "Interface.hh"
#include "TWrapped.hh"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/server/TSimpleServer.h>

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
    static int obj_reside_on(const TObject *obj) {
        return obj_reside_on(const_cast<TObject*>(obj));
    }

    // determines if the object is local
    bool is_local_obj(TObject *obj) {
        return obj_reside_on(obj) == _id;
    }
    bool is_local_obj(const TObject *obj) {
        return obj_reside_on(obj) == _id;
    }


    void read(std::string& _return, const int64_t objid);

    // Used to lock modified objects. Return server version if success otherwise a negative value
    int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);

    // Used to check if versions of read object have changed 
    bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, 
               const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);

    void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & write_objids, 
                 const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);

    void abort(const int32_t tuid, const std::vector<int64_t> & version_ptrs);

};

