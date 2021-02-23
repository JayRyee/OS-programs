#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iomanip>
#include <iostream>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>

using namespace std;

#define BSIZE 128

int main()
{
	// Ask OS to create a socket
	int listen_sd = socket( AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		perror( "socket" );
		exit( 2 );
	}
	
	// Record curr used to comminicate with OS
	struct sockaddr_in curr;
	
	bzero( &curr, sizeof(curr));
	curr.sin_family = AF_INET;
	curr.sin_port = ntohs( 0 );
	curr.sin_addr.s_addr = htonl( INADDR_ANY );
	
	// Ask OS to bind socket to port on host
	int bstat = bind( listen_sd, (struct sockaddr *) &curr, sizeof(curr) );
	if( bstat < 0 )
	{
		perror( "bind" );
		exit( 3 );
	}
	
	char name[128];
	gethostname( name, 128 );
	
	struct hostent* host_name;
	host_name = gethostbyname( name );
	
	// Ask os to return socket information
	socklen_t curr_size = sizeof( curr );
	getsockname( listen_sd, (struct sockaddr *) &curr, &curr_size );
	
	printf("%s %d\n", host_name->h_name, ntohs(curr.sin_port) );
	
	int lstat = listen( listen_sd, 5);
	if( lstat < 0 )
	{
		perror( "listen" );
		exit( 4 );
	}
	
	 //printf( "Server listening on fd %d\n\n", listen_sd );
	
	while(1)
	{
		struct sockaddr_in caddr;
		unsigned int clen = sizeof(caddr);
		
		int comm_sd = accept( listen_sd, (struct sockaddr *) &caddr, &clen );
		if( comm_sd < 0 )
		{
			perror( "accept" );
			exit( 5 );
		}
		
		//printf( "Server connected on fd %d\n\n", comm_sd );
		
		struct hostent *host_info = gethostbyaddr(
			(const char *) &caddr.sin_addr.s_addr,
			sizeof(caddr.sin_addr.s_addr), AF_INET);
			
		if( host_info == NULL )
		{
			perror( "gethostbyaddr" );
			exit( 6 );
		}
		
		char * host = inet_ntoa( caddr.sin_addr );
		if( host == NULL )
		{
			perror( "inet_ntoa" );
			exit( 7 );
		}
		
		//printf( "Server connected to %s (%s)\n\n", host_name->h_name, host );
    	char buffer[BSIZE];
    	
    	bzero( buffer, BSIZE );
		int nrecv = recv( comm_sd, buffer, BSIZE, 0 );
		if (nrecv < 0)
		{
			perror( "recv" );
			exit( 8 );
		}
		
		int fd1 = open(buffer, O_RDONLY);
		//in_file.open(buffer);
		
		if( fd1 == -1 )
		{
			//printf("Unable to open file");
			bzero( buffer, BSIZE );
			char failure[7] = {'F','A','I','L','U','R','E'};
			int nsend = send( comm_sd, failure, strlen(failure), 0 );
			if (nsend < 0)
			{
				perror( "send" );
				exit( 9 );
			}
		}
		else
		{
			bzero( buffer, BSIZE );
			char success[7] = {'S','U','C','C','E','S','S'};
			int nsend = send( comm_sd, success, strlen(success), 0 );
			if (nsend < 0)
			{
				perror( "send" );
				exit( 10 );
			}
			
			bzero( buffer, BSIZE );
			int nrecv = recv( comm_sd, buffer, BSIZE, 0 );
			if (nrecv < 0)
			{
				perror( "recv" );
				exit( 11 );
			}
			
			char *s;
			s = strchr(buffer, 'P');
			char *condition = s;
			if( condition != NULL )
			{
				size_t bytes = sizeof(buffer);
				bzero( buffer, BSIZE );
				ssize_t bytes_r = read( fd1, buffer, bytes );

				while(bytes_r > 0)
				{
					nsend = send( comm_sd, buffer, bytes_r, 0 );
					if (nsend < 0)
					{
						perror( "send" );
						exit( 9 );
					}
					bzero( buffer, BSIZE );
					ssize_t bytes_r = read( fd1, buffer, bytes );
					if( bytes_r == 0)
					{
						break;
					}
					//cout << bytes_r << endl;
				}
				nsend = send( comm_sd, buffer, BSIZE, 0 );

				
			}
			
			
		}
		
		
			
		//printf("open file");
		int close_file = close(fd1);
		if( close_file < 0)
		{
			perror( "Could not close file" );
		}
		close(comm_sd);
		
	}
	
	
	

	return 0;
}
