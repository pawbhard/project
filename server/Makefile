CC=g++

OBJ = server_logging.o server_connection.o server_main.o socket_util.o server_databuffer.o server_result.o threadpool.o database_main.o server_msg_handler.o server_config.o server_shell.o server_cli_handle.o server_tracker.o timer.o server_queue.o

%.o: %.cpp
	$(CC) -c -g -std=gnu++11 -pthread  -I. -o $@ $< 

server_run: $(OBJ)

	python server_cli_handler_gen.py
	g++ -o $@ $^ -g -std=gnu++11 -pthread -lrt -I.

clean:
	rm $(OBJ)
	rm server_run

