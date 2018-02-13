#define ADDRLEN 20
#define SERVER_SOCKET 0
#define CLIENT_SOCKET 1

int getTCPSocket(int mode)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int optval, lfd, cfd;
	char addrStr[ADDRLEN];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	getaddrinfo(NULL, PORT_NUM, &hints, &result);

	if(mode == SERVER_SOCKET){
		optval = 1;
		for(rp = result; rp != NULL; rp = rp->ai_next){
			lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if(lfd == -1)
				continue;
			setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
			if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0){
				printf("TCP Bind: %-30s\n",
					inet_ntop(AF_INET, &rp->ai_addr, addrStr, sizeof(addrStr)));
				freeaddrinfo(result);
				return lfd;
			}
			close(lfd);
		}
	}else if(mode == CLIENT_SOCKET){
		for(rp = result; rp != NULL; rp = rp->ai_next){
			cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if(cfd == -1)
				continue;
			if(connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1){
				printf("TCP Connect: %-30s\n",
					inet_ntop(AF_INET, &rp->ai_addr, addrStr, sizeof(addrStr)));
				freeaddrinfo(result);
				return cfd;
			}
			close(cfd);
		}
	}

	freeaddrinfo(result);
	return -1;
}

int getUDPSocket(int mode)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;	
	int lfd;
	char addrStr[ADDRLEN];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	getaddrinfo(NULL, PORT_NUM, &hints, &result);

	if(mode == SERVER_SOCKET){
		for(rp = result; rp != NULL; rp = rp->ai_next){
			lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if(lfd == -1)
				continue;
			if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0){
				printf("UDP Bind: %-30s\n",
					inet_ntop(AF_INET, &rp->ai_addr, addrStr, sizeof(addrStr)));
				freeaddrinfo(result);
				return lfd;
			}
			close(lfd);
		}
	}else if(mode == CLIENT_SOCKET){
		for(rp = result; rp != NULL; rp = rp->ai_next){
			lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if(lfd == -1)
				continue;
			if(connect(lfd, rp->ai_addr, rp->ai_addrlen) != -1){
				printf("UDP Connect: %-30s\n",
					inet_ntop(AF_INET, &rp->ai_addr, addrStr, sizeof(addrStr)));
				freeaddrinfo(result);
				return lfd;
			}
			close(lfd);
		}
	}

	freeaddrinfo(result);
	return -1;
}
