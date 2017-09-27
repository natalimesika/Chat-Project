#include "UI.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Client.h"

#define MAX_SIZE 16
#define REGISTERED_OP 0
#define LOGIN_OP 1

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


struct Client_Interface
{
	Client* m_client;

};
static int InitExitFromGroup( Client_Interface* _ui, char* _userName );
static int RegisterOption( const char* _userName, const char* _userPassword , Client* _client );
static int LoginOption( const char* _userName, const char* _userPassword , Client* _client);
static int InitGroupCreate( Client_Interface* _ui, char* _userName );
static int InitJoinCreate( Client_Interface* _ui, char* _userName );
static int InitExitFromGroup( Client_Interface* _ui, char* _userName );
static char* Init_username();
static char* Init_Psw();
static char* Init_GroupName();

/**********************************************************************************************************/
/*CreateCommunication*/
/**********************************************************************************************************/

Client_Interface* CreateCommunication( int _serverPort, const char* _serverIP)
{
	Client_Interface* ui = NULL;
	
	ui = (Client_Interface*) malloc( sizeof( Client_Interface ) );
	if(NULL == ui)
	{
		return NULL;
	}
	
	ui->m_client = Connect_User( _serverIP, _serverPort);
	
	return ui;
}

/**********************************************************************************************************/
/*Destroy_UI*/
/**********************************************************************************************************/

void Destroy_UI( Client_Interface* _ui)
{
	if( NULL == _ui)
	{
		return;
	}
	free(_ui);
}

/**********************************************************************************************************/
/*RunUI*/
/**********************************************************************************************************/
int RunUI( Client_Interface* _ui )
{
	int answer = 5;
	int choice;

	if( NULL == _ui)
	{
		return 0;
	}
	
	printf("\033[2J\033[;H");
	printf("Welcome!\n\n");


	while(1)
	{
		printf("Enter \x1b[33m 0 \x1b[0m for \x1b[33m REGISTERATION \x1b[0m\n");
		printf("registered? \n");
		printf("Enter \x1b[33m 1 \x1b[0m for \x1b[33m LOGIN \x1b[0m\n\n \t");  
		scanf("%d", &answer);
	
		while( answer != REGISTERED_OP && answer != LOGIN_OP )
		{
			printf("\033[2J\033[;H");
			printf("Wrong, enter again\n");
			scanf("%d", &answer);
		}
	
		char* userName = Init_username();
		char* userPassword	= Init_Psw();
		printf("\033[2J\033[;H");
		/*register*/
		if( REGISTERED_OP == answer )
		{
			if( -1 == RegisterOption( userName, userPassword, _ui->m_client ) )
			{
				continue;
			}
		
			printf("Do you want to login? enter 1\n");
		
			scanf("%d", &answer);
			printf("%d\n", answer);
		}

		if( LOGIN_OP == answer )
		{
			if( -1 == LoginOption( userName, userPassword, _ui->m_client ))
			{
				continue;
			}
		}
		
		printf("\033[2J\033[;H");
		while(1)
		{
			printf("\n1. Create new chat group:\n");
			printf("2. Join to conversation:\n");
			printf("3. Leave group by name:\n");
			scanf("%d", &choice);
			
			printf("\033[2J\033[;H");
			
			switch(choice)
			{
				case (1):
					InitGroupCreate( _ui , userName);
				break;
				case (2):
					InitJoinCreate( _ui, userName);	
				break;
				case (3):
					InitExitFromGroup( _ui, userName);
				break;
			}
		}
		return 0;
	}	
	return 0;
}

/********************************STATIC******************************************/
static int InitExitFromGroup( Client_Interface* _ui, char* _userName )
{
	Client_err rcvFromexit;
	char _GroupToExitFrom[ MAX_SIZE + 1];
	
	printf("Enter group name:\n");
	
	scanf("%s",_GroupToExitFrom);

	if( NULL == _ui || NULL == _userName  )
	{
		return -1;
	}

	rcvFromexit = Leave_Group_Conversation( _userName, _GroupToExitFrom ,_ui->m_client );
	if( rcvFromexit == CLIENT_SUCCESS)
	{
		printf("\033[2J\033[;H");
		printf(ANSI_COLOR_GREEN);
		printf("\t\tleave successfully\n");
		printf(ANSI_COLOR_RESET);
	
		return 1;
	}
	printf("\033[2J\033[;H");
	printf(ANSI_COLOR_RED);
	printf("\t\tGroup not exist on your group list\n");
	printf(ANSI_COLOR_RESET);
	
	return -1;

}


static int InitJoinCreate( Client_Interface* _ui, char* _userName )
{
	Client_err rcvFromJoin;
	char groupNameToJoin[ MAX_SIZE + 1];
	
	if( NULL == _ui || NULL == _userName   )
	{
		return -1;
	}
	printf("Enter name of group\n");
	scanf("%s", groupNameToJoin);
	
	rcvFromJoin = Join_To_Group(  _userName,  groupNameToJoin , _ui->m_client );
	if( rcvFromJoin == CLIENT_SUCCESS)
	{
		printf("\033[2J\033[;H");
		printf(ANSI_COLOR_GREEN);
		printf("\t\tYou are connected to a group now\n");
		printf(ANSI_COLOR_RESET);
		
		return 1;
	}
	
	printf("\033[2J\033[;H");
	printf(ANSI_COLOR_RED);
	printf("\t\tGroup already exist\n");
	printf(ANSI_COLOR_RESET);
	
	return -1;
}


static int InitGroupCreate( Client_Interface* _ui, char* _userName )
{
	char* groupName;
	Client_err rcvFromCreate;
	
	groupName = Init_GroupName();
	if( NULL == groupName)
	{
		return -1;
	}
	
	rcvFromCreate = Create_New_Group( _userName,  groupName , _ui->m_client );
	if( rcvFromCreate == CLIENT_SUCCESS)
	{
		printf("\033[2J\033[;H");
		printf(ANSI_COLOR_GREEN);
		printf("\t\tGroup create successfully\n");
		printf(ANSI_COLOR_RESET);
	
		return 1;
	}
	printf("\033[2J\033[;H");
	printf(ANSI_COLOR_RED);
	printf("\t\tGroup already exist\n");
	printf(ANSI_COLOR_RESET);
	return -1;
}

static int LoginOption( const char* _userName, const char* _userPassword , Client* _client)
{
	Client_err rcvFromLogin;
	
	rcvFromLogin = User_LogIn( _userName, _userPassword , _client);
	switch(rcvFromLogin)
	{
		case CLIENT_SUCCESS:
		printf(ANSI_COLOR_GREEN);
		printf("\t\tCLIENT SUCCESS\n");
		printf(ANSI_COLOR_RESET);
		return 1;
		break;
		
		case CLIENT_WRONG_PSW:
		printf(ANSI_COLOR_RED);
		printf("\t\tCLIENT WRONG PSW\n");
		printf(ANSI_COLOR_RESET);
		return -1;
		break;
		
		case CLIENT_USER_NAME_NOT_EXIST:
		printf(ANSI_COLOR_RED);
		printf("\t\tCLIENT USER NAME NOT EXIST\n");
		printf(ANSI_COLOR_RESET);
		return -1;
		break;
		
		case CLIENT_ALLREADY_CONNECTED:
		printf(ANSI_COLOR_RED);
		printf("\t\tCLIENT ALLREADY CONNECTED\n");
		printf(ANSI_COLOR_RESET);
		return -1;
		break;
	}
	
	return -1;
}

static int RegisterOption( const char* _userName, const char* _userPassword , Client* _client )
{
	Client_err rcvFromRegiter;
	
	rcvFromRegiter = User_Register( _userName, _userPassword, _client);
	if(CLIENT_SUCCESS == rcvFromRegiter)
	{
		printf("\033[2J\033[;H");
		printf(ANSI_COLOR_GREEN);
		printf("\t\tSuccessfully registeration\n");
		printf(ANSI_COLOR_RESET);
	}

	else if( CLIENT_FAIL == rcvFromRegiter ) 
	{
		printf("\033[2J\033[;H");		
		printf(ANSI_COLOR_RED);
		printf("\t\tFail! try again\n");
		printf(ANSI_COLOR_RESET);
		
		
		return -1;
	}
	return 1;
}


static char* Init_GroupName()
{

	char* newGroupName = NULL;
	
	newGroupName =  (char*)malloc( sizeof(char) );
	if(NULL == newGroupName)
	{
		return 0;	
	}

	printf("Enter group name (Max- 16 tabs):");
	scanf( "%s", newGroupName );
		
	while( strlen( newGroupName ) > SIZE_OF_IP_ADDR )
	{
		printf("Try again\n");
		scanf( "%s", newGroupName );
	}	
	
	return newGroupName;
	
}

static char* Init_username()
{
	char* userName;

	userName =  (char*)malloc( sizeof(char) );
	if(NULL == userName)
	{
		return 0;	
	}
	
	printf("Enter user name (Max- 16 tabs):");
	scanf( "%s", userName );
	
	while( strlen( userName ) > MAX_SIZE )
	{
		printf("Try again\n");
		scanf( "%s", userName );
	}	
	
	return userName;
}

static char* Init_Psw()
{
	char* userPassword;
	
	userPassword =  (char*)malloc( sizeof(char) );
	if(NULL == userPassword)
	{
		return 0;	
	}
	
	printf("Password(Max- 16 tabs):");
	scanf("%s", userPassword );
	
	while( strlen( userPassword ) > MAX_SIZE )
	{
		printf("Try again\n");
		scanf("%s", userPassword );
	}
	
	return userPassword;

}

