#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd, num1,num2;

	printf("Welcome to Write App-1.\n");
	printf("Enter Two Number for Addition.\n");
	scanf("%d %d", &num1, &num2);
	int num[2] = {num1,num2};
	
	fd = open("/dev/WR1", O_RDWR, 0777);

	if (fd < 0)
	{
		printf("Error in opening file.\n");
		return -1;
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
