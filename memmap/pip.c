#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

enum CONSTOIDI
{
	PORTION = 5
};


int main(int argc, char ** argv)
{
	int size_of_data = 1, size_of_pipe_data = 1, size_buff = 1, OpenFlag = 0;
	char *buff_to_read = (char *)calloc( PORTION, sizeof(char));
	char *buff_to_write = (char *)calloc( PORTION, sizeof(char));
	int *fd = (int *)calloc(2, sizeof(int));
	if(pipe(fd) < 0)
		{
			printf("Can't work with pipe\n");
			exit(EXIT_FAILURE);
		}



	pid_t number = fork();
	if(number < 0)
	{
		printf("Can't fork a child\n");
		exit(EXIT_FAILURE);
	}

	if(number == 0)
	{
	OpenFlag = open(argv[1], O_RDONLY);
		if(OpenFlag < 0)
		{
			printf("Can't open a file\n");
			exit(EXIT_FAILURE);
		}

	}
	
	if(number > 0)
	{

		if(close(fd[1]) < 0)
		{
			printf("Can't close write stream");
		}
		while(size_of_pipe_data = read(fd[0], buff_to_write, PORTION) && size_of_pipe_data > 0)
		{
			
			printf("%s", buff_to_write);
		}
		if(close(fd[0]) < 0)
		{
			printf("Can't close read stream");
		}
	}


	if(number == 0)
	{
		if(close(fd[0]) < 0)
		{
			printf("Can't close read stream");
		}
		while(size_buff = read(OpenFlag, buff_to_read, PORTION) && size_buff > 0)
		{

		size_of_data = write(fd[1], buff_to_read, PORTION);
		if(size_of_data < 0)
		{
			printf("Can't write to pipe\n");
			exit(EXIT_FAILURE);
		}
		}
		if(close(fd[1]) < 0)
		{
			printf("Can't close write stream");
		}	
		if(close(OpenFlag) < 0)
		{
			printf("Can't close file stream");
		}
		wait(NULL);	
	}
	free(buff_to_read);
	free(buff_to_write);	

	return 0;	
}
