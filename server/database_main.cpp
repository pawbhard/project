
#include "database_def.h"

int DB::add_new_client( int client_id, int opcode) {
    DATABASE_DEBUG("Adding mapping opcode %d to client_id %d",opcode,client_id);
    unordered_map<int, set<int>>::iterator it;
    set<int> group_list;
    it =  opcode_to_group.find(opcode);
    if(it != opcode_to_group.end())
        group_list = it->second;
        //it->second = opcode_to_group.find(opcode);
    
    lock_guard<mutex> lk(mut);
    if(group_list.size() == 0) {
        DATABASE_DEBUG("No groups for this opcode . Creating new");
        int gp_id = get_group_counter(1);
        
        set<int> cl;
        cl.insert(client_id);
        group_to_client.insert({gp_id, cl});

        set<int> gp;
        gp.insert(gp_id);
        opcode_to_group.insert({opcode, gp});

        group_state.insert({gp_id, true});
        return SUCCESS;
    }
    
    if(it == opcode_to_group.end()) assert(0);
    group_list = it->second;
    set<int> c_list;
    set<int>::iterator itt;
    unordered_map<int, set<int>>::iterator it3;
    for(itt = group_list.begin(); itt!= group_list.end() ; itt++) {
        it3 = group_to_client.find(*itt);
        if(it3 == group_to_client.end()) assert(0);
        if(it3->second.size() != 5) {  //group is not full
            DATABASE_DEBUG("Group not full adding one more");
            it3->second.insert(client_id);
            return SUCCESS;
        } else {
            DATABASE_DEBUG("This group is full checking next");
        }
    }    
    DATABASE_DEBUG("No free group  Creating new");
    int gp_id = get_group_counter(1);

    set<int> cl;
    cl.insert(client_id);
    group_to_client.insert({gp_id, cl});

    group_list.insert(gp_id);
    it->second = group_list;

    group_state.insert({gp_id, true});
    return SUCCESS;
}

set<int> DB::get_client_list(int group_id) {
    unordered_map<int, set<int>>::iterator it;
    set<int> cl_list;
    it = group_to_client.find(group_id);
    if(it == group_to_client.end()) {
        DATABASE_DEBUG("NO known group");
        return cl_list;
    }
    
    cl_list = it->second;
    return cl_list;
}

int DB::get_free_group(int opcode) {
    DATABASE_DEBUG("getting free group for opcode %d",opcode);
    unordered_map<int, set<int>>::iterator it;
    it = opcode_to_group.find(opcode);
    if(it == opcode_to_group.end()) {
        DATABASE_DEBUG("No Group found");
        return -1;
    }
    lock_guard<mutex> lk(mut);
    set<int> glist = it->second;
    set<int>::iterator it1;
    for(it1 = glist.begin(); it1!= glist.end(); it1++) {
        if(is_free(*it1)) {
            return *it1;
        }
    }
    DATABASE_DEBUG("No free group");
    return -1;
}

bool DB::is_free(int group_id) { 
    unordered_map<int, bool>::iterator it;
    it = group_state.find(group_id);
    if(it == group_state.end()) return false;

    return it->second;
}

int DB::delete_client(int client_id) {

    //find all group for this client and remove from all places
    DATABASE_DEBUG("Got request for deleting client %d",client_id);
    unordered_map<int, set<int>>::iterator it;
    set<int>::iterator it1;
    lock_guard<mutex> lk(mut);
    for(it = group_to_client.begin(); it!= group_to_client.end();it++) {
        DATABASE_DEBUG("Searching in group %d",it->first);
        it1 = it->second.find(client_id);
        if(it1 != it->second.end()) {
            DATABASE_DEBUG("Client found deleting");
            it->second.erase(it1);
        }
    }
}

int DB::set_state(int group_id, bool state) {
    unordered_map<int , bool>::iterator it;
    it = group_state.find(group_id);
    if(it == group_state.end()) {
        DATABASE_DEBUG("Group not found");
        return FAILURE;
    }
    it->second = state;
    return SUCCESS;
}
