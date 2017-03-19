#include "DistMassTrans.hh"
#include "IntStr.hh"
#include <assert.h>

using namespace std;

volatile mrcu_epoch_type active_epoch = 1;

template <typename T> 
class IntDistMassTrans {
    DistMassTrans<T> m_;
public:
    bool transGet(int k, T& v) {
        return m_.transGet(IntStr(k).str(), v);
    }
    bool transPut(int k, T v) {
        return m_.transPut(IntStr(k).str(), v);
    }
    bool transUpdate(int k, T v) {
        return m_.transUpdate(IntStr(k).str(), v);
    }
    bool transInsert(int k, T v) {
        return m_.transInsert(IntStr(k).str(), v);
    }
    bool transDelete(int k) {
        return m_.transDelete(IntStr(k).str());
    }
    void thread_init() {
        m_.thread_init();
    }
};

void insertDeleteTest(DistMassTrans<int> h, bool shouldAbort) {
  {
      TransactionGuard t;
      for (int i = 10; i < 25; ++i) {
          assert(h.transInsert(IntStr(i).str(), i+1));
      }
  }

  TestTransaction t2(1);
  assert(h.transInsert(IntStr(25).str(), 26));
  int x;
  assert(h.transGet(IntStr(25).str(), x));
  assert(!h.transGet(IntStr(26).str(), x));

  assert(h.transDelete(IntStr(25).str()));

  if (shouldAbort) {
      TestTransaction t3(2);
      assert(h.transInsert(IntStr(26).str(), 27));
      assert(t3.try_commit());
      assert(!t2.try_commit());
  } else
      assert(t2.try_commit());

  printf("PASS: %s\n", __FUNCTION__);
}

void insertDeleteSeparateTest(DistMassTrans<int> h) {
  {
      TransactionGuard t_init;
      for (int i = 10; i < 12; ++i) {
          assert(h.transInsert(IntStr(i).str(), i+1));
      }
  }

  TestTransaction t(1);
  int x;
  assert(!h.transGet(IntStr(12).str(), x));
  // need a write as well otherwise this txn would successfully commit as read-only
  h.transPut(IntStr(1000).str(), 0);

  TestTransaction t2(2);
  assert(h.transInsert(IntStr(12).str(), 13));
  assert(h.transDelete(IntStr(10).str()));
  assert(t2.try_commit());
  assert(!t.try_commit());


  TestTransaction t3(3);
  assert(!h.transGet(IntStr(13).str(), x));
  // need a write as well otherwise this txn would successfully commit as read-only
  h.transPut(IntStr(1000).str(), 0);

  TestTransaction t4(4);
  assert(h.transInsert(IntStr(10).str(), 11));
  assert(h.transInsert(IntStr(13).str(), 14));
  assert(h.transDelete(IntStr(11).str()));
  assert(h.transDelete(IntStr(12).str()));
  assert(t4.try_commit());
  assert(!t3.try_commit());

  printf("PASS: %s\n", __FUNCTION__);
}

void rangeQueryTest(DistMassTrans<int> h) {
  int n = 99;
  char ns[64];

  {
      TransactionGuard t_init;
      sprintf(ns, "%d", n);
      for (int i = 10; i <= n; ++i) {
          assert(h.transInsert(IntStr(i).str(), i+1));
      }
  }

  {
  TransactionGuard t;
  int x = 0;
  h.transQuery("10", Masstree::Str(), [&] (Masstree::Str , int ) { x++; return true; });
  assert(x == n-10+1);
  
  x = 0;
  h.transQuery("10", ns, [&] (Masstree::Str , int) { x++; return true; });
  assert(x == n-10);

  x = 0;
  h.transRQuery(ns, Masstree::Str(), [&] (Masstree::Str , int ) { x++; return true; });
  assert(x == n-10+1);
  
  x = 0;
  h.transRQuery(ns, "90", [&] (Masstree::Str , int ) { x++; return true; });
  assert(x == n-90);

  x = 0;
  h.transQuery("10", "25", [&] (Masstree::Str , int ) { x++; return true; });
  assert(x == 25-10);

  x = 0;
  h.transQuery("10", "26", [&] (Masstree::Str , int ) { x++; return true; });
  assert(x == 26-10);
  }

  printf("PASS: %s\n", __FUNCTION__);
}

template <typename K, typename V>
void basicQueryTests(DistMassTrans<K, V>& h) {
  TransactionGuard t19;
  h.transQuery("0", "2", [] (Masstree::Str s, int val) { printf("%s, %d\n", s.data(), val); return true; });
  h.transQuery("4", "4", [] (Masstree::Str s, int val) { printf("%s, %d\n", s.data(), val); return true; });

  printf("PASS: %s\n", __FUNCTION__);
}

template <typename T>
void basicQueryTests(T&) {}

template <typename MapType>
void basicMapTests(MapType& h) {
  typedef int Value;
  Value v1,v2,vunused;

  {
      TransactionGuard t;
      //  assert(!h.transGet(t, 0, v1));
      assert(h.transInsert(0, 1));
      h.transPut(1, 3);
  }

  {
      TransactionGuard tm;
      assert(h.transUpdate(1, 2));
  }

  {
      TransactionGuard t2;
      assert(h.transGet(1, v1));
  }

  {
      TransactionGuard t3;
      h.transPut(0, 4);
  }
  {
      TransactionGuard t4;
      assert(h.transGet(0, v2));
  }

  {
      TransactionGuard t5;
      assert(!h.transInsert(0, 5));
  }

  {
      TransactionGuard t6;
      assert(!h.transUpdate(2, 1));
  }

  TestTransaction t7(1);
  assert(!h.transGet(2, vunused));
  // need a write as well otherwise this txn would successfully commit as read-only
  h.transPut(1000, 0);
  TestTransaction t8(2);
  assert(h.transInsert(2, 2));
  assert(t8.try_commit());

  assert(!t7.try_commit());

  TestTransaction t9(3);
  assert(h.transInsert(3, 0));
  TestTransaction t10(4);
  assert(h.transInsert(4, 4));
  try{
    // t9 inserted invalid node, so we are forced to abort
    h.transUpdate(3, vunused);
    assert(0);
  } catch (Transaction::Abort E) {}
  TestTransaction t10_2(5);
  try {
    // deletes should also force abort from invalid nodes
    h.transDelete(3);
    assert(0);
  } catch (Transaction::Abort E) {}
  assert(t9.try_commit());
  assert(!t10.try_commit() && !t10_2.try_commit());

  {
      TransactionGuard t11;
      assert(h.transInsert(4, 5));
  }

  // basic delete
  {
      TransactionGuard t12;
      assert(!h.transDelete(5));
      assert(h.transUpdate(4, 0));
      assert(h.transDelete(4));
      assert(!h.transGet(4, vunused));
      assert(!h.transUpdate(4, 0));
      assert(!h.transDelete(4));
  }

  // delete-then-insert
  {
      TransactionGuard t13;
      assert(h.transGet(3, vunused));
      assert(h.transDelete(3));
      assert(h.transInsert(3, 1));
      assert(h.transGet(3, vunused));
  }

  // insert-then-delete
  {
      TransactionGuard t14;
      assert(!h.transGet(4, vunused));
      assert(h.transInsert(4, 14));
      assert(h.transGet(4, vunused));
      assert(h.transDelete(4));
      assert(!h.transGet(4, vunused));
  }

  // blind update failure
  TestTransaction t15(1);
  assert(h.transUpdate(3, 15));
  TestTransaction t16(2);
  assert(h.transUpdate(3, 16));
  assert(t16.try_commit());
  // blind updates conflict each other now (not worth the extra trouble)
  assert(!t15.try_commit());


  // update aborts after delete
  TestTransaction t17(1);
  assert(h.transUpdate(3, 17));
  TestTransaction t18(2);
  assert(h.transDelete(3));
  assert(t18.try_commit());
  assert(!t17.try_commit());

  basicQueryTests(h);

  printf("PASS: %s\n", __FUNCTION__);
}


IntDistMassTrans<int> m1;
DistMassTrans<int> m2;
DistMassTrans<int> m3;
DistMassTrans<int> m4;
DistMassTrans<int> m5;

int main() {
  int server_id = 0;
  int total_servers = 1;
  Sto::start_dist_sto(server_id, total_servers);

  int thread_id = TThread::get_global_id(0);
  int64_t version = Sto::server->version();
  TThread::init(thread_id, version);

  m1.thread_init();

  basicMapTests(m1);

  // insert-then-delete node test
  insertDeleteTest(m2, false);
  insertDeleteTest(m3, true);

  // insert-then-delete problems with masstree version numbers (currently fails)
  insertDeleteSeparateTest(m4);

  rangeQueryTest(m5);

  // string key testing
  //stringKeyTests();

  //linkedListTests();
  
  //queueTests();

  TThread::cleanup();

  Sto::end_dist_sto();
 
  printf("PASS: %s\n", __FUNCTION__);
}
