service DistSTO {
    binary read(1:i64 objid);
    i64 lock(1:i32 tuid, 2:list<binary> titems);
    bool check(1:i32 tuid, 2:list<binary> titems, 3:bool may_duplicate_items_, 4:list<bool> preceding_duplicate_read_);
    void install(1:i32 tuid, 2:i64 tid, 3: list<i64> objids, 4: list<i64> version_ptrs, 5:list<binary> written_values);
    void abort(1:i32 tuid, 2: list<i64> unlock_objids);
}
