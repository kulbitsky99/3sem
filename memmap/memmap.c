#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>

enum CONSTANTS
{
	MSG_SIZE = 1024
};

int main()

{
	struct mymesg
	{
		long mtype;
	}SIGN_CHILD, SIGN_PAR;

	SIGN_CHILD.mtype = 1;
	SIGN_PAR.mtype = 2;
	struct ch
	{
		char symb;
		char symb2;

	}*ptr, *tmpptr, *ptr_copy, *tmpptr_copy;	
	int fd1, fd2, mes_fd;
	size_t lenght;
	int i;
	char phrase_f[MSG_SIZE] = "I'm the message to be sended to the child!";
        char phrase_s[MSG_SIZE] = "I'm the message to be sended to the parent!";

     

	fd1 = open("mapped1.dat", O_RDWR | O_CREAT, 0666);
	if(fd1 == -1)
	{
		printf("Can't open file!\n");
		exit(-1);
	}
        fd2 = open("mapped2.dat", O_RDWR | O_CREAT, 0666);
        if(fd2 == -1)
        {
                printf("Can't open file!\n");
                exit(-1);
        }


	if((mes_fd = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		printf("Can't create new message queue\n");
		exit(-2);
	}

	lenght = MSG_SIZE * sizeof(struct ch);
        ftruncate(fd1, lenght);
	ftruncate(fd2, lenght);
        ptr = (struct ch *)mmap(NULL, lenght, PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
        tmpptr = (struct ch *)mmap(NULL, lenght, PROT_WRITE | PROT_READ, MAP_SHARED, fd2, 0);

        close(fd1);
	close(fd2);
        if(ptr == MAP_FAILED || tmpptr == MAP_FAILED)
        {
                printf("Mapping of the file failed!\n");
                exit(-3);
        }

	ptr_copy =  ptr;
	tmpptr_copy = tmpptr;

	pid_t number = fork();
	if(number < 0)
	{
		printf("Can't fork a child\n");
		exit(EXIT_FAILURE);
	}
	if(number == 0)
	{
		while(msgrcv(mes_fd, &SIGN_PAR, 0, SIGN_PAR.mtype, 0 ) < 0);

                for(i = 0; i < MSG_SIZE; i++)
                {
                        printf("%c", ptr_copy -> symb);
                        ptr_copy++;
                }
		printf("\n");

		for(int i = 0; i < MSG_SIZE; i++)
		{
			tmpptr_copy -> symb =  phrase_s[i];
			tmpptr_copy -> symb2 = tmpptr -> symb;
			tmpptr_copy++;
		}
		msgsnd(mes_fd, &SIGN_CHILD, 0, 0);
		exit(0);	
	}
	if(number > 0)
	{
                for(i = 0; i < MSG_SIZE; i++)
		{
                        ptr_copy -> symb =  phrase_f[i];
			ptr_copy -> symb2 = ptr -> symb;
			ptr_copy++;
		}
		msgsnd(mes_fd, &SIGN_PAR, 0, 0);
                while(msgrcv(mes_fd, &SIGN_CHILD, 0, SIGN_CHILD.mtype, 0 ) < 0);
                for(int i = 0; i < MSG_SIZE; i++)
                {
                        printf("%c", tmpptr_copy -> symb);
                        tmpptr_copy++;
                }

		printf("\n");

	}

	
	munmap((void *)ptr, lenght);
	munmap((void *)tmpptr, lenght);
	msgctl(mes_fd, IPC_RMID, NULL);
	return 0;

}



