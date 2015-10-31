#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LOG_MEM_SIZE 10000000
static char log_buf[1024];
#define DEBUG(__args...)                        \
    do {                                        \
        memset(log_buf,0,sizeof(log_buf));      \
        snprintf (log_buf,                      \
                sizeof(log_buf), __args);       \
        print_to_file (                    \
                __FUNCTION__, __LINE__,log_buf);\
    } while (0)


void print_to_file(const char *fn, int lnum,const char *trace_buf) {
    static FILE *fp = NULL;
    char path[64];
    if(fp == NULL) {
        snprintf(path, 64, "./server_logs");
        fp = fopen(path, "a+");
    }
    
    if (fp) {
        fprintf(fp, "[%24s::%4d] : %s", fn, lnum, trace_buf);
        fprintf(fp, "\n");
        fflush(fp);
        int file_size = ftell(fp);
        if (file_size > MAX_LOG_MEM_SIZE) {
            fclose(fp);
            fp = NULL;
        }
    }
}

int main() { 
    int i = 10;
    DEBUG("Hello man");
    DEBUG("Second %d ",i);
}
