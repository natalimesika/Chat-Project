#ifndef __PROTOCOL_PROJECT_H__
#define __PROTOCOL_PROJECT_H__

#define MAX_LENGTH_USER_NAME 16
#define MAX_LENGTH_PSW 16
#define MAX_LENGTH_GROUP_NAME 17
#define SIZE_OF_IP_ADDR 17

typedef struct GroupDetails GroupDetails;
typedef struct UserDetails UserDetails;

typedef struct Register_Ans Register_Ans; 
typedef struct Login_Ans Login_Ans; 
typedef struct Create_Group_Ans Create_Group_Ans;
typedef struct Join_Group_Ans Join_Group_Ans;
typedef struct Group_To_Leave Group_To_Leave; 

typedef enum Protocol_Type{

	REGISTER_REQ, 									
	REGISTER_ANS, 									
	LOGIN_REQ, 
	LOGIN_ANS, 		
	LOGOUT,
	UNREGISTER_REQ,
	UNREGISTER_ANS,
	CREATE_GROUP_CONVERSATION_REQ,
	CREATE_GROUP_CONVERSATION_ANS,	
	JOIN_CONVERSATION_REQ,								
	JOIN_CONVERSATION_ANS,								
	CREATE_PRIVATE_CONVERSATION,
	LEAVE_CONVERSATION_REQ,
	LEAVE_CONVERSATION_ANS,				
	GET_USER_LIST,		      
	GET_GROUP_CHAT_LIST

}Protocol_Type;

typedef enum Answers
{
	REGISTER_SUCCESS = 0,
	REGISTER_USER_NAME_ALLREADY_EXIST = 1,
	LOGIN_USER_SUCCESSFULLY_CONNECTED,
	LOGIN_USER_NAME_NOT_EXIST,
	LOGIN_WRONG_PSW,
	LOGIN_ALLREADY_CONNECTED,
	GROUP_SUCCESSFULLY_INSERT,
	GROUP_NAME_ALREADY_EXIST, 
	GROUP_NAME_NOT_EXIST,
	GROUP_SUCCESSFULLY_JOIN, 
	GROUP_SUCCESSFULLY_FOUND
	
	
}Answers;


struct UserDetails{ 
					     
	unsigned char m_type;
	char m_password[ MAX_LENGTH_PSW + 1 ]; 		  
	char m_userName[ MAX_LENGTH_USER_NAME + 1 ]; 
		
};
/*To use for create and join group req*/
struct GroupDetails{					 
	
	unsigned char m_type;
	char m_groupName[ MAX_LENGTH_GROUP_NAME + 1 ];
	char m_userName[ MAX_LENGTH_USER_NAME + 1 ];			
};

struct Group_To_Leave{					 
	
	unsigned char m_type;
	char m_groupName[ MAX_LENGTH_GROUP_NAME + 1 ];
	char m_userName[ MAX_LENGTH_USER_NAME + 1 ];
	char m_groupIP[ SIZE_OF_IP_ADDR ];
};

struct Create_Group_Ans
{
	unsigned char m_type;
	char m_groupIP[ SIZE_OF_IP_ADDR ];
	Answers m_answer;
};

struct Join_Group_Ans
{
	unsigned char m_type;
	char m_groupIP[ SIZE_OF_IP_ADDR ];
	Answers m_answer;
};

struct Register_Ans 
{
	unsigned char m_type; 
	Answers m_answer;
};


struct Login_Ans 
{
	unsigned char m_type; /* LOGIN_ANS */
	Answers m_answer;
};


int Pack( Protocol_Type _type, void* _value );

int UnPack( void* _value, Protocol_Type* _type );



#endif /* __PROTOCOL_PROJECT_H__*/
