//Main file
#include"client_header.h"



int main(int argc, char *argv[])
{
    char buffer[1024];
    //Message format 
    int buf[3]; float *elements;
    char send[1024];

    int sock ,n;
    struct sockaddr_in address;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 1234;

    //connect
    connect(sock, (struct sockaddr *) &address, sizeof(address));
    int group_id;
    memset(buffer,0,sizeof(buffer));
    int rc = read(sock,buffer,1024);
    std::cout<<"received bytes "<<rc<<"\n";
    std::cout<<"Message Received : " << buffer<<"\n";
    memset(buffer,0,sizeof(buffer));
    std::cout<<"Enter Group to join ";
    std::cin>>group_id;
    //send join request
    buf[0] = 0;
    buf[1] = group_id; //for MEAN
    buf[2] = 0;
    std::cout<<"Joined group "<<group_id<<"\n";
    //strcpy(buffer,"Mean");
    int wr = write(sock,buf,sizeof(buf));
    std::cout<<"Written bytes "<<wr<<"\n";
     //start receiver
    std::thread rec(receiver,sock);
    rec.join();
}

