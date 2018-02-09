#define _BSD_SOURCE

#include <netdb.h>
#include "is_seqnum.h"
#include "r_w_func.c"

#define BACKLOG 50

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
	uint32_t seqNum;
	pid_t childPID;
	char reqLenStr[INT_LEN];
	char seqNumStr[INT_LEN];
	struct sockaddr_storage claddr;
	int lfd, cfd, optval, reqLen;
	socklen_t addrlen;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
	char addrStr[ADDRSTRLEN];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	void sig_child(int);
	int readNum;

	seqNum = 0;

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
		addrlen = sizeof(struct sockaddr_storage);
		cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
		if(cfd < 0)
		{
			if(errno == EINTR)
				continue;
			else
				printf("Accept error.\n");
		}

		// if((childPID = fork()) == 0)
		// {
		// 	close(lfd);
		// 	if(getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		// 		snprintf(addrStr, ADDRSTRLEN, ("%s, %s"), host, service);
		// 	else
		// 		snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
		// 	printf("Connection from %s\n", addrStr);
		// 	if(readLine(cfd, reqLenStr, INT_LEN) <= 0){
		// 		printf("Content is null\n");
		// 	}else{
		// 		printf("%s\n", reqLenStr);
		// 	}
		// 	snprintf(seqNumStr, sizeof(seqNumStr), "Server answer back\n");
		// 	write(cfd, &seqNumStr, strlen(seqNumStr));
		// 	exit(0);
		// }

		if((childPID = fork()) == 0)
		{
			close(lfd);
			while((readNum = read(cfd, seqNumStr, sizeof(seqNumStr) - 1)) > 0)
			{
				seqNumStr[readNum + 1] = '\0';
				printf("%s\n", seqNumStr);
			}
			exit(0);
		}
		
		close(cfd);
	}

	return 0;
}

