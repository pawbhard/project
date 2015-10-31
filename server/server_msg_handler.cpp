#include "server_header.h"
#include "database_def.h"
void handle_join(int client_id, int *buffer) {
    //buffer[0] == 0         Taskid should be 0 
    //buffer[1] == opcode    Task client want to register  
    //buffer[2] == 0         No element further //can be used for other signalling
    DEBUG("Handling new connection client_d %d opcode %d",client_id,buffer[1]);
    if(buffer[0] != 0 || buffer[2] != 0 ) assert(0);
    int opcode = buffer[1];
    DB *d = DB::get_instance();
    d->add_new_client(client_id,opcode);

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
    td->delete_track (buffer[0], client_id);

    DEBUG("handling results %f %f",elements[0],elements[1]);
    DB *db = DB::get_instance();
//    db->set_state(1,true);
    int opcode = buffer[1];
    Result *r = Result::get_instance();
    if(opcode == MEAN)
        r->update_mean((int)elements[0] , elements[1]);
    else if(opcode == RANGE)
        r->update_range(elements[0], elements[1]);
    else {
        DEBUG("Unkown result encountered ");
        //assert(0);
    }
}

