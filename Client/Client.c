#include "Client.h"

#define _BSD_SOURCE /* For kill */

#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>	/* signal */
#include <signal.h>		/* signal */

#include "HashMap.h"				
#include "ClientServerComm.h"

#define SERVER_PORT 5555
#define MAX_SIZE 16
#define PID_HASH_CAP 40		/*Max of open group per client*/
#define BUFF_SIZE 4093
#define BUFF_RCV 1024

struct Client
{
	Client_Net* m_clientNet;
	int m_serverPort;
	HashMap* m_pidHash;		/*hashmap to save the pid of the sender and listen process*/

};


typedef struct PID
{
	int m_sender;
	int m_listen;
	char* m_groupName;	/*maybe not neccesry*/
}PID;

static GroupDetails* Init_Group_struct( char* _userName, char* _groupName , Client* _client);

static UserDetails* CreateUserDetails( const char* _userName, const char* _psw );

static void OpenChatWindow(char* _ipAddress, int _port, char* _userName, char* _groupName,  Client* _client);

/*functions for pid hashmap*/
static size_t HashFunctionGroups(const void* _key);

static int EqualityFunctionForGroups (const void* _firstKey, const void* _secondKey);

static int RemovePID( char* _groupName, Client* _client );

static int InsertPID( int _pidSender, int _pidListener, char* _groupName, Client* _client );


/***************************************************************/
Client* Connect_User(const char* _serverIP, int _serverPort)
{
	Client* newClient;
	
	newClient = (Client*)malloc(sizeof( Client) );
	if(NULL == newClient)
	{
		return NULL;
	}

	newClient->m_clientNet = Init_Client_Net();

	newClient->m_serverPort = _serverPort;
	
	/*Connect to the server ip and port*/
	ConnectNewClient(newClient->m_clientNet, _serverIP, _serverPort );
	
	newClient->m_pidHash = HashMap_Create( PID_HASH_CAP, HashFunctionGroups, EqualityFunctionForGroups);
	{
		if( NULL == newClient->m_pidHash)
		{
			Destroy_Client_Net( newClient->m_clientNet );
			free(newClient);
			
			return NULL;
		}
	}
	
	return newClient;
}

void Destroy_Client( Client* _client)
{
	if( NULL == _client)
	{
		return;
	}
	
	Destroy_Client_Net( _client->m_clientNet );
	/*HashMap_Destroy( _client->m_pidHash);*/
	free(_client);
}

/*recive user name and psw ( the struct) and send it to the protocol- the last destination - the DB for update */
Client_err User_Register( const char* _userName, const char* _psw , Client* _client)
{
	UserDetails* user;
	int res = 0;
	int rcvFromSend = 0;
	char _buffer[ BUFF_SIZE ] = {0};
	Protocol_Type _type;
	
	if( NULL == _userName || NULL == _psw || NULL == _client )
	{
		return CLIENT_ALLOC_FAIL;
	}
	
	user = CreateUserDetails( _userName, _psw );
	if(NULL == user)
	{
		return CLIENT_ALLOC_FAIL;
	}

	/*Pack the msg- user struct*/
	res = Pack( REGISTER_REQ, user);
	if( -1 == res)
	{
		return CLIENT_PACKING_FAIL;
	}
	
	/*snd to server the all pack-struct with user name and details, the len and thr number of the socket */
	rcvFromSend = Send_To_Server( user, sizeof( UserDetails ), _client->m_clientNet );
	if( -1 == rcvFromSend)
	{
		return CLIENT_ALLOC_FAIL;
	}
	
	Recive_From_Server( _buffer ,BUFF_SIZE, _client->m_clientNet);
	UnPack( _buffer, &_type );
	
	if( ((Register_Ans*)_buffer)->m_answer == REGISTER_SUCCESS)
	{
		return CLIENT_SUCCESS;
	}
	else if( ((Register_Ans*)_buffer)->m_answer == REGISTER_USER_NAME_ALLREADY_EXIST)
	{
		return CLIENT_FAIL;
	}
	
	return CLIENT_FAIL;

}


Client_err User_LogIn( const char* _userName, const char* _psw , Client* _client)
{
	UserDetails* user;
	int rcvFromSend = 0;
	int res = 0;
	char _buffer[ BUFF_SIZE ] = {0};
	Protocol_Type _type;
	
	if( NULL == _userName || NULL == _psw || NULL == _client )
	{
		return CLIENT_ALLOC_FAIL;
	}

	user = CreateUserDetails( _userName, _psw );
	if(NULL == user)
	{
		return CLIENT_ALLOC_FAIL;
	}
	
	res = Pack( LOGIN_REQ, user);
	if( -1 == res)
	{
		return CLIENT_PACKING_FAIL;
	}
	
	/*snd to server the all pack-struct with user name and details, the len and thr number of the socket */
	rcvFromSend = Send_To_Server( user, sizeof( UserDetails ), _client->m_clientNet );
	if( -1 == rcvFromSend)
	{
		return CLIENT_ALLOC_FAIL;
	}
	
	Recive_From_Server( _buffer ,BUFF_SIZE, _client->m_clientNet);
	UnPack( _buffer, &_type );
	
	if( ((Login_Ans*)_buffer)->m_answer == LOGIN_USER_SUCCESSFULLY_CONNECTED)
	{
		return CLIENT_SUCCESS;
	}
	else if( ((Login_Ans*)_buffer)->m_answer == LOGIN_USER_NAME_NOT_EXIST)
	{
		return CLIENT_USER_NAME_NOT_EXIST;
	}
	else if( ((Login_Ans*)_buffer)->m_answer == LOGIN_WRONG_PSW)
	{
		return CLIENT_WRONG_PSW;
	}
	else if( ((Login_Ans*)_buffer)->m_answer == LOGIN_ALLREADY_CONNECTED)
	{
		return CLIENT_ALLREADY_CONNECTED;
	}
	
	return -1;
}

Client_err Create_New_Group( char* _userName, char* _groupName , Client* _client )
{
	GroupDetails* newGroup;
	int rcv;
	int rcvFromSend;	
	char buffer[ BUFF_SIZE ] = {0};
	char ipAddress[ BUFF_SIZE ] = {0};
	Protocol_Type type;
	
	if(  NULL == _groupName || NULL == _client )
	{
		return CLIENT_ALLOC_FAIL;
	}
	
	newGroup = Init_Group_struct( _userName, _groupName, _client);

	rcv = Pack( CREATE_GROUP_CONVERSATION_REQ, newGroup);
	if( -1 == rcv)
	{
		return CLIENT_PACKING_FAIL;
	}

	rcvFromSend = Send_To_Server( newGroup, sizeof( GroupDetails ), _client->m_clientNet );
	if( -1 == rcvFromSend)
	{
		return CLIENT_SEND_TO_SERVER_FAIL;
	}
		
	Recive_From_Server( buffer , BUFF_RCV, _client->m_clientNet);
	UnPack( buffer, &type );
	
	switch( ( (Create_Group_Ans*)buffer)->m_answer )
	{
		case GROUP_SUCCESSFULLY_INSERT:
		{	
			strcpy( ipAddress, ( (Create_Group_Ans*)buffer)->m_groupIP );

			OpenChatWindow(	ipAddress, _client->m_serverPort, _userName, _groupName, _client );

			return CLIENT_SUCCESS;
		}
		break;
		case GROUP_NAME_ALREADY_EXIST:
		{
			return CLIENT_FAIL_GROUP_CREATE;
		}
		break;
	}
	return CLIENT_SUCCESS;
}

Client_err Join_To_Group( char* _userName, char* _groupNameToFind , Client* _client )
{
	GroupDetails* newGroupToFind;
	int rcvpPack;
	int rcvFromSend;
	char buffer[ BUFF_SIZE ] = {0};
	char ipAddress[ BUFF_SIZE ] = {0};
	Protocol_Type type;
		
	if( NULL == _userName || NULL == _groupNameToFind || NULL == _client)
	{
		return CLIENT_ALLOC_FAIL;
	}

	newGroupToFind = Init_Group_struct( _userName, _groupNameToFind, _client);
	if( NULL == newGroupToFind)
	{
		return CLIENT_ALLOC_FAIL;	
	}
	
	rcvpPack = Pack( JOIN_CONVERSATION_REQ, newGroupToFind);
	if( -1 == rcvpPack)
	{
		return CLIENT_PACKING_FAIL;
	}

	rcvFromSend = Send_To_Server( newGroupToFind, sizeof( GroupDetails ), _client->m_clientNet );
	if( -1 == rcvFromSend)
	{
		return CLIENT_SEND_TO_SERVER_FAIL;
	}

	Recive_From_Server( buffer , BUFF_RCV, _client->m_clientNet);
	UnPack( buffer, &type );

	switch( ( (Create_Group_Ans*)buffer)->m_answer )
	{
		case GROUP_SUCCESSFULLY_JOIN:
		{	
			strcpy( ipAddress, ( (Create_Group_Ans*)buffer)->m_groupIP );
			OpenChatWindow(	ipAddress, _client->m_serverPort, _userName, _groupNameToFind, _client);

			return CLIENT_SUCCESS;
		}
		
		case GROUP_NAME_NOT_EXIST:
		{
			return CLIENT_FAIL_GROUP_CREATE;
		}
	}
	
	return CLIENT_SUCCESS;
}


Client_err Leave_Group_Conversation( char* _userName, char* _groupNameToFind , Client* _client )
{
	GroupDetails* groupToFind;
	int rcvpPack;
	int rcvFromSend;
	char buffer[ BUFF_SIZE ] = {0};
	char ipAddress[ BUFF_SIZE ] = {0};
	Protocol_Type type;
	int pidSender;
	int pidListener;
		
	if( NULL == _userName || NULL == _groupNameToFind || NULL == _client)
	{
		return CLIENT_ALLOC_FAIL;
	}

	groupToFind = Init_Group_struct( _userName, _groupNameToFind, _client);
	if( NULL == groupToFind)
	{
		return CLIENT_ALLOC_FAIL;	
	}
	
	rcvpPack = Pack( LEAVE_CONVERSATION_REQ, groupToFind);
	if( -1 == rcvpPack)
	{
		return CLIENT_PACKING_FAIL;
	}

	rcvFromSend = Send_To_Server( groupToFind, sizeof( GroupDetails ), _client->m_clientNet );
	if( -1 == rcvFromSend)
	{
		return CLIENT_SEND_TO_SERVER_FAIL;
	}

	Recive_From_Server( buffer , BUFF_RCV, _client->m_clientNet);
	UnPack( buffer, &type );

	switch( ( (Create_Group_Ans*)buffer)->m_answer )
	{
		case GROUP_SUCCESSFULLY_FOUND:
		{	
			RemovePID( _groupNameToFind, _client );
			
			return CLIENT_SUCCESS;
		}
		
		case GROUP_NAME_NOT_EXIST:
		{
			return CLIENT_FAIL_GROUP_LEAVE;
		}
	}
	
	return CLIENT_SUCCESS;
}

/*************************************************Static***********************************************************/
static void OpenChatWindow(char* _ipAddress, int _port, char* _userName, char* _groupName,  Client* _client)
{
	char bufferComm[ BUFF_SIZE ] = {0};
	char bufferComm2[ BUFF_SIZE ] = {0};
	FILE* fdListen;
	FILE* fdSender;
	int listenPID;
	int senderPID;
	
	sprintf( bufferComm, "gnome-terminal --geometry 60x60+50+10 -x ./listen.out %s %d %s %s", _ipAddress, _port, _userName, _groupName);
	system(bufferComm);
	
	fdListen = fopen("./fileForListen","r");
	if( NULL == fdListen)
	{
		return;
	}
	
	fscanf(fdListen,"%d",&listenPID );
	fclose(fdListen);
	
	sprintf(bufferComm2,"gnome-terminal --geometry 60x60+110+10 -x ./sender.out %s %d %s %s", _ipAddress, _port, _userName , _groupName);
	system(bufferComm2);
	
	/*scan to file the pid*/
	
	fdSender = fopen("./fileForSender","r");
	if( NULL == fdSender)
	{
		return;
	}
	
	fscanf(fdSender,"%d",&senderPID );
	fclose(fdSender);

	InsertPID( listenPID, senderPID, _groupName, _client );
	
}

static int InsertPID( int _pidSender, int _pidListener, char* _groupName, Client* _client )
{
	PID* pids;
	
	pids = malloc(sizeof(PID));
	if( NULL == pids)
	{
		return -1;
	}
	
	pids->m_sender = _pidSender;
	pids->m_listen = _pidListener;
	
	
	if( MAP_SUCCESS == HashMap_Insert( _client->m_pidHash, _groupName, pids  ) )
	{
		return 1;
	} 
	return -1;
}

static int RemovePID(  char* _groupName, Client* _client )	
{
	PID* pids;
	PID* pidsValue;
	
	if( MAP_SUCCESS == HashMap_Remove( _client->m_pidHash, _groupName, (void**)&pids, (void**)&pidsValue ) )
	{
		return 1;
	} 
	
	kill( pidsValue->m_sender, SIGINT );
	kill( pidsValue->m_listen, SIGINT );
	
	Destroy_Client(_client);
	
	return -1;
}

static GroupDetails* Init_Group_struct( char* _userName, char* _groupName , Client* _client)
{
	GroupDetails* newGroup;
	
	newGroup = ( GroupDetails* )malloc(sizeof(GroupDetails));
	if(NULL == newGroup)
	{
		return NULL;
	}	

	strcpy(newGroup->m_groupName, _groupName);
	strcpy(newGroup->m_userName, _userName);
	
	return newGroup;
}

static UserDetails* CreateUserDetails( const char* _userName, const char* _psw )
{
	UserDetails* user;
	
	user = (UserDetails*)malloc( sizeof( UserDetails) );
	if( NULL == user)
	{
		return NULL;
	}

	strcpy(user->m_userName, _userName);
	strcpy(user->m_password , _psw);
	
	return user;

}

static int EqualityFunctionForGroups (const void* _firstKey, const void* _secondKey)
{
	int res;
	
	res = strcmp(  (char*)_firstKey , (char*)_secondKey );
	if(0 == res)
	{
		return 1;
	}
	
	return 0;
	
}

static size_t HashFunctionGroups(const void* _key)
{
	int i;
	int len;
	int sum = 0;
	
	len = strlen( (char*)_key );
	
	for (i = 0; i < len; i += 1)
	{
		sum += ( (char*)_key)[i];
	}
	return sum;
}




