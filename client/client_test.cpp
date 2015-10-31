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
#include<unistd.h>
#include<limits.h>

#define TRUE   1
#define FALSE  0
#define PORT 1234

//define tasks
#define MEAN 0
#define RANGE 1

void receiver(int sock);
void  task_perform(int sock, std::vector<float> vec, int task, int id);
int  cal_mean(std::vector<float> vec);
int  cal_range(std::vector<float> vec, float *max,float *min);
int  cal_range(std::vector<float> vec, float *max,float *min) { 
    std::vector<float>::iterator it;
    *min = INT_MAX;
    *max = INT_MIN;
    for (it=vec.begin(); it<vec.end(); it++) {
        if(*min > *it) *min = *it;
        if(*max < *it) *max = *it;
	sleep(10);
    }
    std::cout<<"Doing range "<<*max<<" "<<*min<<" \n"; 
//    sleep(1);
    return 1;
}
int cal_mean(std::vector<float> vec) {
    std::vector<float>::iterator it;
    float result = 0;
    for (it=vec.begin(); it<vec.end(); it++)
        result += *it;
    std::cout<<"Mean calculated "<<result/vec.size()<<"\n";
 //   sleep(1);
    return result/vec.size();
}

void task_perform(int sock, std::vector<float> vec, int task,int id) {
    //perform task and send result back
   /* std::cout << "myvector contains:";
    std::vector<int>::iterator it;
    for (it=vec.begin(); it<vec.end(); it++)
        std::cout << ' ' << *it;
    std::cout << '\n';*/
    float min,max,result = 0;
    switch (task) {
        case MEAN: result = cal_mean(vec);
                   break;
        case RANGE: cal_range(vec,&min,&max);  
                   break;
        default : std::cout<<"Unknown task\n";
    }

    //Now we have result 
    //send data back to server
    //format [ task_id opcode Number] [Number_mean Mean]
    int task_id = id;
    int send_ar[] = { task_id, RANGE, 2 };
    //float send_ar2[] = { (float) vec.size(), result };
    float send_ar2[]  =  { min, max };
    std::cout<<"\nSending array " << send_ar[0] <<" "<<send_ar[1] <<" "
            <<send_ar[2] <<" size is "<<sizeof(send_ar)<<"\n";
    std::cout<<"Sending result "<<send_ar2[0]<<" "<<send_ar2[1]<<"\n";
    char buffer[1024];
    memcpy(buffer,send_ar,sizeof(send_ar));
    memcpy(buffer+sizeof(send_ar),send_ar2,sizeof(send_ar2));

    int wr = send(sock, (void *) buffer, sizeof(send_ar)+sizeof(send_ar2) , 0);
    std::cout<<"Bytes sent is "<<wr<< "\n";
}
void receiver(int sock) {
    int rec,N;
    int buf[3]; 
    float elements[200];//MAXIMUM capacity of 1 client
    char buffer[2000];
    while(1) {
        memset(buffer,0,sizeof(buffer));
        rec = recvfrom(sock, (void *) buffer, sizeof(buffer) ,0,NULL,NULL);
        std::cout<<"received bytes "<<rec <<"\n";
        if(rec > 12) {    //minimum 3 integer
            memcpy(buf,buffer,sizeof(buf));
            std::cout<<"task_id :"<<buf[0]
                <<"opcode  :"<<buf[1]
                <<"Number  :"<<buf[2]<<"\n";

            memcpy(elements,buffer+sizeof(buf),buf[2]*sizeof(float));
            int i;
            std::cout<<"Received array ";
            for(int i = 1; i <= buf[2]; i++)
                std::cout<<elements[i]<<" , ";
            std::cout<<"\n";

            std::cout<<"Sending to calculate mean \n";
            //create a vector and pass it by value
            std::vector<float> vec;
            vec.insert(vec.begin(),elements,elements+buf[2]);
            std::thread t (task_perform,sock,vec,RANGE,buf[0]);
            t.join();
        } else if(rec <= 0) { 
            std::cout<<"No server exitting ,.....\n";
            return;
        }
    }
}
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

