#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LOG_MEM_SIZE 10000000

#define ERROR(format,args...) printf("ERROR %s(%d): " format "\n" ,          __FUNCTION__,__LINE__,##args)

static char log_buf[1024];
#define DEBUG(__args...)                        \
    do {                                        \
        memset(log_buf,0,sizeof(log_buf));      \
        snprintf (log_buf,                      \
                sizeof(log_buf), __args);       \
        print_to_file (                         \
                __FUNCTION__, __LINE__,log_buf);\
    } while (0)


void print_to_file(const char *fn, int lnum,const char *trace_buf);
