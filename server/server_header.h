#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<set>
#include<sys/select.h>
#include<fcntl.h>
#include<random>
#include<thread>

#include"socket_util.h"
#include"server_result.h"
#define SERVER_PORT 1234
#define MAX_CLIENTS 10

#define ERROR(format,args...) printf("ERROR %s(%d): " format "\n" ,          __FUNCTION__,__LINE__,##args)

#define DEBUG(format,args...) printf("DEBUG %s(%d): " format "\n" ,          __FUNCTION__,__LINE__,##args)

class connection {
    int sock;              //socket to use
    int connectionlist[1000];  //Array of connected sockets 
    fd_set socks;              //for select 
    int highsock;              //Highest file descriptor for select
    int no_of_active_clients;
    void setnonblocking(int sock);
    void buildselectlist();
    void handle_new_connection();
    void read_socks();
    void handle_data(int list);
    connection() { no_of_active_clients = 0; }
    static connection *cco;

     connection(connection const&){};             // copy constructor is private
     connection& operator=(connection const&){};

    public :
    friend void run_connection(connection *c);
    static connection* get_instance() {
        if(!cco) cco = new connection;
        return cco;
    }
    int get_number_of_clients();
    std::set<int> get_list();
};

void run_connection(connection *c);

struct databuf { 
    void *data;
    size_t capacity;
};

void init_buffer(int cap, databuf **d);
void filldata(databuf *dbuf);
void distribute_data(databuf *d);
void free_buffer(databuf **d);
