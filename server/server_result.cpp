
#include "server_result.h"

void Result::update_mean(int N, int res) {
    int new_mean = (mean_final * mean_elements + N * res ) / ( N + mean_elements);
    mean_elements +=N;
    mean_final = new_mean;
}
