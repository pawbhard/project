#include "server_header.h"
#include "server_logging.h"
using namespace std;
void Consume::spawn() {
	for(int i = 0; i < NUM_OF_OPCODES; i++) {
		thread_op[i] = new std::thread(&Consume::consume_data,this,i);
		thread_op[i]->detach();
	}
}

void Consume::insert_data(int opcode, struct databuf *d) {
	assert ((opcode < NUM_OF_OPCODES) && (opcode >= 0));
	unique_lock<mutex> lk(mt[opcode]);
	data[opcode].push(d);
	cv[opcode].notify_all();
}

void Consume::insert_free_grp(int opcode, int grp_id) {
	assert ((opcode < NUM_OF_OPCODES) && (opcode >= 0));
	unique_lock<mutex> lk(mt[opcode]);
	free_grp[opcode].push(grp_id);
	cv[opcode].notify_all();
}

struct databuf * Consume::pop_data(int opcode) {
	struct databuf *d;
	assert ((opcode < NUM_OF_OPCODES) && (opcode >= 0));
	if (data[opcode].size())
	{
		d = data[opcode].front();
		data[opcode].pop();
	}
	return d;
}

int Consume::pop_free_grp(int opcode) {
	int free_g;
	assert ((opcode < NUM_OF_OPCODES) && (opcode >= 0));
	if (free_grp[opcode].size())
	{
		free_g = free_grp[opcode].front();
		free_grp[opcode].pop();
	}
    DEBUG("Poping free group  is %d",free_g);
	return free_g;
}

void Consume::consume_data(int opcode) {
	while(1) {
		unique_lock<mutex> lk(mt[opcode]);
		cv[opcode].wait(lk);
		if ((data[opcode].size() == 0) || (free_grp[opcode].size() == 0))
			continue;
		distribute_new(opcode, pop_free_grp(opcode), pop_data(opcode));
	}
}

bool Consume::isAvailable(int op) {
    return (bool) ((data[op].size() != 0) && (free_grp[op].size() != 0) );
}

