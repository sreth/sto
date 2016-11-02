#pragma once
#include "DistSTO.h"
#include "Interface.hh"
#include "TWrapped.hh"

#include <thrift/server/TSimpleServer.h>

class DistSTOServer : virtual public DistSTOIf {
private:
    int _id;
    apache::thrift::server::TSimpleServer *_server;

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
    static int obj_reside_on(const TObject *obj) {
        std::hash<TObject*> tobject_hash;
        return tobject_hash(const_cast<TObject *>(obj)) % Sto::total_servers;
    }

    // determines if the object is local
    bool is_local_obj(const TObject *obj) {
        return obj_reside_on(obj) == _id;
    }

    void read(std::string& _return, const int64_t objid);
    int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);
    bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & versions);
    void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & write_objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);
    void abort(const int32_t tuid, const std::vector<int64_t> & version_ptrs);
};

