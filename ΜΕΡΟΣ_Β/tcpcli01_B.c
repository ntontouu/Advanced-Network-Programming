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
#define USERNAME_LEN 20

#define max(a,b) ((a) > (b) ? (a) : (b))

void err_quit(const char* x) 
{ 
    perror(x); 
    exit(1); 
}


void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];
    char username[USERNAME_LEN];
    printf("Please enter your username: ");
    if (fgets(username, USERNAME_LEN, stdin) == NULL) {
        printf("No username provided. Exiting.\n");
        return; 
    }
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\n';
    }
    send(sockfd, username, strlen(username), 0);
    printf("\nWelcome %s. Chat session started.\n", username);
    printf("-----------------------------------------\n");

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {
			ssize_t n;
			if ( (n = recv(sockfd, recvline, MAXLINE,0)) == 0)
				err_quit("str_cli: server terminated prematurely");
            
            recvline[n] = 0;
            fputs(recvline, stdout); 
		}

		if (FD_ISSET(fileno(fp), &rset)) {
			if (fgets(sendline, MAXLINE, fp) == NULL)
				return;
                
			send(sockfd, sendline, strlen(sendline),0);
		}
	}
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);

	exit(0);
}