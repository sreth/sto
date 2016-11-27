#undef NDEBUG
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include "Transaction.hh"
#include "DistTBox.hh"
#include "StringWrapper.hh"
#include "DistSTOServer.hh"

// One server is dedicated as the owner of tbox
// Other servers would have to send need rpc 
void testSimpleCount() {
    DistTBox<int> c;

    if (Sto::server->is_local_obj(&c)) {
        {
            TransactionGuard t_start;
            c = 0;
        }
    }

    Sto::server->wait();

    for (int i = 0; i < 1000; i++) {
        {
            TransactionGuard t;
            c++;
        }
    }
    
    Sto::server->wait();

    {
        TransactionGuard t_end;
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
