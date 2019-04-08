#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

struct property {
	pthread_t  thread;
	int number;
	int semid;
};

void mythid();


int main(int argc, char **argv)
{

	int i = 0, semid = 0;
	char name_of_file[] = "threads.c";
	char * argument = NULL;
	key_t key;

	long int amount = strtol(argv[1], &argument, 10);
		
	struct property * data = malloc((amount+1)*sizeof(struct property));
	
	if((key = ftok(name_of_file, 0)) < 0)
	{
		printf("Can't generate key to the volume of semaphores\n");
		exit(-1);
	}
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
	{
		printf("Opening of volume of semaphores failed!\n");
		exit(-1);
	}
	
	for (i = 0; i < amount ; i++)
	{
		data[i].semid = semid;
		data[i].number = i+1;
		pthread_create(&data[i].thread, NULL, (void*) &mythid, (void*) &data[i]);
	}
	
	for (i = 0; i < amount;  i++)
	{
		pthread_join(data[i].thread, NULL);
	}

	semctl(semid, 0, IPC_RMID, 0);

	return 0;
}


void mythid(void * data)
{
        struct sembuf sops;
        struct property * feature = (struct property*) data;
        int id = feature -> number;
        int semid = feature -> semid;

        if (id == 1)
        {
                sops.sem_op = 1;
                sops.sem_num = 0;
                sops.sem_flg = 0;
                if(semop(semid, &sops, 1) < 0)
                {
                        printf("Can't wait for condition!\n");
                }
        }

        sops.sem_op = -id;
        sops.sem_flg = 0;
        sops.sem_num = 0;

        if(semop(semid, &sops, 1) < 0)
	{
                printf("Can't wait for condition!\n");
        }

        printf("thread %i\n", id);

        sops.sem_op = id+1;
        sops.sem_num = 0;
        sops.sem_flg = 0;

        if(semop(semid, &sops, 1) < 0)
	{
                printf("Can't wait for condition!\n");
        }
}

