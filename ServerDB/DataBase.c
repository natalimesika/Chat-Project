#include "DataBase.h"

#include <stdlib.h> /*malloc*/
#include <string.h> /* strcpy*/
#include <stdio.h>

#include "HashMap.h"
#include "PoolMultyCastAdd.h"

#define CAPACITY 50
#define USER_GROUPS_CAPACITY 50
#define CONNECT 1
#define DISCONNECT 0
#define ALREADY_EXIST 0
#define UNIQE 1
#define MAX_LENGTH_PSW_ 17
#define MAX_LENGTH 17
#define MAX_LENGTH_GROUP_NAME 17


struct UserValue
{
	int m_status;
	char m_psw[ MAX_LENGTH_PSW_];
	HashMap* m_myGroups;
};

struct GroupValue
{
	int m_numOfPartic;
	char m_groupName[ MAX_LENGTH ];
	char m_ip_add_group[ MAX_LENGTH ];
	
};

struct DataBase
{
	UsersDB* m_userDB;
	GroupsDB* m_groupDB;
	IP_Pool_Addr* m_addIPPool;
};

struct UsersDB
{
	HashMap* m_userNames;
};


struct GroupsDB
{
	HashMap* m_groupNames;
	int m_numOfParticipant;

};
static void Update_Group_Details_For_User( DataBase* _dataBase, char* _userNameKey, char* _group_to_add );
static UsersDB* CreateUserDataBase();
static void DestroyUserDB( UsersDB* _user_DB );
static GroupsDB* CreateGroupsDataBase();
static void DestroyGroupsDB( GroupsDB* _group_dataBase );
static void keyDestroyGroup(void* _key);
static void valDestroyGroup(void* _value);
static int EqualityFunctionForGroups (const void* _firstKey, const void* _secondKey);
static size_t HashFunctionGroups(const void* _key);
static void keyDestroyUser(void* _key);
static void valDestroyUser(void* _value);
static int EqualityFunctionForUsers (const void* _firstKey, const void* _secondKey);
static size_t HashFunctionUsers(const void* _key);
static DB_Err InsertUserToList( DataBase* _dataBase ,UserDetails* _userDetails, int _status);

DataBase* CreateMainDataBase()
{

	DataBase* data = NULL;
	
	data = (DataBase*)malloc( sizeof(DataBase) );
	if(NULL == data)
	{
		return NULL;
	}
	
	data->m_userDB = CreateUserDataBase();
	data->m_groupDB = CreateGroupsDataBase();
	data->m_addIPPool =  CreateIP_Multecaste();
	
	return data;

}

void DestroyDB( DataBase* _dataBase )
{

	if(NULL == _dataBase)
	{
		return;
	}

	DestroyUserDB( _dataBase->m_userDB );
	DestroyGroupsDB( _dataBase->m_groupDB );
}

int IsUserNameUnique( DataBase* _dataBase, UserDetails* _userDetails )
{
	void* _pValue;
	DB_Err res;
	
	if( MAP_KEY_NOT_FOUND_ERROR == HashMap_Find( _dataBase->m_userDB->m_userNames , _userDetails->m_userName,  &_pValue) )
	{
		printf("1. user uniqe: %s\n",  _userDetails->m_userName);
		res = InsertUserToList(  _dataBase , _userDetails, DISCONNECT);
		if(DB_SUCCESS != res)
		{
			printf("ERR\n");
		}
		
		return UNIQE;  /*1*/
	}	
	else
	{
		printf("2. user uniq error: %s\n",  _userDetails->m_userName);
		return ALREADY_EXIST;	/*0*/
	}
}

/*
 3 options:
 if the client not connected- connect him.
 if the client allready connect, recive this answerLogin 
 if the client not regitred- error of this answer.
*/

DB_Err CheckClientStatusAndConnect( DataBase* _dataBase, UserDetails* _userDetails )
{
	void* _pValue;
	DB_Err res; 

	if( NULL == _dataBase  || NULL == _userDetails)
	{
		return DB_WORNG_USER_NAME;
	}
	
	/*the name not in the list*/
	if( MAP_KEY_NOT_FOUND_ERROR == HashMap_Find( _dataBase->m_userDB->m_userNames, _userDetails->m_userName, &_pValue) )
	{
		return DB_WORNG_USER_NAME;
	}
	
	/*found*/
	else
	{
		/*alread connect*/
		if( CONNECT == ((UserValue*)_pValue)->m_status )
		{
			return DB_ALREADY_CONNECTED;
		}
		
		if( strcmp( _userDetails->m_password, ((UserValue*)_pValue)->m_psw ) != 0)
		{	
			return DB_WORNG_PSW;
		}
		
		res = InsertUserToList( _dataBase , _userDetails,  CONNECT);
		if(DB_SUCCESS == res)
		{
			return DB_CONNECT_SUCCESFULLY; 
		}
		
	}
	return DB_CONNECT_SUCCESFULLY; 
}


DB_Err Check_Group_Name_And_Insert( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipAddress )
{
	GroupValue* value_for_group = NULL;
	void* pValue;
	
	if(NULL == _group || NULL == _dataBase )
	{
		return DB_INSERT_FAIL;
	}

	value_for_group = malloc( sizeof(GroupValue) );
	if( NULL == value_for_group)
	{
		return DB_ALLOC_FAIL;
	}
	
	strcpy( value_for_group->m_groupName, _group->m_groupName);
	
	value_for_group->m_numOfPartic = 1;
	
	if(MAP_KEY_NOT_FOUND_ERROR != HashMap_Find(_dataBase->m_groupDB->m_groupNames, _group->m_groupName, &pValue ) )
	{
		return DB_GROUP_NAME_ALREADY_EXIST;
	}
	
	else
	{
		GetIPAddress( _dataBase->m_addIPPool, _ipAddress );
		
		strcpy( value_for_group->m_ip_add_group, _ipAddress );
		
		if( MAP_SUCCESS != HashMap_Insert( _dataBase->m_groupDB->m_groupNames, _group->m_groupName, value_for_group) )
		{
				return DB_INSERT_FAIL;
		}

		Update_Group_Details_For_User( _dataBase, _group->m_userName, _group->m_groupName );		

		return DB_GROUP_SUCCESSFULLY_INSERT;		
	}
	
	
	
	return DB_GROUP_SUCCESSFULLY_INSERT;
}

DB_Err Try_to_Join_To_Group( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipAddress, char* _ipJoinGroup )
{
	GroupValue* groupValue;
	void* bla;
	
	if(NULL == _group || NULL == _dataBase )
	{
		return DB_INSERT_FAIL;
	}

	if( MAP_KEY_NOT_FOUND_ERROR == HashMap_Find( _dataBase->m_groupDB->m_groupNames, _group->m_groupName, &bla ) )
	{
		/*group not found*/
		return DB_GROUP_NAME_NOT_EXIST;
	}
	
	/*found! need to get the group ip*/
	if( MAP_SUCCESS == HashMap_Find(_dataBase->m_groupDB->m_groupNames, _group->m_groupName, (void*)&groupValue ) )
	{
		
		if(  0 == strcmp(_group->m_groupName, groupValue->m_groupName) ) 
		{	

			strcpy( _ipAddress, groupValue->m_ip_add_group );

			strcpy( _ipJoinGroup, groupValue->m_ip_add_group );

			++groupValue->m_numOfPartic;
			
			return DB_GROUP_JOIN_SUCCESSFULLY;
		}
	}

	return DB_GROUP_SUCCESSFULLY_INSERT;		
}

DB_Err Try_to_Leave_To_Group( GroupDetails* _group, DataBase* _dataBase, Protocol_Type _type, char* _ipRemoveGroup )
{
	UserValue* userLeave;
	GroupValue* groupValue;
	void* _pKey;
	void* _pValue;
	
	if(NULL == _group || NULL == _dataBase )
	{
		return DB_INSERT_FAIL;
	}

	if( MAP_SUCCESS != HashMap_Find( _dataBase->m_userDB->m_userNames, _group->m_userName, (void**)&userLeave ) )
	{
		/*group not found*/
		return DB_GROUP_NAME_NOT_EXIST;
	}
	
	else
	{	/*remove group*/
		if( MAP_SUCCESS != HashMap_Remove( userLeave->m_myGroups, _group->m_groupName, &_pKey, &_pValue))
		{
			return DB_GROUP_NAME_NOT_EXIST;
		}		
		
		if( MAP_SUCCESS == HashMap_Find(_dataBase->m_groupDB->m_groupNames, _group->m_groupName, (void*)&groupValue ) )
		{
			if(  0 == strcmp(_group->m_groupName, groupValue->m_groupName) ) 
			{	
				strcpy( _ipRemoveGroup, groupValue->m_ip_add_group );

				--groupValue->m_numOfPartic;
			}
		}
		return DB_GROUP_REMOVE_SUCCESSFULLY;
	}
}

/****************************************************************************************************/
static void Update_Group_Details_For_User( DataBase* _dataBase, char* _userNameKey, char* _group_to_add )
{
	UserValue* value_for_user;
	char* groupNameUpdate;
	char* gg;
	

	if( MAP_SUCCESS != HashMap_Find( _dataBase->m_userDB->m_userNames, _userNameKey, (void*)&value_for_user ) )
	{
		printf("1. MAP_SUCCESS != HashMap_Find\n");
		return;
	
	}
	
	groupNameUpdate = (char*)calloc( strlen(_group_to_add), sizeof(char) );
	if ( NULL == groupNameUpdate )
	{
		return;
	}
	strcpy( groupNameUpdate, _group_to_add );
	/*FIXME*/
	HashMap_Insert( value_for_user->m_myGroups, groupNameUpdate, groupNameUpdate );

}

static UsersDB* CreateUserDataBase()
{
	UsersDB* users = NULL;
	
	users = (UsersDB*)malloc( sizeof(UsersDB) );
	if(NULL == users)
	{
		return NULL;
	}
	users->m_userNames = HashMap_Create( CAPACITY , HashFunctionUsers, EqualityFunctionForUsers );
	if(NULL == users->m_userNames)
	{
		return NULL;
	}
	
	return users;
}

static void DestroyUserDB( UsersDB* _user_DB )
{
	if(NULL == _user_DB)
	{
		return;
	}
	
	HashMap_Destroy( &(_user_DB->m_userNames), keyDestroyUser, valDestroyUser);
	
	free(_user_DB);
}
/*By status (whether the user is logged in or not) choose whether to register the user or logging*/
static DB_Err InsertUserToList( DataBase* _dataBase ,UserDetails* _userDetails, int _status)
{
	int rcv;
	UserValue* value;
	
	value = malloc(sizeof(UserValue) );
	if( NULL == value)
	{
		return DB_INSERT_FAIL;
	}
		
	/*DISCONNECT*/

	value->m_status = _status;
	
	/*for register*/
	if( DISCONNECT == _status)
	{
		strcpy(value->m_psw, _userDetails->m_password);

		value->m_myGroups = HashMap_Create( USER_GROUPS_CAPACITY, HashFunctionGroups, EqualityFunctionForGroups);
		if( NULL == value->m_myGroups)
		{
			return DB_INSERT_FAIL;
		}
		
		rcv = HashMap_Insert( _dataBase->m_userDB->m_userNames , _userDetails->m_userName, value);
		if(rcv != MAP_SUCCESS)
		{
			return DB_INSERT_FAIL;
		}
	}
	else
	{
	
	}
	
	return DB_SUCCESS;
}

static GroupsDB* CreateGroupsDataBase()
{
	GroupsDB* groups = NULL;
	
	groups = (GroupsDB*)malloc( sizeof(GroupsDB) );
	if(NULL == groups)
	{
		return NULL;
	}
	
	groups->m_groupNames = HashMap_Create( CAPACITY , HashFunctionGroups, EqualityFunctionForGroups );
	if(NULL == 	groups->m_groupNames)
	{
		return NULL;
	}
	
	groups->m_numOfParticipant = 0;
	
	return groups;
}


static void DestroyGroupsDB( GroupsDB* _group_dataBase )
{
	if(NULL == _group_dataBase)
	{
		return;
	}
	
	HashMap_Destroy( &_group_dataBase->m_groupNames, keyDestroyGroup, valDestroyGroup);
	
	free( _group_dataBase );
}

static void keyDestroyUser(void* _key)
{
	return;
}

static void valDestroyUser(void* _value)
{
	return;

}

static int EqualityFunctionForUsers (const void* _firstKey, const void* _secondKey)
{
	int res;
	
	res = strcmp(  (char*)_firstKey , (char*)_secondKey );
	if(0 == res)
	{
		return 1;
	}
	
	return 0;
	
}

static size_t HashFunctionUsers(const void* _key)
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

/**************************************************/
static void keyDestroyGroup(void* _key)
{
	return;
}

static void valDestroyGroup(void* _value)
{
	return;

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

