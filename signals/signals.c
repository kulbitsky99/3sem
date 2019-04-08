#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

enum CONSTANTS
{
	PHRASE_SIZE = 25
};


int   	fd[2];
int 	checker = 1;
pid_t 	parent_id = 0;
pid_t 	child_id = 0;


void child_handler()
{

	char phrase[PHRASE_SIZE];
	size_t size = 0;

	if ((size = read(fd[0], phrase, PHRASE_SIZE + 1)) < 0)
	{
		printf("Reading error!\n");
		exit(-1);
	}

	printf("In child: %s\n", phrase);
	close(fd[0]);
	close(fd[1]);
	checker--;
}	

void parent_handler()
{
	char phrase[PHRASE_SIZE];
	size_t size = read(fd[0], phrase, PHRASE_SIZE);
	if (size < 0)
	{
		perror("read");
		exit(-1);				
	}
	printf("In parent: %s\n", phrase);
	write(fd[1], "Good morning from parent!", PHRASE_SIZE);
	int sig = kill(child_id, SIGUSR1); 
	if (sig == -1)
	{
		printf("Killing error!\n");
		exit(-5);
	}
	close(fd[0]);
	close(fd[1]);
	checker--;
}
	

int main()
{
	pid_t pid = 0;
	char phrase[PHRASE_SIZE] = "";

	if (pipe(fd) < 0)
	{
		printf("Can't open pipe!\n");
		exit(-1);
	}

	pid = fork();
	if(pid < 0)
	{
		printf("Fork error!\n");
		exit(-2);
	}
	if (pid == 0) 
	{

		signal(SIGUSR1, child_handler);	
		write(fd[1], "Good morning from child!", PHRASE_SIZE);

		child_id = getpid();
		parent_id = getppid();

		int sig  = kill(parent_id, SIGUSR2);
		if (sig == -1)
		{
			printf("Kill error!\n");
			exit(-3);
		}
		while(checker);

	}
	else 
	{
		signal(SIGUSR2, parent_handler);
		child_id = pid;
		while(checker);
	}

	return 0;
}
