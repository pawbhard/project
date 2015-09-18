#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE   1
#define FALSE  0
#define PORT 1234


int main(int argc, char *argv[])
{
    char sendline[1000];
    char recvline[1000];

    char buffer[1025];
    int sock ,n;
    struct sockaddr_in address;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 1234;

    connect(sock, (struct sockaddr *) &address, sizeof(address));
    while (fgets(sendline,1000,stdin) != 0) {
        sendto(sock,sendline,strlen(sendline) ,0,
                (struct sockaddr *)&address,sizeof(address));
        n=recvfrom(sock,recvline,1000,0,NULL,NULL);
        recvline[n]=0;
        fputs(recvline,stdout);
    }
}

