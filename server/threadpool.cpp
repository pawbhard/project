
#include "threadpool.h"

using namespace std;

template <typename T>void threadsafe_queue<T>::push(T new_value) {
    lock_guard<mutex> lk(mut);
    data_q.push(new_value);
    data_cond.notify_one();
}
template <typename T>void threadsafe_queue<T>::wait_and_pop(T& value) {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_q.empty(); } );
    value = data_q.front();
    data_q.pop();
}
template <typename T>shared_ptr<T> threadsafe_queue<T>::wait_and_pop() {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk , [this] { return !data_q.empty(); } );
    shared_ptr<T> res(make_shared<T> (data_q.front()) ) ;
    data_q.pop();
    return res;
}
template <typename T>bool threadsafe_queue<T>::try_pop(T& value) {
    lock_guard<mutex> lk(mut);
    if(data_q.empty())
        return false;
    value = data_q.front();
    data_q.pop();
    return true;
}
template <typename T>shared_ptr<T> threadsafe_queue<T>::try_pop() {
    lock_guard<mutex> lk(mut);
    if(data_q.empty())
        return shared_ptr<T> ();
    shared_ptr<T> res (make_shared<T> ( data_q.front()));
    data_q.pop();
    return res;
}
template <typename T>bool threadsafe_queue<T>::empty() const {
    lock_guard<mutex> lk(mut);
    return data_q.empty();
}


void thread_pool::worker_thread(int i) {
    std::shared_ptr<std::atomic<bool>> f(this->flag[i]); // a copy of the shared ptr to the flag
    std::atomic<bool> & _flag = *f; //if flag is true exit
    while(!done||!_flag) {
        Taskfunc task;
        if(work_queue.try_pop(task)) {
            task();
        } else { 
            std::this_thread::yield();
        }
    }
}

void thread_pool::submit(void (*f)(void *), void * arg) {
    work_queue.push(boost::bind(f,arg));
}

//Function to resize thread pool 
void thread_pool::resize(int nthread) {
    int i;
    int oldThread = static_cast<int> (threads.size());
    //Need to increase the size 
    if(oldThread < nthread) {
        threads.resize(nthread);
        flag.resize(nthread);
        for(i = oldThread; i < nthread ; i++ ) {
            flag.push_back(std::make_shared<std::atomic<bool>>(false));
            threads.push_back(
                std::thread(&thread_pool::worker_thread,this,i));
        }
    }

    //decrease threads
    if(oldThread > nthread) {
        for (int i = oldThread - 1; i >= nthread; --i) {
            flag[i] = std::make_shared<std::atomic<bool>>(true);  // this thread will finish
            threads[i].detach();
        }
        threads.resize(nthread);
        flag.resize(nthread);
    }
}


