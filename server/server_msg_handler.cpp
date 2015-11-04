#include "server_header.h"
#include "database_def.h"
int handle_join(int client_id, int *buffer) {
    //buffer[0] == 0         Taskid should be 0 
    //buffer[1] == opcode    Task client want to register  
    //buffer[2] == 0         No element further //can be used for other signalling
    DEBUG("Handling new connection client_d %d opcode %d",client_id,buffer[1]);
    if(buffer[0] != 0 || buffer[2] != 0 ) assert(0);
    int opcode = buffer[1];
    if(opcode >= NUM_OF_OPCODES || opcode < 0) {
        DEBUG("Invalid opcode removing client");
        char buf[] = "Invalid code ";
        sock_puts(client_id, (void *)buf ,strlen(buf));                                                          
        return FAILURE;
    } else {
        char buf[] = "Join Sucessful";
        sock_puts(client_id, (void *)buf ,strlen(buf));
    }
    DB *d = DB::get_instance();
    d->add_new_client(client_id,opcode);
    return SUCCESS;
}

void handle_results(int client_id, int *buffer, float *elements) {
    //buffer[0]                 task id
    //buffer[1]                 opcode
    //buffer[2]                 Number of element

    //Steps 
    // 1. delete tracking info of this request
    // 2. Update results according to opcode

    track_data *td = track_data::get_instance();
    //delete tracking
    int task_id = buffer[0]>>2;
    int sw_id = buffer[0] & 0x3;
    td->delete_track (task_id, client_id);

    DEBUG("handling results Switch id %d %f %f",sw_id,elements[0],elements[1]);
    DB *db = DB::get_instance();
//    db->set_state(1,true);
    int opcode = buffer[1];
    Result *r = Result::get_instance();
    if(opcode == MEAN)
        r->update_mean((int)elements[0] , elements[1], sw_id /* switch id */);
    else if(opcode == RANGE)
        r->update_range(elements[0], elements[1], sw_id /* switch id */);
    else {
        DEBUG("Unkown result encountered ");
        //assert(0);
    }
}

