#include "server_logging.h"
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
