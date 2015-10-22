#include <iostream>
#include <string>

#include "server_shell.h"
#include "server_config.h"

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

    free_params(params);
}

void handle_get_max_clients(cmd_params *params)
{
    free_params(params);
}

void handle_get_client_groups(cmd_params *params)
{
    free_params(params);
}

void my_handler2(cmd_params *params)
{
    free_params(params);
}


