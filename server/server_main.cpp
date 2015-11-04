
#include "server_header.h"
#include "database_def.h"

#define CS 2

void run_shell(void);
connection* connection::cco = NULL;
Result *Result::result = NULL;
DB *DB::db = NULL;
track_data *track_data::td = NULL;
Consume *Consume::cs = NULL;

int main() { 
    std::vector<std::thread *> filpool;
    connection *conn = connection::get_instance() ;
    std::thread con(run_connection,conn);
    Consume *co = Consume::get_instance(); 
    co->spawn();
    for(int i = 0 ; i < CS ; i++) {
        filpool.push_back(new std::thread(filldata,i)); //i is sw_id
    }
    //std::thread fil(filldata,d);
    run_shell();

    for (int i = 0; i < CS; ++i){
        filpool[i]->join();
        delete filpool[i];
    }

    return 0;
}
