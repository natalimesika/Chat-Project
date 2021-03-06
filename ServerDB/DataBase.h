#ifndef __DB_SERVER_H__
#define __DB_SERVER_H__

#include "Protocol.h"
typedef struct DataBase DataBase;

typedef struct UsersDB UsersDB; 
typedef struct GroupsDB GroupsDB;
typedef struct UserValue UserValue;
typedef struct GroupValue GroupValue;

typedef enum DB_Err{

	DB_ALLOC_FAIL = 0,
	DB_INSERT_FAIL,
	DB_CREATE_FAIL,
	DB_SUCCESS, 
	DB_CONNECT_SUCCESFULLY, 		/**/
	DB_ALREADY_CONNECTED, 
	DB_WORNG_USER_NAME,
	DB_WORNG_PSW,
	DB_GROUP_SUCCESSFULLY_INSERT,
	DB_GROUP_NAME_ALREADY_EXIST,
	DB_GROUP_NAME_NOT_EXIST, 
	DB_GROUP_JOIN_SUCCESSFULLY,
	DB_GROUP_REMOVE_SUCCESSFULLY
	
}DB_Err;

DataBase* CreateMainDataBase();

void DestroyDB( DataBase* _dataBase );

int IsUserNameUnique( DataBase* _dataBase, UserDetails* _userDetails );

DB_Err CheckClientStatusAndConnect( DataBase* _dataBase, UserDetails* _userDetails );

DB_Err Check_Group_Name_And_Insert( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipAddress );

DB_Err Try_to_Join_To_Group( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipAddress, char* _ipJoinGroup );

DB_Err Try_to_Leave_To_Group( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipRemoveGroup );
#endif /* __DB_SERVER_H__ */

