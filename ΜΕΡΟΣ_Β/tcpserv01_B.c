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
#define MAX_CLIENTS 2
#define USERNAME_LEN 20

#define max(a,b) ((a) > (b) ? (a) : (b))

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
struct client_info {
    int fd;
    char username[USERNAME_LEN];
};

void chat_server_select(int listenfd)
{
    int i, maxi, maxfd, sockfd;
    int nready, connfd;
    char buf[MAXLINE];
    char send_msg[MAXLINE + USERNAME_LEN + 3];
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    struct client_info clients[MAX_CLIENTS] = {0}; 

    fd_set rset, allset;
    for (i = 0; i < MAX_CLIENTS; i++)
        clients[i].fd = -1; 
    maxi = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;

    printf("Server listening for %d clients...\n", MAX_CLIENTS);

    for ( ; ; ) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset)) { 
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

            if (connfd < 0) {
                err_sys("accept error");
                continue;
            }
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].fd < 0) {
                    clients[i].fd = connfd;
                    break;
                }
            }

            if (i == MAX_CLIENTS) { 
                printf("Maximum clients reached. Connection rejected.\n");
                close(connfd);
            } else {
                printf("New client connected (FD %d). Waiting for username...\n", connfd);
                FD_SET(connfd, &allset); 
                if (connfd > maxfd)
                    maxfd = connfd;

                if (i > maxi)
                    maxi = i;
            }
            if (--nready <= 0)
                continue; 
        }
        for (i = 0; i <= maxi; i++) { 
            if ( (sockfd = clients[i].fd) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset)) {
                ssize_t n;
                
                if (clients[i].username[0] == 0) {
                    n = recv(sockfd, buf, USERNAME_LEN - 1, 0);
                    if (n > 0) {
                        buf[n-1] = 0;
                        strncpy(clients[i].username, buf, USERNAME_LEN - 1);
                        printf("Client (FD %d) set username: %s\n", sockfd, clients[i].username);
                        int other_client_index = (i == 0) ? 1 : 0;
                        if (clients[other_client_index].fd != -1) {
                            sprintf(send_msg, "\n--- %s joined the chat! ---\n", clients[i].username);
                            send(clients[other_client_index].fd, send_msg, strlen(send_msg), 0);
                        }
                        
                    } else if (n == 0) {
                        goto disconnect;
                    }
                } else {
                    if ( (n = recv(sockfd, buf, MAXLINE, 0)) == 0) {
                        disconnect:
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        printf("Client %s (FD %d) disconnected.\n", clients[i].username, sockfd);
                        int other_client_index = (i == 0) ? 1 : 0;
                        if (clients[other_client_index].fd != -1) {
                             sprintf(send_msg, "\n--- %s left the chat. ---\n", clients[i].username);
                             send(clients[other_client_index].fd, send_msg, strlen(send_msg), 0);
                        }
                        clients[i].fd = -1;
                        clients[i].username[0] = 0;

                    } else {
                        buf[n] = 0;
                        printf("Message from %s: %s", clients[i].username, buf);
                        int other_client_index = (i == 0) ? 1 : 0;
                        int peer_fd = clients[other_client_index].fd;

                        if (peer_fd != -1) {
                            sprintf(send_msg, "[%s]: %s", clients[i].username, buf);
                            send(peer_fd, send_msg, strlen(send_msg), 0);
                        } else {
                            sprintf(send_msg, "Server: Only you are connected currently.\n");
                            send(sockfd, send_msg, strlen(send_msg), 0);
                        }
                    }
                }
                if (--nready <= 0)
                    break; 
            }
        }
    }
}


int
main(int argc, char **argv)
{
	int					listenfd;
	struct sockaddr_in	servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	chat_server_select(listenfd);

	exit(0);
}