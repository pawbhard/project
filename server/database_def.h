#include<vector>
#include<unordered_map>
#include<set>
#include<cstdio>
#include<iostream>
#include <netinet/in.h>
#include <assert.h>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<thread>

#include "server_constants.h"
#include "server_logging.h"
#include "server_queue.h"

using namespace std;

//Database class to provide only one instace of the object 
class DB 
{
    private : unordered_map <int, set<int>>  group_to_client;
              unordered_map <int, set<int>> opcode_to_group;
              unordered_map <int, bool> group_state;       //true free , false busy
              mutex mut;        //mutex
              int get_group_counter(int flag) {
                  static unsigned int group_counter = 1;
                  if(flag == 1) //increment
                     return group_counter++;
                  else 
                      return group_counter;                  
              }
              DB()
              {
                  DEBUG("Created Object of database");
              }

              static DB *db;
              DB ( DB const& ) {};
              DB& operator = ( DB const& ) {};
              bool is_free(int group_id);
    public  : static DB* get_instance ()
              {
                 if (!db) db = new DB;
                 return db;
              }  
            
              //APIs (All are made int to return error )
              int add_new_client(int client_id, int opcode);
              set<int> get_client_list(int group_id);
              int get_free_group(int opcode);
              int delete_client(int client_id);
              int set_state(int group_id, bool state);
              void display_group_clients_all();
              void display_group_clients(int group);
              void display_opcode_groups_all();
              void display_opcode_groups(int opcode);
              int find_opcode_from_grp(int grp_id);
};

