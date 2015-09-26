#define MEAN 0
#include<iostream>
class Result {
    float mean_final;
    int mean_elements;
    float min_final;
    float max_final;
    // other results

    Result() { mean_final=mean_elements=min_final=max_final=0;  }
    static Result *result;
    Result( Result const&) {};
    Result& operator= (Result const&) {};

    public:
    static Result* get_instance() {
        if(!result) result = new Result;
        return result;
    }
    
    void update_mean(int N,float res);
    float get_mean() { return mean_final; }
    int get_mean_elements() { return mean_elements; }
    void update_range(float min_new, float max_new);
};
