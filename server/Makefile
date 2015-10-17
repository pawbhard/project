CC=g++

OBJ = server_connection.o server_main.o socket_util.o server_databuffer.o server_result.o threadpool.o database_main.o server_msg_handler.o server_config.o server_shell.o

%.o: %.cpp
	$(CC) -c -g -std=gnu++11 -pthread -I. -o $@ $< 

server_run: $(OBJ)
	g++ -o $@ $^ -g -std=gnu++11 -pthread -I.
