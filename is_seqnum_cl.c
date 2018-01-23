#include <netdb.h>
#include "is_seqnum.h"

ssize_t readLine(int fd, void *buffer, size_t n);

int main(int argc, char const *argv[])
{
	char* reqLenStr;
	char seqNumStr[INT_LEN];
	int cfd;
	ssize_t numRead;
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;

	getaddrinfo(argv[1], PORT_NUM, &hints, &result);

	for(rp = result; rp != NULL; rp = rp->ai_next){
		cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(cfd == -1)
			continue;
		if(connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;
		close(cfd);
	}

	freeaddrinfo(result);
	reqLenStr = (argc > 2) ? argv[2] : "1";
	write(cfd, reqLenStr, strlen(reqLenStr));
	write(cfd, "\n", 1);
	numRead = readLine(cfd, seqNumStr, INT_LEN);
	printf("Sever Content:\n%s", seqNumStr);

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