// #undef NDEBUG
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include "Transaction.hh"
#include "DistTBox.hh"
#include "StringWrapper.hh"
#include "DistSTOServer.hh"
#include <fstream>

#define TOTAL_BOXES 1000000

using namespace std::chrono;

DistTBox<int> boxes[TOTAL_BOXES];

std::ofstream myfile;

int total_transactions = 1000;

struct Args {
    int thread_id;
    int total_threads;
    int p;
};

int rand(int i, int j) {
    return std::rand() % (j - i) + i;
}

// 0 <= p < 0.5
int nextBox(float p) {
    int bound = int(p * TOTAL_BOXES);
    if (rand(0, 2) == 0)
        return rand(0, bound);
    else
        return rand(bound, TOTAL_BOXES); 
}

void* simpleCount(void *input) {
    struct Args* args = (struct Args*) input;
    int thread_id = TThread::get_global_id(args->thread_id);
    int total_threads = total_threads;
    int p = args->p;

    int64_t version = Sto::server->version();
    TThread::init(thread_id, version);

    for (int i = 0; i < TOTAL_BOXES; i++) { 
        // only thread 0 of the owner should initialize the object
        if (Sto::server->is_local_obj(&boxes[i]) && TThread::id() % 8 == 0) {
            TransactionGuard t;
            boxes[i] = 0;
        }
    }

    // wait till everyone is done with initialization
    Sto::server->wait(total_threads);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
       
    for (int i = 0; i < total_transactions; i++) {
        TRANSACTION {
            for (int j = 0; j < TOTAL_BOXES; j++) {
                int index = nextBox(p);
                int c_read = boxes[index];
                c_read++;
                boxes[index] = c_read;
            }
        } RETRY(true);
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    microseconds period = duration_cast<microseconds>( t2 - t1 );

    // wait till eveyone is done with counting 
    Sto::server->wait(total_threads);

    int sum = 0; 
    for (int i = 0; i < TOTAL_BOXES; i++) {
        TransactionGuard t;
        int c_read = boxes[i];
        sum += c_read;
    }
    assert(sum == total_transactions * TOTAL_BOXES);

    // wait till everyone is done with checking
    Sto::server->wait(total_threads);

    TThread::cleanup();
    delete input;
}


void testSimpleCountWithP(int p) {
    int nthreads_per_server = 8;
    int nthreads = nthreads_per_server * Sto::total_servers;
    pthread_t tids[nthreads_per_server];

    for (int i = 0; i < nthreads_per_server; i++) {
        struct Args* args = new struct Args();
        args->thread_id = i;
        args->total_threads = nthreads;
        args->p = p;
        pthread_create(&tids[i], NULL, simpleCount, args);
    }

    for (int i = 0; i < nthreads_per_server; i++) {
        pthread_join(tids[i], NULL);
    }
}


int main(int argc, char *argv[]) {
    int server_id = atoi(argv[1]);
    int total_servers = atoi(argv[2]);
    Sto::start_dist_sto(server_id, total_servers);
    myfile.open("latency.txt");

    testSimpleCountWithNPercentOfRemoteBoxes(0);
    testSimpleCountWithNPercentOfRemoteBoxes(1);
    testSimpleCountWithNPercentOfRemoteBoxes(2);
    testSimpleCountWithNPercentOfRemoteBoxes(3);
    testSimpleCountWithNPercentOfRemoteBoxes(4);
    testSimpleCountWithNPercentOfRemoteBoxes(5);
    testSimpleCountWithNPercentOfRemoteBoxes(6);
    testSimpleCountWithNPercentOfRemoteBoxes(7);
    testSimpleCountWithNPercentOfRemoteBoxes(8);
    testSimpleCountWithNPercentOfRemoteBoxes(9);

    myfile.close();
    Sto::end_dist_sto();
  
    std::cout << "Done\n";

    return 0;
}
