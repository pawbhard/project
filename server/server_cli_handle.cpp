#include <iostream>
#include <string>

#include "server_shell.h"
#include "server_config.h"
#include "database_def.h"
#include "server_header.h"

using namespace std;

void handle_set_max_clients(cmd_params *params)
{
    if((string)params->type == "int") {
        set_max_clients(atoi(params->val));
    }
    free_params(params);
}

void handle_set_capacity(cmd_params *params)
{
    if((string)params->type == "int") {
        set_capacity(atoi(params->val));
    }
    free_params(params);
}


void handle_get_capacity(cmd_params *params)
{
    cout << "Current capacity is : "<<get_capacity()<<endl;
    free_params(params);
}

void handle_get_max_clients(cmd_params *params)
{
    cout << "Max clients is : "<<get_max_clients()<<endl;
    free_params(params);
}

void handle_show_group_clients(cmd_params *params)
{   
    int group_id = 0;
    DB *db = NULL;
    
    db = DB::get_instance();

    if( !params )
    {
       db->display_group_clients_all();
        //list all groups
    }
    else {
        group_id = atoi(params->val);
        db->display_group_clients(group_id);
    }
    free_params(params);
}

void handle_show_opcode_groups(cmd_params *params)
{
    int opcode = 0;
    DB *db = NULL;

    db = DB::get_instance();

    if( !params )
    {
        db->display_opcode_groups_all();
    }
    else {
        opcode = atoi(params->val);
        db->display_opcode_groups(opcode);
    }
    free_params(params);
}

void handle_show_num_clients(cmd_params *params)
{
    connection *con = connection::get_instance();
    cout << "Number of clients :"<<con->get_number_of_clients()<<endl;
    free_params(params);
}

void handle_set_debug_level(cmd_params *params) 
{
    free_params(params);
}
