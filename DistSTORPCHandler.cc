#include "DistSTORPCHandler.hh"

void DistSTORPCHandler::do_rpc(std::string& _return, const int64_t objid, const int64_t op, const std::vector<std::string> & opargs) {
    TObject &obj = *((TObject *) objid);
    obj.do_rpc(_return, op, opargs);
}

