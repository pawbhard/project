#include "server_header.h"
#include "database_def.h"
void connection::setnonblocking(int sock) {
    DEBUG("Setting nonblocking for socket %d",sock);
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0) {
        ERROR("fcntl(F_GETFL) failed");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sock, F_SETFL, opts) < 0) {
        ERROR("fcntl(F_SETFL) failed ");
        exit(EXIT_FAILURE);
    }
    return;
}

void connection::buildselectlist() {
    FD_ZERO(&socks);
    FD_SET(sock, &socks); //add to socket fd 

    int list;
    for(list = 0; list < MAX_CLIENTS; list++) {
        if(connectionlist[list] != 0) {
            FD_SET(connectionlist[list],&socks);
            if(connectionlist[list] > highsock)
                highsock = connectionlist[list];
        }
    }
}

void connection::handle_new_connection() {
    int list;
    int connection;  // fd for incoming connection
    connection = accept(sock,NULL,NULL);
    DEBUG("Handshake init");
    char buf[100] = " Available : MEAN (100)  RANGE (200)  .. ";
   send(connection,buf,strlen(buf),0);
   /* memset(buf,0,sizeof(buf));
    read(connection,buf,100); */
    DEBUG("Group joined : Mean");
   /* if(connection < 0) {
        ERROR("Accept failure");
        exit(EXIT_FAILURE);
    }
    */
    //set connection non blocking
    setnonblocking(connection);
    for(list = 0; list < MAX_CLIENTS && connection != -1; list++) {
        if(connectionlist[list] == 0) {
            DEBUG("Connection accepted : FD = %d; Slot=%d\n",
                connection,list);
            connectionlist[list] = connection;
            connection = -1;
            no_of_active_clients++;
        }
    }
    if(connection != -1) {
        DEBUG("No room left for new client ");
        char buf[] = "Server busy Try later ";
        sock_puts(connection, (void *)buf ,strlen(buf));
        close(connection);
    }
}

void connection::handle_data(int list) {
    int buffer_int[3];
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    int r = read(connectionlist[list], (void *)buffer , sizeof(buffer));
    DEBUG("No of read bytes is %d",r);
      if(r <= 0) { 
        ERROR("No thing in gets");
        DEBUG("Connection lost : fd = %d; slot = %d\n",
            connectionlist[list],list);
        //free up slot
        connection_free[list] = true;
        //delete client from database 
        DB *dd = DB::get_instance();
        dd->delete_client(connectionlist[list]);
        connectionlist[list] = 0;
    } else  {
        memset(buffer_int,0,sizeof(buffer_int));
        memcpy(buffer_int,buffer,sizeof(buffer_int));
        std::cout<<"Received : ";
        std::cout<<buffer_int[0]<<" task_id \n"
                 <<buffer_int[1]<<" opcode \n"
                 <<buffer_int[2]<<" No of elements "
                 <<"\n";
        
       //update
       if(buffer_int[0]) {
           float *element;
           element = new float[buffer_int[2]] ;
           memset(element,0,sizeof(float)*buffer_int[2]);
           memcpy(element, buffer+3*sizeof(int) ,sizeof(float)*buffer_int[2]); 
           handle_results(connectionlist[list],buffer_int, element);
       } else {
           handle_join(connectionlist[list],buffer_int);
       }
    }
}

void connection::read_socks() {

    int list; //for loop

    if(FD_ISSET(sock,&socks))
        handle_new_connection();

    //run through all socket and check to see if some new thing is there 
    
    for(list = 0; list < MAX_CLIENTS ; list++) {
        if(FD_ISSET(connectionlist[list], &socks))
            handle_data(list);
    }
}

void run_connection(connection *c) {
    int readsocks; //no of sockets ready for reading 
    timeval timeout;

    sockaddr_in server_address; //for bind
    c->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(c->sock < 0) {
        ERROR("Not able to create socket");
        exit(EXIT_FAILURE);
    }

    //enable reuse
    int reuseaddr = 1;
    setsockopt(c->sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    //set socket as non blocking 
    c->setnonblocking(c->sock);

    //binding
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = SERVER_PORT;
    if(::bind(c->sock, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        ERROR("Error in Binding");
        close(c->sock);
        exit(EXIT_FAILURE);
    }
    
    //Setup queue
    listen(c->sock,MAX_CLIENTS);

    //as we start with one socket init it as higsocket for select
    c->highsock = c->sock;
    memset((char *) &c->connectionlist, 0, sizeof(c->connectionlist));

    //Loop forever 
    while(1) {
        c->buildselectlist();
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        
        readsocks = select(c->highsock+1, &c->socks, (fd_set *) 0, (fd_set *) 0,
                             &timeout);

        if(readsocks < 0) {
            ERROR("Select function failure");
            exit(EXIT_FAILURE);
        }

        if(readsocks == 0) {
            //no new messages print .
            //std::cout<<".";
            ;
        } else {
            c->read_socks();
        }
    }
}

int connection::get_number_of_clients() {
    return no_of_active_clients;
}
std::set<int> connection::get_list() {
    std::set<int> clients;
    int list;
    DEBUG("Getting list");
    for(list = 0; list < MAX_CLIENTS ; list++) {
        if(connectionlist[list] != 0) {
            clients.insert(connectionlist[list]);
        }
    }
    return clients;
}
