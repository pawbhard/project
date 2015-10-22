#ifndef _SERVER_SHELL_H

#define _SERVER_SHELL_H

#include <iostream>

#define ORIG_PROMPT "server#"
#define CONF_PROMPT "server(config)#"


typedef struct cmd_params_ {
    int id;
    char type[4];
    int len;
    char* val;
    struct cmd_params_ *next;
} cmd_params;


void free_params(cmd_params * &params);
void display_params(cmd_params * &params);

typedef struct lkup_table_
{
    void (*fn)(cmd_params *);
    const char* key;
} lkup_table;

#endif

