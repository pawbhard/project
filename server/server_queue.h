class Consume {

      mutex mt[NUM_OF_OPCODES];
      condition_variable cv[NUM_OF_OPCODES];
      thread_op[NUM_OF_OPCODES];
      Consume()
      {
         DEBUG("Created Object of Consume");
      }

      static Consume *cs;
      Consume (Consume const&) {};
    public:
      static Consume* get_instance()
      {
        if (!cs) cs = new Consume;
        return cs;
      }

      queue <struct databuf*> data[NUM_OF_OPCODES];
      queue <int> free_grp[NUM_OF_OPCODES];

      void spawn(); 
      void insert_data(int opcode, struct databuf *data); 
      void insert_free_grp(int opcode, int grp_id);
      int pop_data(int opcode);
      int pop_free_grp(int opcode);
      void consume_data(int opcode);
};
