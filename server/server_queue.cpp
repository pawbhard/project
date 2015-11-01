#include <queue>
#define NUM_OF_OPCODES 2

class Consume {

      mutex mt[NUM_OF_OPCODES];
      condition_variable cv[NUM_OF_OPCODES];
    public:
      queue <struct databuf*> data[NUM_OF_OPCODES];
      queue <int> free_grp[NUM_OF_OPCODES];

      thread spawn() {
         return thread( [this] {this->consume_data};);
      }

      void insert_data(int opcode, struct databuf *data) {
         assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
         unique_lock<mutex> lk(mt[opcode]);
         data[opcode].push(data);
         cv[opcode].notify_all();
      }

      void insert_free_grp(int opcode, int grp_id) {
         assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
         unique_lock<mutex> lk(mt[opcode]);
         free_grp[opcode].push(grp_id);
         cv[opcode].notify_all();
      }

      int pop_data(int opcode) {
         int data;
         assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
         if (data[opcode].size())
         {
             data = data[opcode].front();
             data[opcode].pop();
         }
         return data;
      }

      int pop_free_grp(int opcode) {
         int free_grp;
         assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
         if (free_grp[opcode].size())
         {
             free_grp = free_grp[opcode].front();
             free_grp[opcode].pop();
         }
         return free_grp;
      }

      void consume_data(int opcode) {
         while(1) {
            unique_lock<mutex> lk(mt[opcode]);
            cv.wait(lk,[this]) {
               return((data[opcode].size() != 0) && (free_grp[opcode].size() != 0) );
            }

           if ((data[opcode].size() != 0) || (free_grp[opcode].size() != 0))
           {
               continue;
           }

           
         }
      }
};
