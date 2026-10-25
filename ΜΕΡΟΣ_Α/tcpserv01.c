#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>  
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#define LISTENQ 5
#define SERV_PORT 2025
#define MAXLINE 50
#define max(a,b) ((a) > (b) ? (a) : (b))

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
void
str_mpla_mpla_server(int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];
    printf("Chat session started. You are the Server (User B).\n");
    
	FD_ZERO(&rset);
    
	for ( ; ; ) {
		FD_SET(fileno(stdin), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(stdin), sockfd) + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {
			ssize_t n;
			if ( (n = recv(sockfd, recvline, MAXLINE, 0)) == 0) {
				err_sys("str_mpla_mpla_server: client terminated prematurely");
            }
            recvline[n] = 0;
            printf("Client (User A): %s", recvline); 
		}

		if (FD_ISSET(fileno(stdin), &rset)) {
			if (fgets(sendline, MAXLINE, stdin) == NULL)
				return;
			send(sockfd, sendline, strlen(sendline), 0);
		}
	}
}


int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

		if ( (childpid = fork()) == 0) {
			close(listenfd);
			str_mpla_mpla_server(connfd);
			exit(0);
		}
		close(connfd);
	}
}