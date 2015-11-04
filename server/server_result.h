#include<iostream>
#include<mutex>
#include<string.h>
#define CS 2   //Switches no 
#define HIST 10
class Result {
    float mean_final[CS];
    int mean_elements[CS];
    float min_final[CS];
    float max_final[CS];
    // other results
    
    //For history
    float mean_final_hist[CS][HIST];
    float max_final_hist[CS][HIST];
    float min_final_hist[CS][HIST];
    int mean_counter;   //for hist
    int range_counter;  //for hist

    std::mutex mut;
    Result() { 
        memset(mean_final,0,sizeof(mean_final));
        memset(mean_elements,0,sizeof(mean_elements));
        memset(min_final,0,sizeof(min_final));
        memset(max_final,0,sizeof(max_final));
        memset(max_final_hist,0,sizeof(max_final_hist));
        memset(min_final_hist,0,sizeof(min_final_hist));
        memset(mean_final_hist,0,sizeof(mean_final_hist));
        range_counter = 0;
        mean_counter  = 0;
    }
    static Result *result;
    Result( Result const&) {};
    Result& operator= (Result const&) {};
    void update_mean_hist(int sw,float val) {
        mean_final_hist[sw][mean_counter] = val;
        mean_counter++;
        if(mean_counter == HIST) mean_counter = 0;
    }
    void update_min_hist(int sw,float val) {
        min_final_hist[sw][range_counter] = val;
        range_counter++;
        if(range_counter == HIST) range_counter = 0;
    }
    void update_max_hist(int sw,float val) {
        max_final_hist[sw][range_counter] = val;
        range_counter++;
        if(range_counter == HIST) range_counter = 0;
    }
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
    
    //DISPLAY
    void print_history_mean();
    void print_history_range();
};
