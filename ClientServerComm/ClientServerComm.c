#include "ClientServerComm.h"

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>	
#include <stdlib.h>		  /*malloc*/
#include <sys/socket.h>   /*for socket*/
#include <sys/types.h>       
#include <netinet/in.h>   /*for struct sockaddr_in*/
#include <string.h>		  /*memset*/
#include <sys/time.h>     /*FD_SET, FD_ISSET, FD_ZERO macros*/
#include <sys/select.h>	  /*for select*/
#include <arpa/inet.h>


#define MAX_BUFF 4093
#define LENGTH 15


/**************/
/*Statics*/
/**************/

static int CreateSocket();

/*************************************************************************/
Client_Net* Init_Client_Net()
{
	Client_Net* newClient;
	
	newClient = (Client_Net*)malloc(sizeof(Client_Net) );
	if(NULL == newClient)
	{
		return NULL;
	
	}
	newClient->m_socket = CreateSocket();
	newClient->m_status = CLIENT_STATUS_CONNECTED;

	return newClient;
	
}


void Destroy_Client_Net( Client_Net* _client_net )
{
	close(_client_net->m_socket);
	free(_client_net);
}



void ConnectNewClient(Client_Net* _newClient, const char* _serverIP, int _serverPort)
{
	int retVal;
	struct sockaddr_in server_sin;

	memset( &server_sin, 0, sizeof(server_sin) );
	
	server_sin.sin_family = AF_INET;
	server_sin.sin_addr.s_addr = inet_addr( _serverIP );
	server_sin.sin_port = htons( _serverPort );
	
	retVal = connect( _newClient->m_socket, (struct sockaddr *)&server_sin, sizeof(server_sin) );
	if(0 > retVal)
	{
			perror("connect fail");
	}

}

/*copy the struct to the buffer and send it to the server*/
int Send_To_Server( void* _value, int _len , Client_Net* _clientNet )
{
	int sent_bytes;
	
	/*copy the struct to the buffer and send to the server*/ 
	sent_bytes = send( _clientNet->m_socket, _value, _len, 0 );
	if (sent_bytes < 0)
	{
		perror("send failed");
		
		return -1;
	}
	
	return 0;

}

int Recive_From_Server( void* _value, int _len , Client_Net* _clientNet)
{
	int read_byte;
	
	read_byte = recv( _clientNet->m_socket , _value, _len ,0 );
	
	if( read_byte < 0)
	{
		perror("read_byte fail");
		
		return -1;
	} 
	
	return 0;
}


/************************************************STATIC*********************************************************/
static int CreateSocket()
{
	int sock;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > sock)
	{
		perror("socket fail");
		abort();
	}
	
	return sock;
}
