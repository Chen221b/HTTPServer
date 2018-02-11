#define _BSD_SOURCE

#include <netdb.h>
#include "lib.h"
#include "r_w_func.c"

#define BACKLOG 50
#define ADDRLEN 20

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
	pid_t childPID;
	char requestStr[INT_LEN], addrStr[ADDRLEN];
	struct sockaddr_in claddr;
	int lfd, cfd, optval, reqLen, readNum;
	socklen_t addrlen;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	void sig_child(int);

	signal(SIGPIPE, SIG_IGN);	//SIG_IGN ignore the signal and return 1

	signal(SIGCHLD, sig_child);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	getaddrinfo(NULL, PORT_NUM, &hints, &result);

	optval = 1;
	for(rp = result; rp != NULL; rp = rp->ai_next){
		lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(lfd == -1)
			continue;
		setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
		close(lfd);
	}

	listen(lfd, BACKLOG);
	freeaddrinfo(result);

	for(;;){
		addrlen = sizeof(claddr);
		cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
		if(cfd < 0){
			if(errno == EINTR)
				continue;
			else{
				printf("Accept error.\n");
				sleep(1);
			}
		}

		if((childPID = fork()) == 0)
		{
			close(lfd);
			while((readNum = read(cfd, requestStr, sizeof(requestStr) - 1)) > 0)
			{
				requestStr[readNum + 1] = '\0';
				printf("%20s From: %-30s %-10d\n",
				 requestStr, inet_ntop(AF_INET, &claddr.sin_addr, addrStr, sizeof(addrStr)), ntohs(claddr.sin_port));
			}
			exit(0);
		}
		
		close(cfd);
	}

	return 0;
}

