#define MEAN 0

class Result {
    int mean_final;
    int mean_elements;
    // other results

    Result() { mean_final = 0; mean_elements = 0; }
    static Result *result;
    Result( Result const&) {};
    Result& operator= (Result const&) {};

    public:
    static Result* get_instance() {
        if(!result) result = new Result;
        return result;
    }
    
    void update_result(int task, int N, int res)
    {
        switch(task) {
            case MEAN: update_mean(N,res);
                       break;
            
            default  :; //some error handling 
        }
    }
    void update_mean(int N,int res);
    int get_mean() { return mean_final; }
    int get_mean_elements() { return mean_elements; }
};
