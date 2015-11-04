
#include "server_result.h"

void Result::update_mean(int N, float res,int sw = 0) {
    std::lock_guard<std::mutex> lk(mut);
    float new_mean = (mean_final[sw] * mean_elements[sw] + N * res ) / ( N + mean_elements[sw]);
    mean_elements[sw] +=N;
    mean_final[sw] = new_mean;
    std::cout<<"\nUpdated mean with value "<<res<<"for swid "<<sw<<"\n";
}
void Result::update_range(float min_new, float max_new, int sw = 0) {
    std::lock_guard<std::mutex> lk(mut);
    if(min_final[sw] > min_new ) min_final[sw] = min_new;
    if(max_final[sw] < max_new) max_final[sw] = max_new;
    std::cout<<"\nUpdated Range for sw id "<<sw<<"\n";
}
void Result::print_history_mean() {
    for (int sw = 0 ; sw < CS; sw++) {
        std::cout<<"\n Switch"<< sw <<" Mean history : ";
        for(int j = 0 ; j < HIST ; j++) {
            int t = mean_counter;
            std::cout<<mean_final_hist[sw][t]<<" ";
            t++; 
            if(t == HIST) t = 0;
        }
    }
    std::cout<<"\n";
}
void Result::print_history_range() { 
    for (int sw = 0 ; sw < CS; sw++) {
        std::cout<<"\n Switch"<< sw <<" Range history : ";
        for(int j = 0 ; j < HIST ; j++) {
            int t = range_counter;
            std::cout<<"("<<min_final_hist[sw][t]<<", "<<max_final_hist[sw][t]<<") ";
            t++; 
            if(t == HIST) t = 0;
        }
    }
    std::cout<<"\n";
}

