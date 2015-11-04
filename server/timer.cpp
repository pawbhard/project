#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1

#include<time.h>
#include<unistd.h>

using namespace std;
class timer {
    private:
        int duration;
        timer_t timer_id;
        int task_id;
        void (*handler)(sigval);
    public:
        timer(int id, int d,void (*temp) (sigval) ) {
            duration = d;
            handler  = temp;
            task_id = id;

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            sched_param parm;
            parm.sched_priority = 255;
            pthread_attr_setschedparam(&attr,&parm);

            sigevent sev;
            sev.sigev_notify_attributes     = &attr;
            sev.sigev_notify                = SIGEV_THREAD;
            sev.sigev_notify_function       = handler;
            sev.sigev_value.sival_ptr       = this;
            sev.sigev_value.sival_int       = task_id;
            sev.sigev_signo                 = SIG;
            timer_create(CLOCKID, &sev, &timer_id);
        }
        ~timer() {
            stop();
        }
        void stop() { 
            itimerspec its;
            its.it_value.tv_sec = 0;
            its.it_value.tv_nsec = 0;
            timer_settime(timer_id, 0 ,&its, NULL);
            timer_delete (timer_id);
        }
        void start() {
             itimerspec its;

             its.it_value.tv_sec     = duration;
             its.it_value.tv_nsec    = 0;

             its.it_interval.tv_sec  = 0;
             its.it_interval.tv_nsec = 0;
             timer_settime(timer_id, 0, &its, NULL);
        }
};
