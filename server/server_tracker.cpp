#include "server_tracker.h"
//#include <list.h>
//using std::make_pair;
 
int track_data::set_track(const int task_id, int client_id, int s, int e) {
    list <position> client_list = track[task_id];
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
    unordered_map<int, list <position> >::iterator it = track.find(task_id);
    if(it == track.end()) {
        DATABASE_DEBUG("Task not found");
        return FAILURE;
    }
    track.erase(it);
    return SUCCESS;
}  

// to delete a particular client-id
int track_data::delete_track(int task_id, int *client_id) {
    unordered_map<int, list <position> >::iterator it = track.find(task_id);
    if(it == track.end()) {
        DATABASE_DEBUG("Task not found");
        return FAILURE;
    }
    list <position> client_list = track[task_id];
    position p;
    for (list<position>::iterator l_it=client_list.begin(); l_it != client_list.end(); ++l_it) {
        p = *l_it;
        if(p.client_id == *client_id) { 
           client_list.erase(l_it);
	   break;
         }
        
    }
    track[task_id] = client_list;
    return SUCCESS;
} 

int track_data::set_group_task_map(int task_id , int group_id) {
	group_task_map[task_id] = group_id;
        group_status[group_id] = 1;
	return SUCCESS;
} 

void track_data::release_group(int task_id) {
	group_status[group_task_map[task_id]] = 0;
	group_task_map[task_id] = -1;
}

