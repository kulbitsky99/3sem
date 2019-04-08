#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

enum CONSTANTS
{
 	PORT_ADDR = 51000
};

int main () {
 int sockfd;
 int n = 0, i = 0, len = 0;
 
 char sendline[1000] = "", recvline[1000] = "";
 struct sockaddr_un servaddr, cliaddr;

 if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0) ) < 0 ) 
 {
 perror("socket");
 exit(-1);
 }
 
 
 
 bzero(&cliaddr, sizeof(cliaddr));
 cliaddr.sun_family = AF_UNIX;
 strcpy(cliaddr.sun_path, "CL_SOCK");
 /*servaddr.sin_port = htons(PORT_ADDR);
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

 if ( inet_aton(argv[1], &servaddr.sin_addr) == 0 ) {
 printf("Invalid IP address\n");
 close(sockfd);
 exit(-1);
 }*/


 if( connect(sockfd, (struct sockaddr *) &cliaddr, SUN_LEN(&cliaddr)) < 0 ) {
 perror("bind");
 close(sockfd);
 exit(-1);
 }

 bzero(&servaddr, sizeof(servaddr));
 servaddr.sun_family = AF_UNIX;
 strcpy(servaddr.sun_path, "SERV_SOCK");


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


