#ifndef __COMMUNICATION_PROJECT_H__
#define __COMMUNICATION_PROJECT_H__

#include <stddef.h>

typedef struct Client_Net Client_Net; 


typedef enum Client_Status
{
	CLIENT_STATUS_CONNECTED = 0,
	CLIENT_STATUS_DISCONNECTED
	
}Client_Status;


struct Client_Net
{
	int m_socket;
	Client_Status m_status;
};


Client_Net* Init_Client_Net();


void Destroy_Client_Net( Client_Net* _client_net );

	
int Send_To_Server(void* _value, int _len, Client_Net* _clientNet);


void ConnectNewClient(Client_Net* _newClient, const char* _serverIP, int _serverPort);


int Recive_From_Server( void* _value, int _len , Client_Net* _clientNet);

#endif /* __COMMUNICATION_PROJECT_H__*/
