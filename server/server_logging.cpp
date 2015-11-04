#include "server_logging.h"
void print_to_file(const char *fn, int lnum,const char *trace_buf) {
    static FILE *fp = NULL;
    char path[64];
    if(fp == NULL) {
        snprintf(path, 64, "./server_logs");
        fp = fopen(path, "a+");
    }
    
    if (fp && debug_enable) {
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

 void debug_enable_disable(bool val) {
     if(val) {
         std::cout<<"\n Enabling DEBUG logging \n";
         debug_enable = true;
         DEBUG("DEBUG STARTED ");
     } else {
         std::cout<<"\n Disabling DEBUG logging \n";
         DEBUG("DEBUG STOPPED ");
        debug_enable = false;
     }
 }
