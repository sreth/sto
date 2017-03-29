struct DoRpcResponse {
    1: required bool success;
    2: required bool boolResp1;
    3: required i64 intResp1;
    4: required i64 intResp2;
    5: required i64 intResp3;
    6: required i64 intResp4;
    7: required binary strResp1;
    8: required list<i64> intListResp1;
    9: required list<i64> intListResp2;
}

struct DoRpcArgs {
    1: required i64 objid;
    2: required i64 op;
    3: required binary strArg1;
    4: required binary strArg2;
    5: required binary strArg3;
    6: required bool boolArg1;
    7: required bool boolArg2;
    8: required i64 intArg1;
}

service DistSTO {
    void notify();
    void advance();
    i64 transmit(1:binary data);
    DoRpcResponse do_rpc(1:DoRpcArgs opargs);
    i64 lock(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    bool check(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    void install(1:i32 tuid, 2:i64 tid, 3:list<binary> write_values);
    void abort(1:i32 tuid);
}
