service DistSTO {
    binary read(1:i64 objid);
    i64 lock(1:i32 tuid, 2:list<binary> titems);
    bool check(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    void install(1:i32 tuid, 2:i64 tid);
    void abort(1:i32 tuid);
}
