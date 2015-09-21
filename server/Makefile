CC=g++

OBJ = server_connection.o server_main.o socket_util.o server_databuffer.o server_result.o threadpool.o

%.o: %.cpp
	$(CC) -c -g -std=gnu++11 -pthread -I. -o $@ $< 

server_run: $(OBJ)
	g++ -o $@ $^ -g -std=gnu++11 -pthread -I.
