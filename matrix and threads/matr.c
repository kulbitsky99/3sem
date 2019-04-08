#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct arg
{
	pthread_t thread;
	int **MATRIX1;
	int **MATRIX2;
	int **RESULT;
	int i;
	int j;
	int N;
};

void mythid(void * attr)
{
	int k = 0;
	struct arg * help = (struct arg*) attr;
	help -> RESULT[help -> i][help -> j] = 0;
	for(k = 0; k < help -> N; k++)
		help -> RESULT[help -> i][help -> j] += help -> MATRIX1[help -> i][k] * help -> MATRIX2[k][help -> j];
}     

int main()
{	
	
	int **MATRIX1, **MATRIX2,**RESULT;
	int i, j, N;
	
	FILE * in = fopen("matrix.txt", "r");
	fscanf(in,"%d", &N);

	MATRIX1 = malloc(N * sizeof(int*));
	MATRIX2 = malloc(N * sizeof(int*));
	RESULT = malloc(N * sizeof(int*));
	struct arg ** POINTERS = malloc(N * sizeof(struct arg *));

	for (i = 0; i < N; i++)
	{
		MATRIX1[i] = malloc(N * sizeof(int));
		MATRIX2[i] = malloc(N * sizeof(int));
		POINTERS[i] = malloc(N * sizeof(struct arg));
		RESULT[i] = malloc(N * sizeof(int));
	}
	for(i = 0; i < N; i++)
	{	
		for(j = 0; j < N; j++)
			fscanf(in,"%d", &MATRIX1[i][j]);
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
			fscanf(in,"%d", &MATRIX2[i][j]);
        }  
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			POINTERS[i][j].MATRIX1 = MATRIX1;
			POINTERS[i][j].MATRIX2 = MATRIX2;
			POINTERS[i][j].RESULT = RESULT;
 			POINTERS[i][j].N = N;
			POINTERS[i][j].i = i;
			POINTERS[i][j].j = j;

			pthread_create(&POINTERS[i][j].thread, NULL, (void*)&mythid, (void *)&POINTERS[i][j]);
		}
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			int *ptr;
			pthread_join(POINTERS[i][j].thread, (void**)&ptr);
			printf("%d ", RESULT[i][j]);
		}
		printf("\n");
	}
		

return 0;
}
