#include "server_header.h"
#include "database_def.h"
//#include <list.h>
//using std::make_pair;
 
int track_data::set_track(const int task_id, int client_id, int s, int e) {
    //TODO : add client_list in header file
//    list <position> client_list = track[task_id];
    DEBUG("Inside set_track with task_id : %d, client_id : %d", task_id, client_id);
    client_list = track[task_id];
    //TO-DO deep dive for 'position' structure 
    position p;
    p.client_id = client_id;
    p.start = s;
    p.end = e;
    client_list.push_back(p);
    track[task_id] = client_list;
    return SUCCESS;
}
 
// to delete all client-id in one shot  
int track_data::delete_all_track(int task_id) {
    DEBUG("Inside track_data");
    unordered_map<int, list <position> >::iterator it = track.find(task_id);
    if(it == track.end()) {
        DEBUG("Task not found");
        return FAILURE;
    }
    track.erase(it);
    return SUCCESS;
}  

// to delete a particular client-id
int track_data::delete_track(int task_id, int client_id) {
    DEBUG("Inside delete_track with task_id : %d, client_id : %d", task_id, client_id);
    unordered_map<int, list <position> >::iterator it = track.find(task_id);
    if(it == track.end()) {
        DEBUG("Task not found");
        return FAILURE;
    }
    list <position> client_list = track[task_id];
    position p;
    for (list<position>::iterator l_it=client_list.begin(); l_it != client_list.end(); ++l_it) {
        p = *l_it;
        if(p.client_id == client_id) {
           DEBUG("client_id found, deleting"); 
           client_list.erase(l_it);
	   break;
         }        
    }
    if (client_list.size() == 0)
    {
       DEBUG("client list size zero, calling releasing group with task_id %d", task_id);
       release_group(task_id);
    }
    track[task_id] = client_list;
    return SUCCESS;
} 

list<int> track_data::get_clients_from_track(int task_id) {
        DEBUG("Inside get_clients_from_track");
	list <int> cl ;
        list <position> lp = track[task_id];
        position p;
	 for (list<position>::iterator lp_it = lp.begin() ; lp_it != lp.end() ; ++lp_it) {
		p = *lp_it;
                cl.push_back(p.client_id);
         }
       return cl;
       
}

int track_data::set_group_task_map(int task_id , int group_id, void *arg) {  //TODO : add one more argument here, pointer to buffer
        DEBUG("Inside set_group_task_map");
        buffer b;
        b.grp_id = group_id;
        b.buf_ptr = (databuf *) arg;
        DEBUG("Task_id : %d, Group_id : %d", task_id, b.grp_id);
        group_task_map.insert({task_id,b});
//	group_task_map[task_id] = group_id;
        group_status[group_id] = 1;
	return SUCCESS;
} 

int track_data::release_group(int task_id) {
//	group_status[group_task_map[task_id]] = 0;
//	group_task_map[task_id] = -1;
        DEBUG("Inside release_group with task_id : %d", task_id);
        DB *db = DB::get_instance();
        unordered_map<int, buffer>::iterator it = group_task_map.find(task_id);
        if(it == group_task_map.end()) {
           DEBUG("Task id not found");
           return FAILURE;
        }   

        int group_id = it->second.grp_id;
        databuf *d = it->second.buf_ptr;
        group_status[group_id] = 0;
        DEBUG("Freed Group_id : %d", group_id);
        db->set_state(group_id, true);
        free_buffer(&d);
}     

/*
//This function to be called in handle_results(). set_track() to be called before task disrtibution and after distribution of task start timer.
int delete_client_entry (int task_id, int client_id) {

   // Client responded back with result so deleting its entry from the backup database 
    delete_track (task_id, client_id);		
}

// timer stops
void handle_timer(sigval s)
{
  track_data *td = track_data::get_instance();
//   int client_id;
//   int start;
//   int end;
//   int ret_val;
//   int *arr;
//   int put;

   SERVER_DEBUG ("Timer fired for task id : %d", s.sival_int);

   list<int> cl = td->get_clients_from_track(s.sival_int);   

   DEBUG("Clients in the list");
   for(list<int>::iterator it = cl.begin(), it != cl.end(), ++it) {
   DEBUG("*it");

   release_group(s.sival_int);
   }
// TODO : Send back data once again to the client in list cl

}
*/
