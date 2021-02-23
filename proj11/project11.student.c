#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/type.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <neddb.h>

int main(aint argc, char* argv[]){

	ifstream in_file;
	string inf

	

	if (argc != 3)
	{
		cout << "Usage: <host> <port> <file_name>" << endl;
		exit(1);
	}

	char * hostname = argv[1];
	unsigned short int port = atoi(argv[2]);
	
	int sd = socket( AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		perror( "socket" );
		exit( 2 );
	}
	
	struct hostent * server = gethostbyname( hostname );
	if ( server == NULL)
	{
		printf( "Error: no such host %s\n", hostname );
		exit( 3 );
	}
	
    
	return 0;
}
