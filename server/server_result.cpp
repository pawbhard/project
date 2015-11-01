
#include "server_result.h"

void Result::update_mean(int N, float res,int sw = 0) {
    std::lock_guard<std::mutex> lk(mut);
    float new_mean = (mean_final[sw] * mean_elements[sw] + N * res ) / ( N + mean_elements[sw]);
    mean_elements[sw] +=N;
    mean_final[sw] = new_mean;
    std::cout<<"Updated mean with value "<<res<<"for swid "<<sw<<"\n";
}
void Result::update_range(float min_new, float max_new, int sw = 0) {
    std::lock_guard<std::mutex> lk(mut);
    if(min_final[sw] > min_new ) min_final[sw] = min_new;
    if(max_final[sw] < max_new) max_final[sw] = max_new;
    std::cout<<"Updated Range for sw id "<<sw<<"\n";
}

