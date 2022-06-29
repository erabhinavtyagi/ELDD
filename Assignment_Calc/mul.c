/*********** MULTIPLICATION APP *********************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	int num1,num2;

	printf("************ Welcome to Multiplication App **************\n");
	printf("Enter Two Number for Multiplication.\n");
	scanf("%d %d", &num1, &num2);
	int num[2] = {num1,num2};


	fd = open("/dev/MulDev", O_RDWR, 0777);

	if (fd < 0)
	{
		perror("Error in Opening File.\n");
		exit(EXIT_FAILURE);
	}	

	printf("Success in Opening File.\n");

	int result = write(fd, &num,sizeof(num));

	if (result < 0)
        {
                printf("Error in Writing Data from User to kernel.\n");
                return -1;
        }
	printf("Success in Writing Data from User to Kernel.\n");

	close(fd);
	return 0;
}
