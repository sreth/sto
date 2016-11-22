#pragma once
#include "DistSTO.h"
#include "Interface.hh"
#include "TWrapped.hh"

#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h> 
#include <server/TThreadPoolServer.h> 

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

class DistSTOServer : virtual public DistSTOIf {
private:
    int _id;
    TThreadPoolServer *_server;
    std::unordered_map<int32_t, std::vector<std::string>> _tuid_titems;

public:
    DistSTOServer(int id, int port) {
        // Currently we can have at most 4 servers, need to change later
        assert( id >= 0 && id < 4);
        _id = id;
        shared_ptr<DistSTOServer> handler(this);
        shared_ptr<TProcessor> processor(new DistSTOProcessor(handler));
        shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
        shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
        shared_ptr<TProtocolFactory> protocolFactory(new TCompactProtocolFactory());

	// the number of threads should be the same as the number of clients 
	// so that each client connection has a dedicated server thread
	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(32);
	shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();

	// TThreadPoolServer, though is faster than other server types, consumes a lot of resources
	// Alternative is to use THsHaServer
        _server = new TThreadPoolServer(processor, serverTransport, transportFactory, protocolFactory, threadManager);
        _tuid_titems = std::unordered_map<int32_t, std::vector<std::string>>();
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

    void do_rpc(std::string& _return, const int64_t objid, const int64_t op, const std::vector<std::string> & opargs);

    int64_t lock(const int32_t tuid, const std::vector<std::string> & titems, const bool may_duplicate_items_, 
                 const std::vector<bool> & preceding_duplicate_read_);
 
    bool check(const int32_t tuid, const std::vector<std::string> & titems, const bool may_duplicate_items_, 
               const std::vector<bool> & preceding_duplicate_read_);

    void install(const int32_t tuid, const int64_t tid, const std::vector<std::string> & write_values);

    void abort(const int32_t tuid);
};

