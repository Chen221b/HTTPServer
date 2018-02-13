#define ADDRLEN 20
#define INT_LEN 300

int TCPChat(int lfd)
{
	char requestStr[INT_LEN], addrStr[ADDRLEN];
	struct sockaddr_in claddr;
	int addrlen, cfd, readNum;
	pid_t childPID;

	for(;;){
		addrlen = sizeof(claddr);
		cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
		if(cfd < 0){
			if(errno == EINTR)
				continue;
			else
				printf("Accept error.\n");sleep(1);
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
	}
	return 0;
}

int UDPChat(int lfd)
{
	char requestStr[INT_LEN], addrStr[ADDRLEN];
	struct sockaddr_in claddr;
	int addrlen, readNum;

	for(;;){
		addrlen = sizeof(claddr);
		readNum = recvfrom(lfd, requestStr, sizeof(requestStr) - 1, 0, (struct sockaddr *) &claddr, &addrlen);
		requestStr[readNum + 1] = '\0';
		printf("%20s From: %-30s %-10d\n",
			requestStr, inet_ntop(AF_INET, &claddr.sin_addr, addrStr, sizeof(addrStr)), ntohs(claddr.sin_port));
	}
	return 0;
}
