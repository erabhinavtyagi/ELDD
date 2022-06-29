#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd, op_result;

	printf("Welcome to Read Multiply\n");

	fd = open("/dev/MulDev", O_RDWR, 0777);

	if (fd < 0)
	{
		printf("Error in opening file.\n");
		return -1;
	}
	
	printf("Success in Opening File.\n");
	
	int result = read(fd, &op_result,sizeof(op_result));
	
	if (result < 0)
        {
                printf("Error in Reading Data from kernel to User.\n");
                return -1;
        }
	printf("Success in Reading Data from Kernel to User.\n");

	close(fd);
	return 0;
}
