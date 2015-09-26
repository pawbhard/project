
#include "server_header.h"
#include "database_def.h"
connection* connection::cco = NULL;
Result *Result::result = NULL;
DB *DB::db = NULL;
#define CAPACITY 10
int main() {

    connection *conn = connection::get_instance() ;
    std::thread con(run_connection,conn);
    databuf *d;
    init_buffer(CAPACITY,&d);
    filldata(d);
    return 0;
}
