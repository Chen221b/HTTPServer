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