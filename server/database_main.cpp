
#include "database_def.h"

void DB::display_opcode_groups_all() {
    unordered_map<int, set<int>>::iterator it;
    
    if( opcode_to_group.begin() == opcode_to_group.end())
    {
        cout << "No operations present"<<endl;
        return;
    }

    for( it = opcode_to_group.begin(); it != opcode_to_group.end(); it++ )
    {
        cout << "\nOperation (opcode) : "<<it->first<<endl;
        set<int> group_list = it->second;
        set<int>::iterator itt;
        
        if( group_list.begin() != group_list.end() )
            cout << "Groups : ";
        else
            cout << "No groups performing this Op" <<endl;
        for ( itt = group_list.begin(); itt != group_list.end(); itt++ )
        {
            cout << *itt<<"\t";
        }
    }
    cout<<endl;
}

void DB::display_opcode_groups(int opcode) {

    unordered_map<int, set<int>>::iterator it;
    set<int> group_list;

    it =  opcode_to_group.find(opcode);
    if(it != opcode_to_group.end())
        group_list = it->second;
    else 
    {
        cout << "Opcode Not found"<<endl;
        return;
    }

    set<int>::iterator itt;
    if( group_list.begin() != group_list.end() )
        cout << "Groups : ";
    else
        cout << "No groups in the opcode" <<endl;
    for ( itt = group_list.begin(); itt != group_list.end(); itt++ )
    {
        cout << *itt<<"\t";
    }
    cout << endl;
}
void DB::display_group_clients(int group) {

    unordered_map<int, set<int>>::iterator it;
    set<int> client_list;

    it =  group_to_client.find(group);
    if(it != opcode_to_group.end())
        client_list = it->second;
    else 
    {
        cout << "Group Not found"<<endl;
        return;
    }

    set<int>::iterator itt;
    if( client_list.begin() != client_list.end() )
        cout << "Clients : ";
    else
        cout << "No clients in the group" <<endl;
    for ( itt = client_list.begin(); itt != client_list.end(); itt++ )
    {
        cout << *itt<<"\t";
    }
    cout << endl;
}

void DB::display_group_clients_all() {
    unordered_map<int, set<int>>::iterator it;
    
    if( group_to_client.begin() == group_to_client.end())
    {
        cout << "No groups present"<<endl;
        return;
    }

    for( it = group_to_client.begin(); it != group_to_client.end(); it++ )
    {
        cout << "\nGroup ID : "<<it->first<<endl;
        set<int> client_list = it->second;
        set<int>::iterator itt;
        
        if( client_list.begin() != client_list.end() )
            cout << "Clients : ";
        else
            cout << "No clients in the group" <<endl;
        for ( itt = client_list.begin(); itt != client_list.end(); itt++ )
        {
            cout << *itt<<"\t";
        }
    }
    cout << endl;
}

int DB::add_new_client( int client_id, int opcode) {
    Consume *cs = Consume::get_instance();
    DEBUG("Adding mapping opcode %d to client_id %d",opcode,client_id);
    unordered_map<int, set<int>>::iterator it;
    set<int> group_list;
    it =  opcode_to_group.find(opcode);
    if(it != opcode_to_group.end())
        group_list = it->second;
        //it->second = opcode_to_group.find(opcode);
    
    lock_guard<mutex> lk(mut);
    if(group_list.size() == 0) {
        DEBUG("No groups for this opcode . Creating new");
        int gp_id = get_group_counter(1);
        cs->insert_free_grp(opcode, gp_id);
        
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
            DEBUG("Group not full adding one more");
            it3->second.insert(client_id);
            return SUCCESS;
        } else {
            DEBUG("This group is full checking next");
        }
    }    
    DEBUG("No free group  Creating new");
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
        DEBUG("NO known group");
        return cl_list;
    }
    
    cl_list = it->second;
    return cl_list;
}

int DB::get_free_group(int opcode) {
    DEBUG("getting free group for opcode %d",opcode);
    unordered_map<int, set<int>>::iterator it;
    it = opcode_to_group.find(opcode);
    if(it == opcode_to_group.end()) {
        DEBUG("No Group found");
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
    DEBUG("No free group");
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
    DEBUG("Got request for deleting client %d",client_id);
    unordered_map<int, set<int>>::iterator it;
    set<int>::iterator it1;
    lock_guard<mutex> lk(mut);
    for(it = group_to_client.begin(); it!= group_to_client.end();it++) {
        DEBUG("Searching in group %d",it->first);
        it1 = it->second.find(client_id);
        if(it1 != it->second.end()) {
            DEBUG("Client found deleting");
            it->second.erase(it1);
        }
    }
}

int DB::set_state(int group_id, bool state) {
    Consume *cs = Consume::get_instance();
    unordered_map<int , bool>::iterator it;
    it = group_state.find(group_id);
    if(it == group_state.end()) {
        DEBUG("Group not found");
        return FAILURE;
    }
    it->second = state;

    int opcode = find_opcode_from_grp(group_id);
    if (state == true)
    {
        cs->insert_free_grp(opcode, group_id);
    }
    return SUCCESS;
}

int DB::find_opcode_from_grp(int grp_id) {
    unordered_map<int, set<int>>::iterator it;
    set<int>::iterator it1;
    for(it = opcode_to_group.begin(); it!= opcode_to_group.end();it++) {
        DEBUG("Searching in opcode %d",it->first);
        it1 = it->second.find(grp_id);
        if(it1 != it->second.end()) {
           return it->first;
        }
    }
}
