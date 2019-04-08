#include <stdio.h>
#include <stdlib.h>
//#include "functions.h"

void sub();
void add();
int main(int argc, int ** argv)
{
	int a[4];
	int b[4];
	int c[4];
        a[0] = atoi(argv[1]);
        a[1] = atoi(argv[2]);
        a[2] = atoi(argv[3]);
        a[3] = atoi(argv[4]);
        b[0] = atoi(argv[5]);
        b[1] = atoi(argv[6]);
        b[2] = atoi(argv[7]);
        b[3] = atoi(argv[8]);
  	add(a, b);
	printf("\n");
	sub(a, b);
	return 0;
}

void add(int *a, int *b)
{
	int c[4];
	for(int i = 0; i < 4; i++)
	{
		c[i] = a[i] + b[i];
		printf("%d ", c[i]);
	}
}

void sub(int *a, int *b)
{
        int c[4];
        for(int i = 0; i < 4; i++)
        {
                c[i] = a[i] - b[i];
                printf("%d ", c[i]);
        }
}



