service DistSTO {
    void notify();
    binary do_rpc(1:i64 objid, 2:i64 op, 3:list<binary> opargs);
    i64 lock(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    bool check(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    void install(1:i32 tuid, 2:i64 tid, 3:list<binary> write_values);
    void abort(1:i32 tuid);
}
