#include "server_config.h"

static unsigned int capacity = 100;
static unsigned int max_clients = 50;

void set_max_clients(unsigned int x) {
    max_clients = x;
}

void set_capacity(unsigned int x) {
    capacity = x;
}

unsigned int get_capacity() {
    return capacity;
}
unsigned int get_max_clients() {
    return max_clients;
}

