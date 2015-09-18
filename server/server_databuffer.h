
#include<random>
#include<thread>
#include"server_connection.h"
#include"socket_util.h"
struct databuf { 
    void *data;
    size_t capacity;
};

void init_buffer(int cap, databuf *d);
void filldata(databuf *dbuf);
void distribute_data(databuf *d);
void free_buffer(databuf *d);


