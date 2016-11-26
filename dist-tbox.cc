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
// Other servers would have to send 
void testSimpleCount() {
    DistTBox<int> c;

    {
        TransactionGuard t1;
        c = 100;
    }

    {
        TransactionGuard t2;
        int c_read = c;
        assert(c_read == 100);
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
