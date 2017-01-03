// #undef NDEBUG
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include "Transaction.hh"
#include "DistTBox.hh"
#include "StringWrapper.hh"
#include "DistSTOServer.hh"

#define TOTAL_BOXES 100 

DistTBox<int> boxes[TOTAL_BOXES];

int count_per_thread = 10000;

struct Args {
    int thread_id;
    int threads_per_server;
    int perc_of_remote_boxes;
};

// All these tests are run on 4 different servers

void* simpleCount(void *input) {
    struct Args* args = (struct Args*) input;
    int thread_id = TThread::get_global_id(args->thread_id);
    int nthreads_per_server = args->threads_per_server;
    int total_threads = nthreads_per_server * Sto::total_servers;
    int perc_of_remote_boxes = args->perc_of_remote_boxes;
    delete input;

    int64_t version = Sto::server->version();
    TThread::init(thread_id, version);

    for (int i = 0; i < TOTAL_BOXES; i++) { 
        // only thread 0 of the owner should initialize the object
        if (Sto::server->is_local_obj(&boxes[i]) && TThread::id() % 8 == 0) {
            //std::cout << "Server " << Sto::server->id() << " is the owner of box " << i <<"\n";
            TransactionGuard t;
            boxes[i] = 0;
        }
    }

    // wait till everyone is done with initialization
    Sto::server->wait(total_threads);
 
    for (int i = 0; i < count_per_thread; i++) {
        for (int j = 0; j < TOTAL_BOXES; j++) {
            TRANSACTION {
                int c_read = boxes[j];
                c_read++;
                boxes[j] = c_read;
            } RETRY(true);
        }
    }

    // wait till eveyone is done with counting 
    Sto::server->wait(total_threads);
 
    for (int i = 0; i < TOTAL_BOXES; i++) {
        TransactionGuard t;
        int c_read = boxes[i];
        if (i < ((perc_of_remote_boxes * TOTAL_BOXES * Sto::total_servers) / 100))
            assert(c_read == count_per_thread * total_threads);
        else {
            assert(c_read == count_per_thread * nthreads_per_server);
        }
    }


    // wait till everyone is done with the above transaction
    Sto::server->wait(total_threads);

    TThread::cleanup();
}


void testSimpleCountWithNPercentOfRemoteBoxes(int N) {
    
    int nthreads_per_server = 8;
    pthread_t tids[nthreads_per_server];

    for (int i = 0; i < TOTAL_BOXES; i++) {
	if (i < ((N * TOTAL_BOXES * Sto::total_servers) / 100))
            DistSTOServer::obj_owner[&boxes[i]] = i % Sto::total_servers;
        else
	    DistSTOServer::obj_owner[&boxes[i]] = Sto::server->id(); 
    }

    for (int i = 0; i < nthreads_per_server; i++) {
        struct Args* args = new struct Args();
        args->thread_id = i;
        args->threads_per_server = nthreads_per_server;
        args->perc_of_remote_boxes = N;
        pthread_create(&tids[i], NULL, simpleCount, args);
    }

    for (int i = 0; i < nthreads_per_server; i++) {
        pthread_join(tids[i], NULL);
    }
   
}

void testSimpleCountWith0PercentOfRemoteBoxes() {
   
    testSimpleCountWithNPercentOfRemoteBoxes(0);

    printf("PASS: %s\n", __FUNCTION__);
}

void testSimpleCountWith5PercentOfRemoteBoxes() {
   
    testSimpleCountWithNPercentOfRemoteBoxes(5);

    printf("PASS: %s\n", __FUNCTION__);
}

void testSimpleCountWith10PercentOfRemoteBoxes() {
   
    testSimpleCountWithNPercentOfRemoteBoxes(10);

    printf("PASS: %s\n", __FUNCTION__);
}

int main(int argc, char *argv[]) {
    int server_id = atoi(argv[1]);
    int total_servers = atoi(argv[2]);
    Sto::start_dist_sto(server_id, total_servers);
    testSimpleCountWith0PercentOfRemoteBoxes();
    Sto::end_dist_sto();
    return 0;
}
