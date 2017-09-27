#define _BSD_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>		/*for pid*/
#include <unistd.h>			/*for pid*/


#define MSG_BUFF_SIZE (4096)

#define ARGIP 1
#define ARGPORT 2
#define ARG_USER_NAME 3
#define ARG_GROUP_NAME 4

int main(int argc, char *argv[])
{
	int sock;
	socklen_t addrlen;
	struct sockaddr_in addr;
	struct ip_mreq mreq;
	char msgbuf[ MSG_BUFF_SIZE ] = {0};
	int yes=1;     
	FILE* fdListen;
	
	fdListen = fopen("./fileForListen","w");
 	fprintf(fdListen,"%d",getpid() );
	fclose(fdListen);
	
     /* create what looks like an ordinary UDP socket */
	if (( sock = socket( AF_INET, SOCK_DGRAM ,0 ) ) < 0)
	{
		perror("socket");
		return 0;
	}

	if (setsockopt( sock,SOL_SOCKET, SO_REUSEADDR ,&yes,sizeof(yes) ) < 0) 
	{
		perror("setsockopt");
		return 0;
	}

     /* set up destination address */
     memset( &addr, 0, sizeof(addr) );
     addr.sin_family = AF_INET;
     addr.sin_addr.s_addr = inet_addr( argv[ ARGIP ]); 
     addr.sin_port = htons( atoi(argv[ ARGPORT ]) ); 
     
	/* bind to receive address */
	if (bind( sock,(struct sockaddr *)&addr,sizeof(addr) ) < 0) 
	{
		perror("bind");
		return 0;
	}
	
	mreq.imr_multiaddr.s_addr = inet_addr( argv[ ARGIP ] );
	mreq.imr_interface.s_addr = htonl( INADDR_ANY );
	
	if ( setsockopt( sock,IPPROTO_IP ,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) 
	{
		perror("setsockopt");
		return 0;
	}
	
	printf("\x1b[36m++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++\x1b[0m\n");
	printf("Welcome %s\n",argv[ ARG_USER_NAME ] );
	printf("This group name is: %s\n",argv[ ARG_GROUP_NAME ] );
	printf("\x1b[36m++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++\x1b[0m\n");
	printf("\n");

	while (1)
	{
		addrlen = sizeof(addr);
	
		if( ( recvfrom( sock ,msgbuf ,MSG_BUFF_SIZE , 0,(struct sockaddr *) &addr,&addrlen)) < 0)
		{
			perror("recvfrom");
			return 0;
		}
		printf( "%s", msgbuf );  
	}
	return 0;
}
