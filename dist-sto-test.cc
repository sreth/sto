#undef NDEBUG
#include "Transaction.hh"
#include "DistSTOServer.hh"

#include <chrono>

int data_size[] = {0, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};

using namespace std::chrono;

// Measure average execution time of transmit with different data sizes
void*  measure_latency_with_diff_data_sizes(void *) {
    int num_tries = 100;
    int64_t version = Sto::server->version();
    TThread::init(0, version);
    Sto::server->wait(2);
    if (Sto::server->id()) {
        int size = sizeof(data_size) / sizeof(data_size[0]);
    	for (int i = 0; i < size; i++) {
            std::string data(data_size[i], 'a');
            // time the average execution time of transmit
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            for (int j = 0; j < num_tries; j++) {
                TThread::client(0)->transmit(data);
            }
	    high_resolution_clock::time_point t2 = high_resolution_clock::now();
	    auto duration = duration_cast<microseconds>( t2 - t1 ).count() / num_tries;
            std::cout << "Data of length " << data_size[i] << " bytes takes " << duration << " microseconds\n";
        }
    }
    Sto::server->wait(2);
    TThread::cleanup();
}

void measureLatency(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, measure_latency_with_diff_data_sizes, NULL);
    pthread_join(tid, NULL);
    printf("Finish %s\n", __FUNCTION__);
}

/*
void* measure_throughput_with_diff_perc_of_remote_objs(void *) {
    if (Sto::server->id()) {
    }
}

void measureThroughput(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, measure_throughput_with_diff_perc_of_remote_objs, NULL);
    pthread_join(tid, NULL);
    printf("Finish %s\n", __FUNCTION__);
}
*/

int main(int argc, char *argv[]) {
    int server_id = atoi(argv[1]);
    int total_servers = atoi(argv[2]);
    Sto::start_dist_sto(server_id, total_servers);
    measureLatency();
    // measureThroughput();
    Sto::end_dist_sto();
    return 0;
}
