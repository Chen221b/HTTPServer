server TCPClient UDPClient:server.o TCPClient.o UDPClient.o
	gcc -o server server.o
	gcc -o TCPClient TCPClient.o
	gcc -o UDPClient UDPClient.o

server.o: lib.h r_w_func.c get_socket.c chat.c

TCPClient.o: lib.h r_w_func.c get_socket.c

UCPClient.o: lib.h r_w_func.c get_socket.c

clean:
	rm *.o