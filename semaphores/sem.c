#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef int myType;

typedef struct
        {
                enum {false, true} choosing;
                int number;
		int m;
        }DATA;



int max(DATA * d, int dim)//!!!!!!!!!!!!!!
{
	int result;
	result = d -> number;
	for(int i = 1; i < dim; i++)
	{
		if(((d + i) -> number) > result)
			result = (d + i) -> number;
	}
	return result;
}

int main()
{
	
	FILE * op;
	FILE * cl;
	if((op = fopen("data.txt", "r")) == NULL)
	{
		printf("Can't open data.txt\n");
		exit(-1);
	}
        if((cl = fopen("result.txt", "w")) == NULL)
        {
                printf("Can't open result.txt\n");
                exit(-1);
        }
	int dim, semid, shmid;
	int j = 0;
	myType rubbish = 0;

	fscanf(op, "%d", &dim);
	DATA* data = (DATA*)calloc(dim, sizeof(*data));
	struct sembuf *sops = (struct sembuf*)calloc(1, sizeof(struct sembuf));
	
	myType *vect = (myType *)calloc(dim, sizeof(myType));
	myType *vect_result = (myType *)calloc(dim, sizeof(myType));
	myType *elem = (myType *)calloc(dim, sizeof(myType));
	
        if((shmid = shmget(IPC_PRIVATE, dim * sizeof(DATA), 0666 | IPC_CREAT)) < 0)
        {
                printf("Can't give shared memory!\n");
                exit(-2);
        }
        if((data = (DATA *)shmat(shmid, NULL, 0)) == (DATA *)(-1))
        {
                printf("Can't attach shared memory!\n");
                exit(-3);
        }
        if((semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) < 0)
        {
                printf("Can't give semaphores!\n");
                exit(-2);
        }

	for(int i = 0; i < dim; i++)
	{
		fscanf(op, "%d", &(vect[i]));
	}
	pid_t pid = getpid();
	data -> m = 0;
	if(pid != 0)
	for(int i = 0; i < dim; i++)
	{
		pid = fork();
		if(pid == 0)//if(some condition)
		{
			(data + i) -> choosing = true;
			(data + i) -> number = max(data, dim) + 1;
                        (data + i) -> choosing = false;
			//printf("my pid1 = %d\n", getpid());
			for(j = 0; j < dim; j++)
			{
				while((data + j) -> choosing);
				//printf("my pid2 = %d\n", getpid());
				while(((data + j) -> number) && ((((data + j) -> number) < ((data + i) -> number)) || ( ((((data + j) -> number) == ((data + i) -> number))) && (j < i))));
				//printf("my pid3 = %d\n", getpid());

			}
			//printf("my pid4 = %d\n", getpid());
			for(int l = 0; l < dim * (data -> m); l++)//critical section
				fscanf(op, "%d", &rubbish);

			(data -> m)++;
			myType res = 0;//critical section
		        for(int k = 0; k < dim; k++)
		        {
               			fscanf(op, "%d", &(elem[k]));
				res += elem[k] * vect[k];
		       	}
			
			sops -> sem_op = (data -> m) - 1;
                        sops -> sem_flg = 0;
                        sops -> sem_num = 0;
                        if(semop(semid, sops, 1) < 0)
                        {
                                printf("Can't wait for condition\n");
                                exit(-4);
                        }

                        
                        fprintf(cl, "%d\n", res);
                        sops -> sem_op = (data -> m);
                        sops -> sem_flg = 0;
                        sops -> sem_num = 0;
                        if(semop(semid, sops, 1) < 0)
                        {
                                printf("Can't wait for condition\n");
                                exit(-4);
                        }


                        if((data -> m) == dim)
                                (data -> m)++;

			(data + i) -> number = 0;
		        exit(0);
		
			
		}
	}	
	while((data -> m) != dim + 1);
        if(shmdt(data) < 0)
	{       
                printf("Can't detach shared memory\n");
                exit(-5);
        }
	semctl(semid, 0, IPC_RMID, NULL);
	shmctl(shmid, IPC_RMID, NULL);
       
	fclose(op);
	fclose(cl);
	return 0;
}

