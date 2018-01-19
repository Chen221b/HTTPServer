#define _BSD_SOURCE

#include <netdb.h>
#include "is_seqnum.h"

#define BACKLOG 50

ssize_t readLine(int fd, void *buffer, size_t n);

int main(int argc, char const *argv[])
{
	uint32_t seqNum;
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

	seqNum = 0;

	signal(SIGPIPE, SIG_IGN);	//SIG_IGN ignore the signal and return 1

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

		if(cfd == -1)
			continue;
		if(getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			snprintf(addrStr, ADDRSTRLEN, ("%s, %s"), host, service);
		else
			snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
		printf("Connection from %s\n", addrStr);
		if(readLine(cfd, reqLenStr, INT_LEN) <= 0){
			close(cfd);
			continue;
		}
		reqLen = atoi(reqLenStr);
		if(reqLen <= 0){
			close(cfd);
			continue;
		}
		snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
		if(write(cfd, &seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
			fprintf(stderr, "Error on write");
		seqNum += reqLen;
	}

	return 0;
}

ssize_t readLine(int fd, void *buffer, size_t n)
{
	ssize_t numRead;
	size_t totRead;
	char* buf;
	char ch;

	buf = buffer;
	totRead = 0;
	for(;;){
		numRead = read(fd, &ch, 1);
		if(numRead == -1){
			if(errno == EINTR)
				continue;
			else 
				return -1;
		}else if(numRead == 0){
			if(totRead == 0)
				return 0;
			else
				break;
		}else{
			if(totRead < n - 1){
				totRead++;
				*buf++ = ch;
			}
			if(ch == '\n')
				break;
		}
	}
	*buf = '\0';
	return totRead;
}