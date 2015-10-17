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

/* Declare the command handlers here */

//Sample
void my_handler1(cmd_params *params) {
    std::cout << "Entered my_handler 1\n";
    free_params(params);

}
void my_handler2(cmd_params *params) {
    std::cout << "Entered my_handler 2\n";
    free_params(params);
}



/*end command handler declaration */



typedef struct lkup_table_
{
    void (*fn)(cmd_params *);
    const char* key;
} lkup_table;

lkup_table fn_tbl[] = 
{
    { &my_handler1, "my_handler1" },
    { &my_handler2, "my_handler2" },
    { NULL, NULL }
};


#endif

