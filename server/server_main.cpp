
#include "server_connection.h"
#include "socket_util.h"

int main() {

    connection conn = connection::get_instance() ;
    conn.run_connection();
    return 0;
}
