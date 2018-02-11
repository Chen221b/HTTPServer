server client:server.o client.o
	gcc -o server server.o
	gcc -o client client.o

server.o: lib.h r_w_func.c

client.o: lib.h r_w_func.c

clean:
	rm *.o