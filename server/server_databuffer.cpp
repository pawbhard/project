#include "server_header.h"
#include "threadpool.h"

using namespace std;
void init_buffer(int cap, databuf **d) {
    (*d) = new databuf;
    if(!*d) {
        std::cout<<"Malloc failed ";
        exit(EXIT_FAILURE);
    }
    (*d)->capacity = cap;
    (*d)->data = new int[cap];
    if((*d)->data == NULL) {
        std::cout<<"Malloc failed";
        exit(EXIT_FAILURE);
    }
}

void filldata(databuf *dbuf) {

    //Create thread pool for distrubute 
    thread_pool t;

    int random_integer,minn =  20,maxx = 70;
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
        ar = (int *) dbuf->data;
        sleep(1);
        random_integer = uni(rng);
        DEBUG("Got temprature from snmp %d",random_integer);
        ar[pos] = random_integer;
        pos++;
        if(pos == dbuf->capacity) {
            //buffer is full send this to some one and create new 
            databuf *temp = dbuf;
            dbuf = NULL;
            init_buffer(temp->capacity, &dbuf);
            pos = 0;
            t.submit(distribute_data,(void *) temp);
           /*si
           //call function in separate thread 
            std::thread distribute(distribute_data, temp);
            //detach the thread
            distribute.detach();
            */
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

void distribute_data(void *arg)
{

    databuf *d = (databuf *) arg;
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
    int size_data,put;
    int *arr = NULL;
    int *data = (int *) d->data;
    int cap = d->capacity;
    int per_client = ceil((float)cap/(float)no_of_clients);
    set<int>::iterator it;
    for(it = client_list.begin() ; it != client_list.end(); ++it)
    {
        if(cap <= 0) break;
        size_data = cap > per_client ? per_client : cap;
        arr = new int[size_data+1];//in first sending no of element
        if(arr == NULL) {
            std::cout<<"MAlloc failed";
            exit(EXIT_FAILURE);
        }
        arr[0] = size_data;
        memcpy(arr+1,data+(d->capacity - cap) , size_data*sizeof(int));
        put = sock_puts(*it, (void *) arr, (size_data+1)*sizeof(int));
        DEBUG("Sending %d data to %d",put,*it);
        cap -= size_data;
        free(arr);
        arr = NULL;
    }
    DEBUG("Send complete");
    free_buffer(&d);
}





