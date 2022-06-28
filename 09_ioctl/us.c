#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl.h"
int main(int argc, const char *argv[])
{
	int fd;
	fd = open("/dev/THANOS", O_RDWR);
	if(!fd)
	{
		perror("Error in opening file\n");
		return -1;
	}

	printf("fd = %d\n", fd);

	ioctl(fd, CMD_1);
	ioctl(fd, CMD_2, 9);

	close(fd);
	return 0;
}
