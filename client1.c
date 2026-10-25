#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXRCVLEN 500
/*#define PORTNUM 2300*/

int main(int argc, char *argv[])
{
	char buffer[MAXRCVLEN + 1];
	int len, mysocket;
	struct sockaddr_in dest;
	int PORTNUM;
	if(argc!=3){
		fprintf(stderr, "Usage: %s <ip_address> <port>\n", argv[0]);
		exit(1);
	}
	
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
if(inet_aton(argv[1], &dest.sin_addr)==0){
fprintf(stderr, "Invalid IP address '%s'\n", argv[1]);
exit(1);
}

PORTNUM = atoi(argv[2]);

if(PORTNUM <= 1024 || PORTNUM > 65535){
fprintf(stderr, "Port number %d is out of range.\n", PORTNUM);
exit(1);
}


	/*dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);*/
	dest.sin_port = htons(PORTNUM);

	connect(mysocket, (struct sockaddr * )&dest, sizeof(struct sockaddr));
	len = recv(mysocket, buffer, MAXRCVLEN, 0);

if(len>0){
	
	buffer[len]='\0';
	printf("REceived %s(%d bytes).\n", buffer, len);
} else {
printf("Received (\\%d bytes) Connection closed or error\\n",len);
}
	close(mysocket);
	exit(0);
}
