#include "ServerSide.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>	
#include <stdlib.h>		  /*malloc*/
#include <sys/socket.h>   /*for socket*/
#include <sys/types.h>       
#include <netinet/in.h>   /*for struct sockaddr_in*/
#include <string.h>		  /*memset*/
#include <signal.h>		  /*sigaction*/
#include <sys/time.h>     /*FD_SET, FD_ISSET, FD_ZERO macros*/
#include <sys/select.h>	  /*for select*/


#include "ServerApp.h"
#include "DataBase.h"
#include "server_tcp.h"

#define IP_LEN

struct ClientData 
{
	int m_clientSock;				  /* The client's socket number */
	struct timeval m_client_timeout;  /* Cliects experation time */
	struct sockaddr_in m_clientSin;
};


void* WelcomeNewClient( ClientData* _client, void* _contexUser)
{
	printf("Client connect \n");

	return _client;
}

void* ClientDisconnected( ClientData* _client, void* _contexUser)
{
	printf("Client disconnect \n");

	return _contexUser;
}

void RunServerSide()
{
	DataBase* dataBase;
	Params serverParams;
	Server* server;
	int port;
	
	dataBase = CreateMainDataBase();
	if( NULL == dataBase )
	{
		return;		
	}
	
	printf("Port:\n");
	scanf("%d", &port);
	
	serverParams.m_back_log = 11;
	serverParams.m_server_port = port;
	serverParams.m_doSometing = GetFromClientAndSendToServer;
	serverParams.m_context = dataBase;			/*Pointer to the data base*/

	serverParams.m_maxWaitTime = 300;
	serverParams.m_acceptFunc = WelcomeNewClient;
	serverParams.m_contexAccept = NULL;
	serverParams.m_disconnect = ClientDisconnected;
	serverParams.m_contexDisconnect = NULL;

	server = CreateServer( serverParams );

	RunServer(server);
}

