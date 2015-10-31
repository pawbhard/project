CC=g++

OBJ = client_task.o client_main.o client_receiver.o 

%.o: %.cpp
	$(CC) -c -g -std=gnu++11 -pthread  -I. -o $@ $< 

client_run: $(OBJ)

	g++ -o $@ $^ -g -std=gnu++11 -pthread -lrt -I.

clean:
	rm $(OBJ)
	rm client_run

