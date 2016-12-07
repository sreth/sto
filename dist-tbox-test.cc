#undef NDEBUG
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include "Transaction.hh"
#include "DistTBox.hh"
#include "StringWrapper.hh"
#include "DistSTOServer.hh"

DistTBox<int> c;

int count_per_thread = 10000;

struct Args {
    int thread_id;
    int total_threads;
};

// All these tests are run on 4 different servers

void* simpleCount(void *input) {
    struct Args* args = (struct Args*) input;
    int thread_id = args->thread_id;
    int nthreads = args->total_threads;
    int64_t version = Sto::server->version();
    TThread::init(thread_id, version);
    
    // only thread 0 of owner should initialize the object
    if (Sto::server->is_local_obj(&c) && TThread::id() == 0) {
        TransactionGuard t;
        c = 0;
    }
 
    // wait till everyone is done with initialization
    Sto::server->wait(nthreads);
    
    for (int i = 0; i < count_per_thread; i++) {
        TRANSACTION {
            int c_read = c;
            c_read++;
            c = c_read;
        } RETRY(true);
    }

    // wait till eveyone is done with counting 
    Sto::server->wait(nthreads);

    {
        TransactionGuard t;
        int c_read = c;
        if (c_read != count_per_thread * nthreads) {
	    std::cout << "Server " << Sto::server->id() << " thread " << TThread::id() << " gets total = " << c_read << "\n";
        }
        assert(c_read == count_per_thread * nthreads);
    }

    // wait till everyone is done with the above transaction
    Sto::server->wait(nthreads);
}


void testSimpleCountWithNThreads(int N) {
    
    int nthreads_per_server = N;
    int nthreads = nthreads_per_server * Sto::total_servers;
    pthread_t tids[nthreads_per_server];

    for (int i = 0; i < nthreads_per_server; i++) {
        struct Args* args = new struct Args();
        args->thread_id = i;
        args->total_threads = nthreads;
        pthread_create(&tids[i], NULL, simpleCount, args);
    }

    for (int i = 0; i < nthreads_per_server; i++) {
        pthread_join(tids[i], NULL);
    }
   
}

void testSimpleCountWith1Thread() {
   
    testSimpleCountWithNThreads(1);

    printf("PASS: %s\n", __FUNCTION__);
}

void testSimpleCountWith4Threads() {
   
    testSimpleCountWithNThreads(4);

    printf("PASS: %s\n", __FUNCTION__);
}

void testSimpleCountWith8Threads() {
   
    testSimpleCountWithNThreads(8);

    printf("PASS: %s\n", __FUNCTION__);
}

int main(int argc, char *argv[]) {
    int server_id = atoi(argv[1]);
    int total_servers = atoi(argv[2]);
    Sto::start_dist_sto(server_id, total_servers);
    testSimpleCountWith1Thread();
    testSimpleCountWith4Threads();
    testSimpleCountWith8Threads();
    Sto::end_dist_sto();
    return 0;
}
