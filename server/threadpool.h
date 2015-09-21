#include<mutex>
#include<atomic>
#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<boost/bind.hpp>
#include<boost/function/function0.hpp>

typedef boost::function0<void> Taskfunc;

using namespace std;

template<typename T>
class threadsafe_queue {
    private:
        mutable mutex mut;
        queue<T> data_q;
        condition_variable data_cond;
    public:
        threadsafe_queue()  {
        }
        threadsafe_queue(threadsafe_queue const& other_q) {
            lock_guard<mutex> lk(other_q.mut);
            data_q = other_q.data_q;
        }
        void push(T new_value) ;
        void wait_and_pop(T& value) ;
        shared_ptr<T> wait_and_pop() ;
        bool try_pop(T& value) ;
        shared_ptr<T> try_pop() ;
        bool empty() const ;
};



class join_threads {
    std::vector<std::thread>& threads;
    
public:
    explicit join_threads(std::vector<std::thread> & threads_) :                threads(threads_) {}  
    ~join_threads() {

        for(unsigned long i = 0; i<threads.size() ; ++i) {
            if(threads[i].joinable())
                threads[i].join();
        }
    }   
};


class thread_pool { 
    std::atomic_bool done;
    threadsafe_queue<Taskfunc> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread(); 
public:
    thread_pool(): done(false), joiner(threads) {
        unsigned const thread_count = 3;
        try {
            for(unsigned i = 0; i<thread_count;++i) {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
            }
        } catch (...) {
            done = true;
            throw;
        }
    }
    ~thread_pool() {
        done = true;
    }

    void submit(void (*f)(void *), void * arg);
};



