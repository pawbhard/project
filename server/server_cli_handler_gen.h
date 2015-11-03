#ifndef _SHELL_HANDLER_GEN_H
#define _SHELL_HANDLER_GEN_H

void handle_set_max_clients(cmd_params *);
void handle_set_capacity(cmd_params *);
void handle_get_max_clients(cmd_params *);
void handle_get_capacity(cmd_params *);
void handle_show_group_clients(cmd_params *);
void handle_show_opcode_groups(cmd_params *);
void handle_show_num_clients(cmd_params *);
void handle_show_server_status(cmd_params *);
void handle_show_results_switch_id(cmd_params *);
void handle_show_results(cmd_params *);
void handle_debug_toggle(cmd_params *);
void handle_set_debug_level(cmd_params *);

/* Lookup Table*/

lkup_table fn_tbl[] = 
{
	{ &handle_set_max_clients, "handle_set_max_clients" },
	{ &handle_set_capacity, "handle_set_capacity" },
	{ &handle_get_max_clients, "handle_get_max_clients" },
	{ &handle_get_capacity, "handle_get_capacity" },
	{ &handle_show_group_clients, "handle_show_group_clients" },
	{ &handle_show_opcode_groups, "handle_show_opcode_groups" },
	{ &handle_show_num_clients, "handle_show_num_clients" },
	{ &handle_show_server_status, "handle_show_server_status" },
	{ &handle_show_results_switch_id, "handle_show_results_switch_id" },
	{ &handle_show_results, "handle_show_results" },
	{ &handle_debug_toggle, "handle_debug_toggle" },
	{ &handle_set_debug_level, "handle_set_debug_level" },
	{ NULL, NULL }
};

#endif

