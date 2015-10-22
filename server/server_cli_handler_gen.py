
shell_handler = open("server_cli_handler_gen.h","w")

shell_handler.write("\
#ifndef _SHELL_HANDLER_GEN_H\n\
#define _SHELL_HANDLER_GEN_H\n\n\
")


with open("server.cmds", "r") as shell_cmds:
    for line in shell_cmds:
        if line.startswith("handler") == True:
            lineset = line.split(" ");
            shell_handler.write("void "+lineset[1].rstrip()+"(cmd_params *);\n");
    shell_cmds.close();

shell_handler.write("\n/* Lookup Table*/\n\n");
shell_handler.write("lkup_table fn_tbl[] = \n{\n");



with open("server.cmds", "r") as shell_cmds:
    for line in shell_cmds:
        if line.startswith("handler") == True:
            lineset = line.split(" ")
            shell_handler.write("\t{ &"+lineset[1].rstrip()+", \""+lineset[1].rstrip()+"\" },\n")
    shell_cmds.close();

shell_handler.write("\t{ NULL, NULL }\n};");

shell_handler.write("\n\n#endif\n\n");
shell_handler.close()


