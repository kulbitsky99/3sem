#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int main()
{
	int *array;
	int shmid;
	int new = 1;
	char pathname[] = "data.txt";
	key_t key;
	if((key = ftok(pathname, 0)) < 0)
	{
		printf("Can't generate key");
	}
	if((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if(errno != EEXIST)
		{
			printf("Can't create shared memory\n");
			exit(-1);
		}
		else
		{
			if((shmid = shmget(key, 3 * sizeof(int), 0)) < 0)
			{
				printf("Can't find shared memory\n");
				exit(-2);
			}
			new = 0;
		}
	}
	if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
	{
		printf("Can't attach shared memory!\n");
		exit(-3);
	}
	if(new)
	{
		array[0] = 1;
		array[1] = 0;
		array[2] = 1;
	}
	else
	{
		array[0]++;
		array[2]++;
	}
	printf("Program 1 was started %d times, Program 2 - %d times, total - %d times!\n", array[0], array[1], array[2]);
	if(shmdt(array) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-4);
	}

}
