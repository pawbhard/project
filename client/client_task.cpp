#include "client_header.h"

int  cal_range(std::vector<float> vec, float *max,float *min) {
    std::vector<float>::iterator it;
    *min = INT_MAX;
    *max = INT_MIN;
    for (it=vec.begin(); it<vec.end(); it++) {
        if(*min > *it) *min = *it;
        if(*max < *it) *max = *it;
    }
    std::cout<<"Doing range "<<*max<<" "<<*min<<" \n";
//    sleep(1);
    return 1;
}
float cal_mean(std::vector<float> vec) {
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
    int send_ar[] = { task_id, task, 2 };
    //float send_ar2[] = { (float) vec.size(), result };
    float send_ar2[2]; 

    if(task == MEAN) {
        send_ar2[0] = (float) vec.size();
        send_ar2[1] = result;
    } else if (task == RANGE) {
        send_ar2[0] = min;
        send_ar2[1] = max;
    }
    std::cout<<"\nSending array " << send_ar[0] <<" "<<send_ar[1] <<" "
            <<send_ar[2] <<" size is "<<sizeof(send_ar)<<"\n";
    std::cout<<"Sending result "<<send_ar2[0]<<" "<<send_ar2[1]<<"\n";
    char buffer[1024];
    memcpy(buffer,send_ar,sizeof(send_ar));
    memcpy(buffer+sizeof(send_ar),send_ar2,sizeof(send_ar2));

    int wr = send(sock, (void *) buffer, sizeof(send_ar)+sizeof(send_ar2) , 0);
    std::cout<<"Bytes sent is "<<wr<< "\n";
}

