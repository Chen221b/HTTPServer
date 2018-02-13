#include <netdb.h>
#include "lib.h"
#include "r_w_func.c"
#include "get_socket.c"

ssize_t readLine(int fd, void *buffer, size_t n);

int main(int argc, char const *argv[])
{
	char* requestStr;
	char seqNumStr[INT_LEN];
	int cfd;
	ssize_t numRead;

	cfd = getUDPSocket(1);

	snprintf(seqNumStr, sizeof(seqNumStr), "An UDP Message.");
	requestStr = seqNumStr;
	sendto(cfd, requestStr, strlen(requestStr), 0, NULL, 0);

	return 0;
}

