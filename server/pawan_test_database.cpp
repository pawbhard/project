
#include <iostream>
#include "database_def.h"
DB *DB::db = NULL;
int main()
{
    DB *d;
    d  = DB::get_instance();
    d->add_new_client(12,1);
    d->add_new_client(15,1);
    d->add_new_client(18,1);
    d->add_new_client(19,2);
    d->add_new_client(28,1);
    d->add_new_client(48,1);
    d->add_new_client(58,1);
    d->add_new_client(68,1);
    int id = d->get_free_group(2);
    cout<<id<<"\n";
    set<int> gl = d->get_client_list(id);
    set<int>::iterator it;
    std::cout<<"Client list opcode 2 ";
    for(it = gl.begin(); it!= gl.end() ; it++)
        cout<<*it<<" ";

    cout<<"\n";   
    
    return 0;
}
