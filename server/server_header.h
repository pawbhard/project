#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<set>
#include<sys/select.h>
#include<fcntl.h>
#include<random>
#include<thread>
#include<assert.h>
#include<mutex>
#include<atomic>
#include<queue>
#include<memory>
#include<mutex>

#include"socket_util.h"
#include"server_result.h"
#include "server_constants.h"
#include "server_config.h"
#include "server_tracker.h"
#include "timer.h"
#include "server_queue.h"

#define SERVER_PORT 1234
#define MAX_CLIENTS get_max_clients()
#define CAPACITY get_capacity()
class connection {
    int sock;              //socket to use
    int connectionlist[1000];  //Array of connected sockets 
    bool connection_free[1000]; //true means free
    fd_set socks;              //for select 
    int highsock;              //Highest file descriptor for select
    int no_of_active_clients;
    void setnonblocking(int sock);
    void buildselectlist();
    void handle_new_connection();
    void read_socks();
    void handle_data(int list);
    connection() { 
        no_of_active_clients = 0; 
        for(int i = 0; i < 1000; i++)
            connection_free[i] = true;
    }
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
    int sw_id;
    int refcnt;
};

void init_buffer(int cap, databuf **d,int sw_id);
void filldata(databuf *dbuf,int sw_id);
void distribute_data(void *arg);
void free_buffer(databuf **d);
void handle_results(int client_id, int *buffer, float *element);
void handle_join(int client_id, int *buffer);
void handle_timer(sigval);
