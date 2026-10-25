#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*#define PORTNUM 2300*/

int main(int argc, char *argv[])
{

	char msg[15]="Hello World!\n";

	struct sockaddr_in dest;
	struct sockaddr_in serv;
	int mysocket;
	unsigned int socksize=sizeof(struct sockaddr);
	
int PORTNUM;
if(argc !=2){
fprintf(stderr, "Usage: %s <port\n>", argv[0]);
exit(1);
}
PORTNUM = atoi(argv[1]);
	memset(&dest, 0, sizeof(dest));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port= htons(PORTNUM);

	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

	listen(mysocket,1);
	int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

	if (send(consocket, msg, strlen(msg),0)>0){
		printf("Incoming connection from %s - sending welcome \n", inet_ntoa(dest.sin_addr));
sleep(1);
}

	close(consocket);
	close(mysocket);
	exit(0);
}

