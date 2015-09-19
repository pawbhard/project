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
#include<vector>

#define TRUE   1
#define FALSE  0
#define PORT 1234

//define tasks
#define MEAN 0

void receiver(int sock);
void  task_perform(int sock, std::vector<int> vec, int task);
int  cal_mean(std::vector<int> vec);

int cal_mean(std::vector<int> vec) {
    std::vector<int>::iterator it;
    int result = 0;
    for (it=vec.begin(); it<vec.end(); it++)
        result += *it;
    return result/vec.size();
}

void task_perform(int sock, std::vector<int> vec, int task) {
    //perform task and send result back
   /* std::cout << "myvector contains:";
    std::vector<int>::iterator it;
    for (it=vec.begin(); it<vec.end(); it++)
        std::cout << ' ' << *it;
    std::cout << '\n';*/
    int result = 0;
    switch (task) {
        case MEAN: result = cal_mean(vec);
                   break;

        default : std::cout<<"Unknown task";
    }

    //Now we have result 
    //send data back to server
    //format [ TASK, No_of_ele, result ]
    int send_ar[] = { task, (signed) vec.size(), result };
    send(sock, (void *) send_ar, sizeof(send_ar) , 0);
}
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

            std::cout<<"Sending to calculate mean \n";
            //create a vector and pass it by value
            std::vector<int> vec;
            vec.insert(vec.begin(),ar+1,ar+N+1);
            std::thread t (task_perform,sock,vec,MEAN);
            t.detach();
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

