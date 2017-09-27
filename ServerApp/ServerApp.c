#include "ServerApp.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "server_tcp.h"
#include "DataBase.h"

#define ALREADY_EXIST 0
#define UNIQE 1
#define MAX_IP_ADDR 16

static void* CheckUserNameOnDB( UserDetails* _user, DataBase* _dataBase, Protocol_Type _type );
static void* ANSForGroupRequests( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type);

void* GetFromClientAndSendToServer( void* _rcvFromClient, void* _dataBase, int _socket )
{
	Protocol_Type _type;
	Register_Ans* answer = NULL;

	UnPack( _rcvFromClient , &_type );
	
	switch(_type)
	{
		case( (unsigned char)REGISTER_REQ ):
		case( (unsigned char)LOGIN_REQ ):
		{																
			answer = CheckUserNameOnDB( _rcvFromClient, (DataBase*) _dataBase, _type);
			if( NULL == answer)
			{
				return NULL;
			}
		}
		break;
		case( (unsigned char)CREATE_GROUP_CONVERSATION_REQ ):
		{
			answer = ANSForGroupRequests( _rcvFromClient, (DataBase*) _dataBase, CREATE_GROUP_CONVERSATION_REQ); 
		}
		break;
		case( (unsigned char)JOIN_CONVERSATION_REQ ):
		{
			answer = ANSForGroupRequests( _rcvFromClient, (DataBase*) _dataBase, JOIN_CONVERSATION_REQ); 
		}
		break;
		/*Checks whether the user in the group-then returns an exit permit*/
		case( (unsigned char)LEAVE_CONVERSATION_REQ ):
		{
			answer = ANSForGroupRequests( _rcvFromClient, (DataBase*) _dataBase, LEAVE_CONVERSATION_REQ); 
		}
		break;
	}

	Pack( answer->m_type, answer );

	return answer;
}

/******************************************STATIC***********************************************/

static void* ANSForGroupRequests( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type)
{
	Create_Group_Ans* ansGroupPtr;
	char ipAddress[ MAX_IP_ADDR ] = {0};
	char ipJoinGroup[ MAX_IP_ADDR ] = {0};
	char ipRemoveGroup[ MAX_IP_ADDR ] = {0};
	char ipJoinGroup2[ MAX_IP_ADDR ] = {0};
	DB_Err db_res;

	ansGroupPtr = (Create_Group_Ans*)malloc( sizeof(Create_Group_Ans) );
	if( NULL == ansGroupPtr)
	{
		return NULL;
	}
	/*TODO Swich*/
	
	if( _type == CREATE_GROUP_CONVERSATION_REQ )
	{
		ansGroupPtr->m_type = CREATE_GROUP_CONVERSATION_ANS;
		db_res = Check_Group_Name_And_Insert( _group, _dataBase, _type, ipAddress);
	}
	
	if( _type == JOIN_CONVERSATION_REQ )
	{
		ansGroupPtr->m_type = JOIN_CONVERSATION_ANS;
		db_res = Try_to_Join_To_Group( _group, _dataBase, _type, ipJoinGroup2, ipJoinGroup );
	}
	
	if( _type == LEAVE_CONVERSATION_REQ )
	{
		ansGroupPtr->m_type = LEAVE_CONVERSATION_ANS;
		db_res = Try_to_Leave_To_Group( _group, _dataBase, _type, ipRemoveGroup );
	}
	
	switch(db_res)
	{
		case DB_GROUP_SUCCESSFULLY_INSERT:
		{
			strcpy( ansGroupPtr->m_groupIP, ipAddress );
			ansGroupPtr->m_answer = GROUP_SUCCESSFULLY_INSERT;
		}
		break;
		case DB_GROUP_NAME_ALREADY_EXIST:
		{
			ansGroupPtr->m_answer = GROUP_NAME_ALREADY_EXIST;		
		}
		break;
		case DB_GROUP_NAME_NOT_EXIST:
		{
				strcpy( ansGroupPtr->m_groupIP, ipAddress );
				ansGroupPtr->m_answer = GROUP_NAME_NOT_EXIST;
		}
		break;
		case DB_GROUP_JOIN_SUCCESSFULLY:
		{
			strcpy( ansGroupPtr->m_groupIP, ipJoinGroup );
			ansGroupPtr->m_answer = GROUP_SUCCESSFULLY_JOIN;
		}
		break;
		
		case DB_GROUP_REMOVE_SUCCESSFULLY:
		{
			strcpy( ansGroupPtr->m_groupIP, ipRemoveGroup );
			ansGroupPtr->m_answer = GROUP_SUCCESSFULLY_JOIN;
		}
		break;
	}
	
	return ansGroupPtr;
}



static void* CheckUserNameOnDB( UserDetails* _user, DataBase* _dataBase, Protocol_Type _type )
{
	int res = 0;
	Register_Ans* answerReg;
	Login_Ans* answerLogin;
	DB_Err rcv;
	
	
	if( NULL == _user || NULL == _dataBase)
	{
		return NULL;
	}
	
	if(REGISTER_REQ ==_type )
	{
		answerReg = (Register_Ans*)malloc( sizeof(Register_Ans) );
		if( NULL == answerReg)
		{
			return NULL;
		}	
	
		answerReg->m_type = REGISTER_ANS;
	
		res = IsUserNameUnique( _dataBase, _user );
		if( res == UNIQE ) 
		{
			answerReg->m_answer = REGISTER_SUCCESS;
		}
	
		else if( res == ALREADY_EXIST)
		{
			answerReg->m_answer = REGISTER_USER_NAME_ALLREADY_EXIST;
		}

		return answerReg;	
	}
	
	if(LOGIN_REQ ==_type )
	{
		answerLogin = (Login_Ans*)malloc( sizeof( Login_Ans ) );
		if( NULL == answerLogin)
		{
			return NULL;
		}	
		
		answerLogin->m_type = LOGIN_ANS;
	
		rcv = CheckClientStatusAndConnect( _dataBase,  _user );
		switch(rcv)
		{
			case DB_CONNECT_SUCCESFULLY:
				printf("LOGIN_USER_SUCCESSFULLY_CONNECTED: %d\n", rcv);
				answerLogin->m_answer = LOGIN_USER_SUCCESSFULLY_CONNECTED;
			break;
			
			case DB_ALREADY_CONNECTED:
				printf("LOGIN_ALLREADY_CONNECTED: %d\n", rcv);
				 answerLogin->m_answer = LOGIN_ALLREADY_CONNECTED;
			break;
			
			case DB_WORNG_USER_NAME:
				printf("LOGIN_USER_NAME_NOT_EXIST: %d\n", rcv);
				 answerLogin->m_answer = LOGIN_USER_NAME_NOT_EXIST;
			break;
			
			case DB_WORNG_PSW:
				printf("LOGIN_WRONG_PSW: %d\n", rcv);
				 answerLogin->m_answer = LOGIN_WRONG_PSW;
			break;
		}
		
		return answerLogin;
	}

}

/*Maybe TODO message struct from Server*/

