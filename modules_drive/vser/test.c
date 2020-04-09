#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#include "vser.h"

int main(int argc, char *argv[])
{
	int fd;
	int ret;
	unsigned int baud;

	struct option opt = {8,1,1};

	fd = open("/dev/vser0",O_RDWR);
	if(fd ==  -1)
	{
		goto fail;
	}

	baud = 9600;
	ret = ioctl(fd, VS_SET_BAUD,baud);
	if(ret == -1)
	{
		goto fail;
	}

	ret = ioctl(fd, VS_GET_BAUD,baud);
	if(ret == -1)
	{
		goto fail;
	}

	ret = ioctl(fd, VS_SET_FFMT,&opt);
	if(ret == -1)
	{
		goto fail;
	}

	ret = ioctl(fd, VS_GET_FFMT,&opt);
	if(ret == -1)
	{
		goto fail;
	}

	printf("baud=%d\n",baud);

	close(fd);

fail:
	perror("ioctl test");
	exit(EXIT_FAILURE);

	return 0;
}
