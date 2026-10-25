#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1000

int main(int argc, char **argv){
int sockfd, n;
char recvline[MAXLINE -1];
struct sockaddr_in servaddr;

if (argc !=2){
printf("Usage: %s <ipaddress>", argv[0]);
exit(0);
}

if ((sockfd = socket(AF_INET, SOCK_STREAM,0))<0)
printf("Socket error");
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(2020);

if (inet_pton (AF_INET, argv[1], &servaddr.sin_addr)<=0)
printf("inet_pton error for %s", argv[1]);

if (connect (sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)
printf("Connect error");

while ((n=read(sockfd,recvline, MAXLINE))>0){
recvline[n]='\0';
if(fputs(recvline,stdout)==EOF)
printf("pputs error");
}

if(n<0)
printf("read error\n");
exit(0);
}
