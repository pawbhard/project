#include <cstdio>
#include <cstring>   //strlen
#include <cstdlib>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<iostream>
#include<thread>

#define TRUE   1
#define FALSE  0
#define PORT 1234

void receiver(int sock);
int  task_perform(int *ar, int N, int task);
int  cal_mean(int *ar, int N);

void receiver(int sock) {
    int rec,N;
    int ar[10000]; //capacity to process
    while(1) {
        memset(ar,0,sizeof(ar));
        rec = recvfrom(sock, (void *) ar, sizeof(ar) ,0,NULL,NULL);
        if(rec > 0) {
            int N = ar[0]; //Get number of element from start;
            int i;
            std::cout<<"Received array ";
            for(int i = 1; i <= N; i++)
                std::cout<<ar[i]<<" , ";
            std::cout<<"\n";
        }
    }
}
int main(int argc, char *argv[])
{
    char sendline[1000];
    char recvline[1000];

    //test int array
    int ar[20];
    memset(ar,0,sizeof(ar));
    
    char buffer[1025];
    int sock ,n;
    struct sockaddr_in address;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 1234;

    connect(sock, (struct sockaddr *) &address, sizeof(address));
    std::thread rec(receiver,sock);
    rec.join();
}

