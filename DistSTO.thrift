service DistSTO {
    binary read(1:i64 objid);
    i64 lock(1:i32 tuid, 2:list<i64> version_ptrs, 3:list<bool> has_read);
    bool check(1:i32 tuid, 2:list<i64> version_ptrs, 3:list<i64> versions);
    void install(1:i32 tuid, 2:i64 tid, 3:list<binary> written_values);
    void abort(1:i32 tuid);
}


