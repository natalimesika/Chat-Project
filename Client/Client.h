#ifndef __CLIENT_PROJECT_H__
#define __CLIENT_PROJECT_H__

#include <stddef.h>

#include "Protocol.h"

typedef struct Client Client;

typedef enum Client_err
{
	CLIENT_SUCCESS = 0,
	CLIENT_ALLOC_FAIL,
	CLIENT_FAIL,
	CLIENT_PACKING_FAIL,
	CLIENT_INIT_FAIL, 
	CLIENT_WRONG_PSW, 
	CLIENT_USER_NAME_NOT_EXIST, 
	CLIENT_ALLREADY_CONNECTED,
	CLIENT_SEND_TO_SERVER_FAIL, 
	CLIENT_FAIL_GROUP_CREATE,
	CLIENT_FAIL_GROUP_LEAVE
	

}Client_err;


Client* Connect_User(const char* _serverIP, int _serverPort);

void Destroy_Client( Client* _client);

Client_err User_Register( const char* _userName, const char* _psw , Client* _client);

Client_err User_LogIn( const char* _userName, const char* _psw , Client* _client);

Client_err Create_New_Group( char * _userName, char* _groupName , Client* _client );

Client_err Join_To_Group( char* _userName, char* _groupNameToFind , Client* _client );

Client_err Leave_Group_Conversation( char* _userName, char* _groupNameToFind , Client* _client );

#endif /* __CLIENT_PROJECT_H__*/
