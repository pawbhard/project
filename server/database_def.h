#include<vector>
#include<unordered_map>
#include<set>
#include<cstdio>
#include<iostream>
#include <netinet/in.h>
#include <assert.h>
#include "server_constants.h"
#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE

#define DATABASE_ERROR(format,args...) printf("DATABASE ERROR %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
#define DATABASE_DEBUG(format,args...) printf("DATABASE DEBUG %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)

#else
#define DATABASE_ERROR(format,args...) printf(" ")
#define DATABASE_DEBUG(format,args...) printf(" ")
#endif

using namespace std;

//Database class to provide only one instace of the object 
class DB 
{
    private : unordered_map <int, set<int>>  group_to_client;
              unordered_map <int, set<int>> opcode_to_group;
              unordered_map <int, bool> group_state;       //true free , false busy
              int get_group_counter(int flag) {
                  static unsigned int group_counter = 1;
                  if(flag == 1) //increment
                     return group_counter++;
                  else 
                      return group_counter;                  
              }
              DB()
              {
                  DATABASE_DEBUG("Created Object of database");
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
};

