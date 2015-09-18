#include "server_connection.h"
#include "socket_util.h"
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
    if(connection < 0) {
        ERROR("Accept failure");
        exit(EXIT_FAILURE);
    }
    
    //set connection non blocking
    setnonblocking(connection);
    for(list = 0; list < MAX_CLIENTS && connection != -1; list++) {
        if(connectionlist[list] == 0) {
            DEBUG("Connection accepted : FD = %d; Slot=%d\n",
                connection,list);
            connectionlist[list] = connection;
            connection = -1;
        }
    }
    if(connection != -1) {
        DEBUG("No room left for new client ");
        sock_puts(connection, "Server busy Try later ");
        close(connection);
    }
}

void connection::handle_data(int list) {
    char buffer[1024]; //for read 
    
    if(sock_gets(connectionlist[list], buffer, 1024) < 0) {
        ERROR("No thing in gets");
        DEBUG("Connection lost : fd = %d; slot = %d\n",
            connectionlist[list],list);
        //free up slot
        connectionlist[list] = 0;
    } else {
        //received data 
        std::cout<<"Received "<<buffer;
        
        //catenate received and send back
        strncat(buffer," : Received \n",1024);

        sock_puts(connectionlist[list],buffer);
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

void connection::run_connection() {
    int readsocks; //no of sockets ready for reading 
    timeval timeout;

    sockaddr_in server_address; //for bind
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        ERROR("Not able to create socket");
        exit(EXIT_FAILURE);
    }

    //enable reuse
    int reuseaddr = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    //set socket as non blocking 
    setnonblocking(sock);

    //binding
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = SERVER_PORT;
    if(bind(sock, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        ERROR("Error in Binding");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    //Setup queue
    listen(sock,MAX_CLIENTS);

    //as we start with one socket init it as higsocket for select
    highsock = sock;
    memset((char *) &connectionlist, 0, sizeof(connectionlist));

    //Loop forever 
    while(1) {
        buildselectlist();
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        
        readsocks = select(highsock+1, &socks, (fd_set *) 0, (fd_set *) 0,
                             &timeout);

        if(readsocks < 0) {
            ERROR("Select function failure");
            exit(EXIT_FAILURE);
        }

        if(readsocks == 0) {
            //no new messages print .
            std::cout<<".";
        } else {
            read_socks();
        }
    }
}


