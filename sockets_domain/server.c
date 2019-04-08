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
	DEPTH = 2, 
	PORT_ADDR = 51500
};

int main()
{
 int sockfd, newsockfd;
 int clilen, n, b;
 char line[1000] = "";
 char ch1[10] = "", ch2[10] = "";
 char * ch3;
 int result;
 struct sockaddr_un servaddr, cliaddr;
 
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sun_family = AF_UNIX;
 strcpy(servaddr.sun_path, "SERV_SOCK");
 /*servaddr.sin_port = htons(PORT_ADDR);
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);*/

 if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
 {
 perror("socket");
 exit(-1);
 }

 if(bind(sockfd, (struct sockaddr * ) &servaddr, SUN_LEN(&servaddr)) < 0 ) 
 {
 perror("bind");
 close(sockfd);
 exit(-1);
 }
 if(listen(sockfd, DEPTH) < 0)
 {
 	perror("listen");
	close(sockfd);
	exit(-1);
 }
 while(1) 
 {
 clilen = sizeof(struct sockaddr_un);

 if(((newsockfd = accept(sockfd, (struct sockaddr* ) &cliaddr, &clilen))) < 0)
 {
 	perror("accept");
	close(sockfd);
	exit(1);
 }
 pid_t pid = fork();
 if(pid == 0)
 {
 //printf("I'm here!"); 
 while ( ( n = read(newsockfd, line, 999) ) > 0 ) 
 {
 int i = 0;
 printf("line = %s\n", line);
 while(line[i] != '+')
 {
 	ch1[i] = line[i];
	i++;
 }
 i++;
 int j = 0;
 while(line[i] >= 48 && line[i] <= 57)
 {
//	printf("I was here!\n");
 	ch2[j] = line[i];
	i++;
	j++;
 }
 result = atoi(ch1) + atoi(ch2);
 //printf("%d %d\n", atoi(ch1), atoi(ch2));
 sprintf(ch3, "%d", result);
 //printf("%s\n", ch3);
 for(i = 0; i < 10; i++)
 {
 	ch1[i] = '\0';
	ch2[i] = '\0';
 }

 if((n = write (newsockfd, ch3, strlen(ch3) + 1)) < 0)
 {
 	perror("write");
	close(sockfd);
	close(newsockfd);
 }
 }
 if(n < 0)
 {
 	perror("read");
	close(sockfd);
	close(newsockfd);
	exit(1);
 }
 close(newsockfd);
 exit(0);
 }
 }
 
 return 0;
}
