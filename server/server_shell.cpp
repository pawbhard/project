#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "server_shell.h"
#include "server_cli_handler_gen.h"

using namespace std;
using namespace boost;

void trim_string(string &s, char *trim_chars) {
    s.erase(0, s.find_first_not_of(trim_chars));
    s.erase(s.find_last_not_of(trim_chars)+1);
}

string& trim(string &s) {
    char trim_chars[] = " \t\n\r";
    trim_string(s, trim_chars);
    return s;
}

void display_params(cmd_params *params)
{
    cmd_params *tmp = params;
    while(tmp) {
        cout << "ID : "<<tmp->id<<" Type : "<<tmp->type<<" len : "<<tmp->len<<" Value : "<<tmp->val<<endl;
        tmp=tmp->next;

    }
}

void free_params(cmd_params * &params )
{
    if( params == NULL )
        return;

    cmd_params *tmp = NULL;

    while(params) {
        tmp = params->next;
        free(params->val);
        free(params);
        params = tmp;
    }
    params = NULL;
}

bool add_cmd_params(string type, int id, cmd_params * &params, string val) {

    if( type != "int" && type != "str" && type != "var")
        return false;
    
    cmd_params *tmp = (cmd_params *) malloc(sizeof(cmd_params));
    
    tmp->id = id;
    strcpy(tmp->type,type.c_str());

    tmp->len = val.length();
    
    char *value = (char *)malloc((sizeof(char)*tmp->len)+1);
    strcpy(value, val.c_str());
    tmp->val = value;

    tmp->next = params;
    params = tmp;
    return true;
}

bool find_fn_ptr(string line, cmd_params * &params) {
    for( int i=0; fn_tbl[i].fn; i++ )
    {
        if(line == fn_tbl[i].key)
        {
            (*(fn_tbl[i].fn))(params);
            free_params(params);
            return true;
        }
    }
    return false;
}

bool parse_and_call_handler(string &input_cmd) {

    bool cmd_start = false;
    bool syntax_match = false;
    string line;
    ifstream conf_file;
    cmd_params *params = NULL;

    int id = 0;

    conf_file.open("server.cmds");

    if(!conf_file.is_open())
        return false;

    while(getline( conf_file, line)){
        line = trim(line);

        if(!line.length())
            continue;

        if( cmd_start == false && line == "</cmd>")
        {
            cout << "Parsing cmdfile failed, got </cmd> before <cmd>";
            free_params(params);
            return false;
        }
        else if( cmd_start == true && line == "<cmd>")
        {
            cout << "Parsing cmdfile failed, got <cmd> before </cmd>";
            free_params(params);
            return false;
        }

        if(line == "<cmd>") {
            cmd_start = true;
            continue;
        }
        else if (line == "</cmd>") {
            syntax_match = false;
            cmd_start = false;
            continue;
        }


        if ( cmd_start != true ) {
            continue;
        }
        //syntax already matched 
        if( syntax_match == true )
        {
            if(line.find("handler") == 0)
            {
                line.erase(0, strlen("handler"));
                line = trim(line);
                return find_fn_ptr(line, params);
            }
        }

        //check syntax first
        else if( syntax_match == false && line.find("syntax") == 0) {
            line.erase(0, strlen("syntax"));
            line = trim(line);

            //tokenize the command
            istringstream file_iss(line);
            istringstream input_iss(input_cmd);
            bool option_match_done = false;

            while ( file_iss && input_iss ){

                string file_sub;
                string input_sub;
                bool option_match = false;

                file_iss >> file_sub;
                input_iss >> input_sub;

                if(!input_sub.length() || !file_sub.length())
                    continue;

                if( input_sub != file_sub ) 
                {
                    if( file_sub.c_str()[0] == '<' &&
                            file_sub.c_str()[file_sub.length()-1] == '>') {
                        char tmp_str[] = "<>";
                        trim_string(file_sub, tmp_str);
                        if(!add_cmd_params(file_sub, id++, params,
                                    input_sub))
                        {
                            free_params(params);
                        }
                        continue;
                    } else if( file_sub.c_str()[0] == '{') {
                        while( file_sub.c_str()[0] != '}') {
                            file_iss >> file_sub;
                            file_sub = trim(file_sub);

                            if(!file_sub.length())
                                continue;

                            char_separator<char> sep("|");
                            tokenizer<char_separator<char>> tokens(file_sub, sep);
                            BOOST_FOREACH(string t, tokens)
                            {
                                if( t.c_str()[0] == '<' &&
                                        t.c_str()[t.length()-1] == '>') {
                                    char tmp_str[] = "<>";
                                    trim_string(t, tmp_str);
                                    if(t == "int")
                                    {
                                        bool isNumber = true;
                                        for(string::const_iterator k = input_sub.begin(); k != input_sub.end(); ++k)
                                            isNumber = isNumber && isdigit(*k);
                                        if(isNumber)
                                        {   
                                            syntax_match = true;
                                            option_match_done = true;
                                        }
                                    }
                                    if(!add_cmd_params(t, id++, params,
                                                input_sub))
                                    {
                                        free_params(params);
                                        params = NULL;
                                    }
                                    if( syntax_match == true )
                                        break;
                                } else {
                                    if( t == input_sub )
                                    {
                                        syntax_match = true;
                                        option_match_done = true;
                                        free_params(params);
                                        params = NULL;
                                        if(!add_cmd_params("var", id++, params,
                                                input_sub))
                                        {
                                            free_params(params);
                                            params = NULL;
                                        }
                                    
                                        break;
                                    }
                                }
                            }
                            if(option_match_done == true)
                                break;
                        }
                        if (syntax_match == false)
                            break;
                    }
                    else {
                        syntax_match = false;
                        break;
                    }
                }
            } 


            if( (!file_iss && !input_iss) || (option_match_done == true && syntax_match == true) )
                syntax_match = true;
            else {
                free_params(params);
                continue;
            }

        }


    }

    return false;
}

void show_commands(string type) {
    ifstream conf_file;
    string line;

    conf_file.open("server.cmds");
    
    cout << "\nList of available commands :"<<endl;

    while(getline( conf_file, line)){
        line = trim(line);

        if(line.find("syntax") == 0) {
            line.erase(0, strlen("syntax"));
            line = trim(line);
            if( type == "set help")
            {
                if (line.find("set") == 0) {
                    cout << line <<endl;
                }
            } 
            else if( type == "show help" )
            {
                if (line.find("show") == 0) {
                    cout<< line<<endl;
                }
            } else if ( type== "debug help" )
            {
                if(line.find("debug") == 0) {
                    cout << line<<endl;
                }
            }
            else {
                cout << line <<endl;
            }
        }
    }
    cout <<endl;
}


bool handle_command(string &input_cmd) {
    
    bool no_cmd = false; 
    //find if it is a "no" command
                        
    if(input_cmd.find("no ") == 0) {
       no_cmd = true;

       input_cmd.erase(0,strlen("no "));
       input_cmd = trim(input_cmd);
    }
    
    return parse_and_call_handler(input_cmd);
}
void run_shell()
{
    string buf;

    while(1) {
        cout << PROMPT;
        getline(cin, buf);

        buf = trim(buf); 
        
        if(!buf.length())
            continue;

        if (buf == "exit" || buf == "quit") {
            cout << "\nExiting Server\n";
            exit(0);
        }
        else if (buf == "help" || buf == "set help" || buf == "show help" || buf == "debug help") {
            show_commands(buf);
        }
        else {
            if( handle_command(buf) == false ) {
                cout << "\nInvalid cmd\n";
            }
        }
    }
}
