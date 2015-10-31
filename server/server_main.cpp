
#include "server_header.h"
#include "database_def.h"

void run_shell(void);

connection* connection::cco = NULL;
Result *Result::result = NULL;
DB *DB::db = NULL;
#define CAPACITY get_capacity()


int main() {

    connection *conn = connection::get_instance() ;
    std::thread con(run_connection,conn);
    databuf *d;
    init_buffer(CAPACITY,&d);
    
    std::thread fil(filldata,d);
    run_shell();
    fil.join();
    return 0;
}
