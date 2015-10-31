#ifndef _SHELL_HANDLER_GEN_H
#define _SHELL_HANDLER_GEN_H

void handle_set_max_clients(cmd_params *);
void handle_set_capacity(cmd_params *);
void handle_get_max_clients(cmd_params *);
void handle_get_capacity(cmd_params *);
void handle_show_group_clients(cmd_params *);
void handle_show_opcode_groups(cmd_params *);

/* Lookup Table*/

lkup_table fn_tbl[] = 
{
	{ &handle_set_max_clients, "handle_set_max_clients" },
	{ &handle_set_capacity, "handle_set_capacity" },
	{ &handle_get_max_clients, "handle_get_max_clients" },
	{ &handle_get_capacity, "handle_get_capacity" },
	{ &handle_show_group_clients, "handle_show_group_clients" },
	{ &handle_show_opcode_groups, "handle_show_opcode_groups" },
	{ NULL, NULL }
};

#endif

