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

// One server is dedicated as the owner of tbox
// Other servers would have to send rpc to modify tbox
void testSimpleCount() {

    // only owner should initialize the object
    if (Sto::server->is_local_obj(&c)) {
        TransactionGuard t;
        c = 0;
    }

    Sto::server->wait();

    for (int i = 0; i < 1000; i++) {
        TRANSACTION {
            int c_read = c;
            c_read++;
            c = c_read;
        } RETRY(true);
    }
  
    Sto::server->wait();

    {
        TransactionGuard t;
        int c_read = c;
        assert(c_read == 1000 * Sto::total_servers);
    }

    printf("PASS: %s\n", __FUNCTION__);
}


int main(int argc, char *argv[]) {
    int server_id = atoi(argv[1]);
    int total_servers = atoi(argv[2]);
    Sto::initialize_dist_sto(server_id, total_servers);
    testSimpleCount();
    return 0;
}
