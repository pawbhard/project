#include"client_header.h"

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
            std::cout<<"\ntask_id :"<<buf[0]
                <<"opcode  :"<<buf[1]
                <<"Number  :"<<buf[2]<<"\n";

            memcpy(elements,buffer+sizeof(buf),buf[2]*sizeof(float));
            int i;
            std::cout<<"Received array ";
            for(int i = 0; i <= buf[2]; i++)
                std::cout<<elements[i]<<" , ";
            std::cout<<"\n";

            std::cout<<"Sending to perform task \n";
            //create a vector and pass it by value
            std::vector<float> vec;
            vec.insert(vec.begin(),elements,elements+buf[2]);
            std::thread t (task_perform,sock,vec,buf[1],buf[0]);
            t.join();
        } else if(rec <= 0) {
            std::cout<<"No server exitting ,.....\n";
            return;
        }
    }
}
