#include "server_header.h"
using namespace std;
void init_buffer(int cap, databuf **d) {
    (*d) = new databuf;
    (*d)->capacity = cap;
    (*d)->data = new int[cap];
}

void filldata(databuf *dbuf) {
    
    int random_integer,minn =  10,maxx = 10000;
    //For now generate random integer
    std::random_device rd;     
    // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    
    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(minn,maxx); 
    // guaranteed unbiased
    DEBUG("In fill data capacity %d",dbuf->capacity);
    int *ar = (int *) dbuf->data;
    int pos = 0;
    while(1) {
        sleep(1);
        random_integer = uni(rng);
        DEBUG("Integer generated %d",random_integer);
        ar[pos] = random_integer;
        pos++;
        if(pos == dbuf->capacity -1) {
            //buffer is full send this to some one and create new 
            databuf *temp = dbuf;
            dbuf = NULL;
            init_buffer(temp->capacity, &dbuf);
            pos = 0;
            //call function in separate thread 
            std::thread distribute(distribute_data, temp);
            //detach the thread
            distribute.detach();
        }
    }
}

void free_buffer(databuf **d) {
    if((*d)!= NULL) {
        if((*d)->data != NULL) {
            free((*d)->data);
        }
        free(*d);
        *d = NULL;
        }
}

void distribute_data(databuf *d)
{
    //1. find no of clients 
    //2. divide capacity and create limits 
    //3. iterate and send data to clients 
    //4. free databuf 

    connection *c = connection::get_instance();
    int no_of_clients = c->get_number_of_clients();
    set<int> client_list = c->get_list();
    if(client_list.size() == 0) {
        DEBUG("No clients ignoring data");
        free_buffer(&d);
        return;
    }
    int size_data;
    int *arr;
    int *data = (int *) d->data;
    int cap = d->capacity;
    int per_client = ceil((float)cap/(float)no_of_clients);
    set<int>::iterator it;
    for(it = client_list.begin() ; it != client_list.end(); ++it)
    {
        if(cap <= 0) break;
        size_data = cap > per_client ? per_client : cap;
        arr = new int[size_data];
        memcpy(arr,data+(d->capacity - cap) , size_data*sizeof(int));
        sock_puts(*it, (void *) arr, size_data*sizeof(int));
        DEBUG("Sending data to %d",*it);
        cap -= size_data;
    }
    DEBUG("Send complete");
    free_buffer(&d);
}





