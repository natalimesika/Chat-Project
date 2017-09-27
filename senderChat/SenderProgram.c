#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	struct sockaddr_in addr;
	char message [MSG_BUFF_SIZE] = {0};
	char messageToSnd [MSG_BUFF_SIZE] = {0};
	char wholeMessage[MSG_BUFF_SIZE] = {0};
	FILE* fdSender;
	
	fdSender = fopen("./fileForSender","w");
	fprintf(fdSender,"%d",getpid() );
	fclose(fdSender);

	if( (sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket");
		return 0;
	}
	
    /* set up destination address */
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( argv[ ARGIP ] );
	addr.sin_port = htons( atoi(argv[ ARGPORT ]) );
    
    strcpy(wholeMessage, argv[ARG_USER_NAME] );
    	
	printf("\x1b[36m++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++\x1b[0m\n");
	printf("Welcome %s\n",argv[ ARG_USER_NAME ] );
	printf("Write here...: \n" );
	printf("\x1b[36m++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++\x1b[0m\n");
	printf("\n");
     
	while(1)
	{
		fgets(message, MSG_BUFF_SIZE, stdin);
		sprintf( messageToSnd,"\x1b[36m%s:\x1b[0m %s", argv[ARG_USER_NAME], message);
		
		if(sendto (sock ,messageToSnd ,sizeof(messageToSnd) ,0,(struct sockaddr *) &addr, sizeof(addr)) < 0)
		{
		   perror("sendto");
		   exit(1);
		}
		sleep(1);
	}
}


