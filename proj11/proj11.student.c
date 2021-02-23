#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iomanip>
#include <iostream>

using namespace std;

#define BSIZE 128

int main(int argc, char* argv[]){


	if (argc != 4)
	{
		cout << argc << endl;
		cout << "Usage: <host> <port> <file_name>" << endl;
		exit(1);
	}

	char * hostname = argv[1];
	unsigned short int port = atoi(argv[2]);
	char * message = argv[3];
	//cout << message << endl;
	//cout << port << endl;
	int sd = socket( AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		perror( "socket" );
		exit( 2 );
	}
	
	struct hostent *server = gethostbyname( hostname );
	if ( server == NULL)
	{
		printf( "Error: no such host %s\n", hostname );
		exit( 3 );
	}
	
	struct sockaddr_in saddr;
	
	bzero( &saddr, sizeof(saddr) );
	saddr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&saddr.sin_addr.s_addr, server->h_length );
	saddr.sin_port = htons( port );

	
	int stat = connect( sd, (struct sockaddr*) &saddr, sizeof(saddr) );
	if (stat < 0)
	{
		perror( "connect" );
		exit( 4 );
	}
	
	//printf( "Client connected on fd %d\n\n", sd );
	
	char buffer[BSIZE];
	bzero( buffer, BSIZE );
	for(int i = 0; i < strlen(message); i++)
	{
		//printf("%c", message[i]);
		buffer[i] = message[i];
	}
	//printf("\n");
	//fgets(buffer, BSIZE, message );
	
	int nsend = send( sd, buffer, strlen(buffer), 0 );
	if (nsend < 0)
	{
		perror( "send" );
		exit( 5 );
	}
	//printf( "client send %d bytes\n", nsend);
	//printf("Message: %s\n", buffer);
	
	bzero( buffer, BSIZE );
	int nrecv = recv( sd, buffer, BSIZE, 0 );
	if (nrecv < 0)
	{
		perror( "recv" );
		exit( 4 );
	}
	
	//printf("Client Recieved %d bytes\n", nrecv );
	//printf( "Message: %s\n", buffer );

		
	char *s;
	s = strchr(buffer, 'S');
	char *condition = s;

	if(condition != NULL)
	{
		//cout << "sucess" << endl;
		char proceed[7] = {'P','R','O','C','E','E','D'};
		bzero( buffer, BSIZE );

		//cout << "sending" << endl;
		
		int nsend = send( sd, proceed, strlen(proceed), 0 );
		if (nsend < 0)
		{
			perror( "send" );
			exit( 5 );
		}
		//printf( "client send %d bytes\n", nsend);
		//printf("Message: %s\n", proceed);
	
		//bzero( buffer, BSIZE );
		
		while(nrecv != 0)
		{
			bzero( buffer, BSIZE );
			int nrecv = recv( sd, buffer, BSIZE, 0 );
			if (nrecv < 0)
			{
				perror( "recv" );
				exit( 4 );
			}
			if (nrecv == 0)
			{
				break;
			}
	
			write(1, buffer, BSIZE);
		}
	
	}
	
	
	
	
	
    
	return 0;
}
