//Header file
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
float  cal_mean(std::vector<float> vec);
int  cal_range(std::vector<float> vec, float *max,float *min);


