#include<unordered_map>
#include<cstdio>
#include<iostream>
#include<netinet/in.h>
//#include<stdlib.h>
#include <list>
#define DATABASE_ERROR(format,args...) printf("DATABASE ERROR %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
#define DATABASE_DEBUG(format,args...) printf("DATABASE DEBUG %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)

#define FAILURE 0
#define SUCCESS 1

using namespace std;

typedef struct {
    int client_id;
    int start;
    int end;
} position;

class track_data {
    private :
        unordered_map <int, list <position> > track;
        unordered_map < int, int > group_task_map;
	unordered_map < int, int> group_status; 
        track_data()
        {
            DATABASE_DEBUG("Created Object of track_data");
        }

        static track_data *td;
        track_data ( track_data const& ) {};
        track_data& operator = ( track_data const& ) {};

    public :
        static track_data* get_instance()
        {
            if (!td) td = new track_data;
            return td;
        }   
        int set_track(int task_id, int client_id, int s, int e);
        int set_group_task_map (int task_id , int group_id); 
        int delete_track(int task_id, int *client_id);
        int delete_all_track(int task_id);
        void release_group (int task_id);
         
};

