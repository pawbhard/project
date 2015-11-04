
#include "server_result.h"
#include "server_logging.h"

void Result::update_mean(int N, float res,int sw = 0) {
    std::unique_lock<std::mutex> lk(mut[sw],std::defer_lock);
    lk.lock();
    float new_mean = (mean_final[sw] * mean_elements[sw] + N * res ) / ( N + mean_elements[sw]);
    mean_elements[sw] +=N;
    mean_final[sw] = new_mean;
    update_mean_hist(sw,new_mean);
    lk.unlock();
    DEBUG("Updated mean with value %f for switch id %d ",res,sw);
    //std::cout<<"\nUpdated mean with value "<<res<<"for swid "<<sw<<"\n";
}
void Result::update_range(float min_new, float max_new, int sw = 0) {
    std::unique_lock<std::mutex> lk(mut[sw],std::defer_lock);
    lk.lock();
    if(min_final[sw] > min_new ) 
    {
        min_final[sw] = min_new;
    }
    if(max_final[sw] < max_new)
    {
        max_final[sw] = max_new;
    }

    update_min_max_hist(sw, min_final[sw], max_final[sw]);
    lk.unlock();
    DEBUG("Updated Range with value %f,%f for switch id %d ",min_new,max_new,sw);
    //std::cout<<"\nUpdated Range for sw id "<<sw<<"\n";
}
void Result::print_history_mean() {
    for (int sw = 0 ; sw < CS; sw++) {
        std::cout<<"\n Switch"<< sw <<" Mean history : ";
        for(int j = 0 ; j < HIST ; j++) {
            int t = mean_counter[sw];
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
            int t = range_counter[sw];
            std::cout<<"("<<min_final_hist[sw][t]<<", "<<max_final_hist[sw][t]<<") ";
            t++; 
            if(t == HIST) t = 0;
        }
    }
    std::cout<<"\n";
}

