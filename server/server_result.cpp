
#include "server_result.h"

void Result::update_mean(int N, float res) {
    float new_mean = (mean_final * mean_elements + N * res ) / ( N + mean_elements);
    mean_elements +=N;
    mean_final = new_mean;
    std::cout<<"Updated mean with value "<<res<<"\n";
}
void Result::update_range(float min_new, float max_new) {
    if(min_final > min_new ) min_final = min_new;
    if(max_final < max_new) max_final = max_new;
}

