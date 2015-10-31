#include "server_header.h"
#include "threadpool.h"
#include "database_def.h"

using namespace std;
static int task_id = 1;
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
    float *ar = (float *) dbuf->data;
    int pos = 0;
    while(1) {
        ar = (float *) dbuf->data;
        usleep(1);
        random_integer = uni(rng);
        //DEBUG("Got temprature from snmp %d",random_integer);
        ar[pos] = (float) random_integer;
        pos++;
        if(pos == dbuf->capacity) {
            //buffer is full send this to some one and create new 
            databuf *temp = dbuf;
            dbuf = NULL;
            init_buffer(temp->capacity, &dbuf);
            pos = 0;
            t.submit(distribute_data,(void *) temp);
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
    //Steps 
    // 1. get all task iterate on them 
    // 2. for each get one free group 
    // 3. get all clients for that group 
    // 4. construct and send data to client
    // 5. track data sent 
    // 6. start a timer for task_id
    
    DB *db = DB::get_instance();
    track_data *td = track_data::get_instance();

    int i , opcode;
    for ( i = 0; i < MAX_TASK ; i++ ) {
        switch(i) {
            case MEAN : opcode = MEAN ;
                        break;
            case RANGE : opcode = RANGE;
                         break;
            default :   ERROR("Unkown opcode error");
                        assert(0);
        }
        int group_id = db->get_free_group(opcode);
        set<int> client_list = db->get_client_list(group_id);
        if(client_list.size() == 0) {
            DEBUG("No clients free ignoring data for opcode %d",opcode);
            continue;
            //free_buffer(&d);
            //return;
        }
        db->set_state(group_id, false);

        int size_data,put;
        int no_of_clients = client_list.size();
        int buf[3];
        buf[0] = task_id; task_id++;
        buf[1] = i; //opcode from loop 
        float *arr = NULL;
        float *data = (float *) d->data;
        int cap = d->capacity;
        int per_client = ceil((float)cap/(float)no_of_clients);
        set<int>::iterator it;

        // Storing mapping of task_id and group_id with buffer pointer
        td->set_group_task_map (task_id, group_id, arg);

        timer *t1;
        t1 = new timer(task_id, 100, handle_timer);
        t1->start();
       
        for(it = client_list.begin() ; it != client_list.end(); ++it)
        {
            if(cap <= 0) break;
            size_data = cap > per_client ? per_client : cap;
            buf[2] = size_data; //set third parameter
            arr = new float[size_data];//in first sending no of element
            if(arr == NULL) {
                std::cout<<"MAlloc failed";
                exit(EXIT_FAILURE);
            }

            //Tracking of data
            td->set_track(task_id-1, *it, (d->capacity - cap), ((d->capacity - cap)+ size_data));

            //copy and send data 
            memcpy(arr,data+(d->capacity - cap) , size_data*sizeof(int));
            //copy both 
            char buffer[1000]; //to send data
            memcpy(buffer,buf,sizeof(int)*3);
            memcpy(buffer+sizeof(int)*3,arr,sizeof(float)*size_data);
            int total_size = sizeof(int) * 3 + sizeof(float)*size_data;
            put = sock_puts(*it, (void *) buffer, total_size);
            assert(put == total_size);
            DEBUG("Sending %d data to %d",put,*it);
            cap -= size_data;
            free(arr);
            arr = NULL;
        }
        DEBUG("Send complete");

    }
#if 0
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
#endif
    //free_buffer(&d);
}

void handle_timer(sigval s)
{
  track_data *td = track_data::get_instance();
//   int client_id;
//   int start;
//   int end;
//   int ret_val;
//   int *arr;
//   int put;

   DEBUG ("Timer fired for task id : %d", s.sival_int);

   list<int> cl = td->get_clients_from_track(s.sival_int);

   DEBUG("Clients in the list");
   for(list<int>::iterator it = cl.begin(); it != cl.end(); ++it) {
   DEBUG("*it");

   td->release_group(s.sival_int);
   }
// TODO : Send back data once again to the client in list cl

}




