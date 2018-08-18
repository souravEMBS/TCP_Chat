#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(const char* msg) // error prints
{
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{
	/* code */

	int socketFd,portno,n;
	struct sockaddr_in serv_addr; //socket address
	struct hostent* server; // host
	char buffer[256];

	if(argc < 3){
		fprintf(stderr,"usage %s host port",argv[0]);
		exit(0);
	}
	portno  = atoi(argv[2]);

	socketFd = socket(AF_INET, SOCK_STREAM,0); // create cocket
	if(socketFd == -1)
		error("socket open error: ");

	server = gethostbyname(argv[1]); // returns hostent for a given host name
	if(server == NULL)
	{
		fprintf(stderr,"no host present by name %s ",argv[1]);
		exit(0);
	}

	bzero((char *)&serv_addr,sizeof(serv_addr));//fill with zero
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(socketFd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)// connect to the socket
		error("connection error... ");

	printf("chat with client\n");

	while(1)
	{
		bzero(buffer,sizeof(buffer));
		printf("client: ");
		fgets(buffer,sizeof(buffer)-1,stdin); //get chat input
		n = write(socketFd,(char *)buffer,sizeof(buffer)); //write to the socket
		if(n < 0)
			error("unable to write.. ");
		//printf("client: %s\n", buffer);
		bzero(buffer,sizeof(buffer));

		n = read(socketFd,buffer,sizeof(buffer));//read from socket
		if(n<0)
			error("unable to read.. ");
		printf("server: %s\n",buffer);
		int i  = strncmp("bye",buffer,3);
		if(i == 0)
			break;
	}
	close(socketFd);
	return 0;
}