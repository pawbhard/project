#include "server_header.h"
void Consume::spawn() {
	for(int i = 0; i < NUM_OF_OPCODES; i++) {
		thread_op[i] = new thread(consume_data,i);
		thread_op[i].detach();
	}
}

void Consume::insert_data(int opcode, struct databuf *data) {
	assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
	unique_lock<mutex> lk(mt[opcode]);
	data[opcode].push(data);
	cv[opcode].notify_all();
}

void Consume::insert_free_grp(int opcode, int grp_id) {
	assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
	unique_lock<mutex> lk(mt[opcode]);
	free_grp[opcode].push(grp_id);
	cv[opcode].notify_all();
}

int Consume::pop_data(int opcode) {
	int data;
	assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
	if (data[opcode].size())
	{
		data = data[opcode].front();
		data[opcode].pop();
	}
	return data;
}

int Consume::pop_free_grp(int opcode) {
	int free_grp;
	assert ((opcode < NUM_OF_OPCODES) && (opcode > 0));
	if (free_grp[opcode].size())
	{
		free_grp = free_grp[opcode].front();
		free_grp[opcode].pop();
	}
	return free_grp;
}

void Consume::consume_data(int opcode) {

	while(1) {
		unique_lock<mutex> lk(mt[opcode]);
		cv.wait(lk,[this]) {
			return((data[opcode].size() != 0) && (free_grp[opcode].size() != 0) );
		}

		if ((data[opcode].size() != 0) || (free_grp[opcode].size() != 0))
		{
			continue;
		}

		distribute_new(opcode, pop_free_grp(opcode), pop_data(opcode));
	}
}
