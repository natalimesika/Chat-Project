#include "RunChat.h"



#include "server_tcp.h"
#include "DataBase.h"


struct ServerChat
{
	Server* m_serverTcp;
	DataBase* m_dataBase;
	
};



ServerChat* CreateChat( Params _params)
{

	ServerChat* serverChat = NULL;
	
	serverChat = (ServerChat*)malloc( sizeof( ServerChat ) );
	if( NULL == serverChat )
	{
		return NULL;
	}
	
	serverChat->m_dataBase = CreateMainDataBase();
	if( NULL == serverChat->m_dataBase )
	{
		return NULL;		
	}

	serverChat->m_serverTcp = CreateServer(Params _serverParams);
	if( NULL == serverChat->m_serverTcp)
	{
		return NULL;
	}
	
	
	return serverChat;
}

void DestroyChat( ServerChat* _serverChat )
{
	if( NULL == _serverChat)
	{
		return;
	}	

	_serverChat->m_serverTcp = DestroyServer( serverChat->m_serverTcp );
	_serverChat->m_dataBase = DestroyDB( serverChat->m_dataBase ); 
}

int RunChat( ServerChat* _serverChat )
{
	SERVER_ERR res;
	if( NULL == _serverChat)
	{
		return 0;
	}
	
	res = RunServer( _serverChat->m_serverTcp );
	if( SERVER_SUCCSSES != res)
	{
		return 0;
	}
	
}


