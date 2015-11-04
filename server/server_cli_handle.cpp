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
}

void handle_set_capacity(cmd_params *params)
{
    if((string)params->type == "int") {
        set_capacity(atoi(params->val));
    }
}


void handle_get_capacity(cmd_params *params)
{
    cout << "Current capacity is : "<<get_capacity()<<endl;
}

void handle_get_max_clients(cmd_params *params)
{
    cout << "Max clients is : "<<get_max_clients()<<endl;
}

void handle_show_group_clients(cmd_params *params)
{   
    int group_id = 0;
    DB *db = NULL;
    
    db = DB::get_instance();

    if( !params || string(params->val) == "all" )
    {
       db->display_group_clients_all();
    }
    else {
        group_id = atoi(params->val);
        db->display_group_clients(group_id);
    }
}

void handle_show_opcode_groups(cmd_params *params)
{
    int opcode = 0;
    DB *db = NULL;

    db = DB::get_instance();

    if( !params || string(params->val) == "all" )
    {
        db->display_opcode_groups_all();
    }
    else {
        opcode = atoi(params->val);
        db->display_opcode_groups(opcode);
    }
}

void handle_show_num_clients(cmd_params *params)
{
    connection *con = connection::get_instance();
    cout << "Number of clients :"<<con->get_number_of_clients()<<endl;
}

void handle_show_server_status(cmd_params *params) {
    cout << "Server status : Active "<<endl;
    handle_get_capacity(NULL);
    handle_get_max_clients(NULL);
    handle_show_num_clients(NULL);
    cout << "\nOpcode Groups :"<<endl;
    cout << "================"<<endl;
    handle_show_opcode_groups(NULL);
    cout << "\nGroup Clients :"<<endl;
    cout << "================"<<endl;
    handle_show_group_clients(NULL);
}

void handle_show_results(cmd_params *params) {
    Result *res = Result::get_instance();
    if ( params ) {
        if ( string(params->val) == "all") {
            for ( int i=0; i<CS; i++) {
                cout <<"\nSwitch :"<<i<<endl;
                cout << "\nMean :"<<res->get_mean(i) <<endl;
                cout << "Mean elements :"<<res->get_mean_elements(i)<<endl;
                float min=0, max=0;
                res->get_range(min, max,i);
                cout << "Range :: min: "<<min<<" max: "<<max<<endl;
            }   
        } else if ( string(params->val) == "history") {
            res->print_history_mean();
            res->print_history_range();
        }
    }
}

void handle_show_results_switch_id(cmd_params *params) {
    Result *res = Result::get_instance();
    int switch_id = atoi(params->val);

    if(params) {
        cout << "\nSwitch :"<<switch_id<<endl; 
        cout << "\nMean :"<<res->get_mean(switch_id)<<endl;
        cout << "Mean elements :"<<res->get_mean_elements(switch_id) <<endl;
        float min=0, max=0;
        res->get_range(min, max,switch_id);
        cout << "Range :: min: "<<min<<" max: "<<max<<endl;
    }

}

void handle_debug_toggle(cmd_params *params)
{
    if(params) {
        if(string(params->val) == "enable") {
            debug_enable_disable(true);
        } else {
            debug_enable_disable(false);
        }
    }
}

