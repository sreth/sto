#pragma once
#include "RPC.h"
#include "Interface.hh"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/server/TSimpleServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class DistSTORPCHandler : virtual public RPCIf {
private:
    TSimpleServer *_server;

public:
    DistSTORPCHandler(int port) {
        shared_ptr<DistSTORPCHandler> handler(this);
        shared_ptr<TProcessor> processor(new RPCProcessor(handler));
        shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
        shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
        shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
        _server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);
    }

    // run the server
    void handle() {
        _server->serve();
    }

    void do_rpc(std::string& _return, const int64_t objid, const int64_t op, const std::vector<std::string> & opargs);
};
