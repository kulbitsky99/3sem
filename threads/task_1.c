#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
	pthread_t thread = pthread_self();
	printf("%zu\n", thread);

	/*if(argc == 2)
	{

		int i = 0, n = atoi(argv[2]);
		for(i = 0; i < n; i++)
		{
		
		}
	}
	else
	{
		printf("Invalid input parameters!\n");
		exit(-1);
	}*/
	return 0;
}
