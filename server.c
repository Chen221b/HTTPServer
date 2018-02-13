#define _BSD_SOURCE

#include <netdb.h>
#include "lib.h"
#include "r_w_func.c"
#include "get_socket.c"
#include "chat.c"

#define BACKLOG 50
#define max(A,B) ((A)>(B)?(A):(B))

void sig_child(int sig)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminate\n", pid);
	return;
}

int main(int argc, char const *argv[])
{
	int tcpfd, udpfd, nready, maxfdpl;
	fd_set rset;

	void sig_child(int);
	signal(SIGPIPE, SIG_IGN);	//SIG_IGN ignore the signal and return 1
	signal(SIGCHLD, sig_child);
	tcpfd = getTCPSocket(0);
	listen(tcpfd, BACKLOG);

	udpfd = getUDPSocket(0);
	// UDPChat(udpfd);

	FD_ZERO(&rset);
	maxfdpl = max(tcpfd, udpfd) + 1;
	for(;;){
		FD_SET(tcpfd, &rset);
		FD_SET(udpfd, &rset);
		if((nready = select(maxfdpl, &rset, NULL, NULL, NULL)) < 0){
			if(errno = EINTR)
				continue;
			else
				printf("Error In Select.\n");
		}

		if(FD_ISSET(tcpfd, &rset))
			TCPChat(tcpfd);

		if(FD_ISSET(udpfd, &rset))
			UDPChat(udpfd);
	}

	// TCPChat(tcpfd);

	return 0;
}

