#ifndef __SERVER_TCP_H__
#define __SERVER_TCP_H__

#include "DataBase.h"
#include <stddef.h>

#define BUFF_SIZE 1024

/** 
 * @brief Create Server.
 * @author Natali Mesika (natalimesik@gmail.com)
 */ 
 
/** 
 * @brief User-use function. this is the application .
 */

typedef struct ClientData ClientData;
typedef struct Message_Data Message_Data;
typedef struct Server Server;
typedef struct Params Params;


typedef void*(*ApplicationFunc)(void* msg, void* contex, int socket);
typedef void*(*WelcomeFunc)(ClientData* _client, void* contexUser);


typedef enum 
{
	SERVER_SUCCSSES, 
	SERVER_ALLOCATED_FAIL, 
	SERVER_SOCKET_FAIL, 
	SERVER_TOO_MANY_CLIENTS, 
	SERVER_DB_CREATE_FAIL,
	SERVER_FAIL
	
}SERVER_ERR;



struct Message_Data
{
	int m_socket;
	int m_bufferSize;
	char m_buffer[ BUFF_SIZE ];
};

struct Params
{
	int m_back_log;
	int m_server_port;
	size_t m_maxWaitTime;
	void* m_context;
	void* m_contexAccept;
	void* m_contexDisconnect;
	void* m_msg;
	WelcomeFunc m_acceptFunc;
	ApplicationFunc m_doSometing;
	WelcomeFunc m_disconnect;
};


/** 
 * @brief Create server
 * 
 * @brief Create Sarver that waiting for clients, maximum 1000 clients can enter on the same time.
 * @val   _serverParams: struct  that contain(?):
 * @val  _back_log: the size of the Os queue- contain no more than 128 clients;
 * @val  _server_port ;
 * @val	size_t m_maxWaitTime: timeout;
 * @val void* m_context: for the first function;
 * @val void* m_contexAccept: ;
 * @val void* m_contexDisconnect: ;
 * @val WelcomeFunc m_acceptFunc;
 * @val ApplicationFunc m_doSometing;
 * @val WelcomeFunc m_disconnect;
 
 * @returns This function returns Sarver booted and ready Running waiting for clients.
 */
Server* CreateServer(Params _serverParams);

SERVER_ERR RunServer(Server* _server );
 
void DestroyServer(Server* _server );



#endif /* __SERVER_TCP_H__ */

