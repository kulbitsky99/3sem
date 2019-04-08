#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

enum CONSTANTS
{
 	PORT_ADDR = 52000
};

int main (int argc, char *argv[]) {
 int sockfd;
 int n = 0, i = 0, len = 0;
 
 char sendline[1000] = "", recvline[1000] = "";
 struct sockaddr_in servaddr;

 if (argc != 2)
 {
 printf("insert IP!\n");
 exit(-1);
 }

 if ( (sockfd = socket(PF_INET, SOCK_STREAM, 0) ) < 0 ) 
 {
 perror("socket");
 exit(-1);
 }
 
 
 
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(PORT_ADDR);
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

 if ( inet_aton(argv[1], &servaddr.sin_addr) == 0 ) {
 printf("Invalid IP address\n");
 close(sockfd);
 exit(-1);
 }


 if( connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0 ) {
 perror("bind");
 close(sockfd);
 exit(-1);
 }

 for(i = 0; i < 3; i++)
 {
 printf("Insert string in the following manner:'a+b ' ");
 fflush(stdin);
 fgets(sendline, 1000, stdin);
 
 if ((n = write(sockfd, sendline, strlen(sendline) + 1)) < 0) {
 perror("send");
 exit(-1);
 }
 if((n = read(sockfd, recvline, 999)) < 0)
 {
 	perror("Can't read\n");
	close(sockfd);
	exit(1);
 }
 printf("result = %s\n", recvline);
 }
 close(sockfd);

 return 0;
}


