#include<iostream>
#include<mutex>
#include<string.h>
#define CS 2
class Result {
    float mean_final[CS];
    int mean_elements[CS];
    float min_final[CS];
    float max_final[CS];
    // other results
    
    std::mutex mut;
    Result() { 
        memset(mean_final,0,sizeof(mean_final));
        memset(mean_elements,0,sizeof(mean_elements));
        memset(min_final,0,sizeof(min_final));
        memset(max_final,0,sizeof(max_final));
    }
    static Result *result;
    Result( Result const&) {};
    Result& operator= (Result const&) {};

    public:
    static Result* get_instance() {
        if(!result) result = new Result;
        return result;
    }
    
    void update_mean(int N,float res,int sw);
    float get_mean(int sw = 0) { return mean_final[sw]; }
    int get_mean_elements(int sw = 0) { return mean_elements[sw]; }
    void update_range(float min_new, float max_new, int sw);
    void get_range(float &min, float &max,int sw = 0) { min = min_final[sw]; max = max_final[sw]; }
};
