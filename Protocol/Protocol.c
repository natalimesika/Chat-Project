#include "Protocol.h"

#define MAX_LENGTH_USER_NAME 16

#include <arpa/inet.h>

int Pack( Protocol_Type _type, void* _value)
{
	unsigned char type_char = (unsigned char) _type;
	Register_Ans* _reg_ans_ptr;
	Login_Ans* _login_ans_ptr;
	Create_Group_Ans* create_ans_ptr;
	
	switch( _type )
	{
		case REGISTER_REQ:
		case LOGIN_REQ:
		{
			( (UserDetails*) _value)->m_type = type_char;
		}
		break;
		case CREATE_GROUP_CONVERSATION_REQ:
		case JOIN_CONVERSATION_REQ:
		{
			( (GroupDetails*) _value)->m_type = type_char;
		}
		break;
		case LEAVE_CONVERSATION_REQ:
		{
			( (GroupDetails*) _value)->m_type = type_char;
		}
		
		/*answers*/
		case REGISTER_ANS:
		{
			_reg_ans_ptr = _value; 					
			_reg_ans_ptr->m_type = _type;
			_reg_ans_ptr->m_answer = htonl(_reg_ans_ptr->m_answer );
		}
		break;
		case LOGIN_ANS:
		{
			_login_ans_ptr = _value; 						
			_login_ans_ptr->m_type = _type;
			_login_ans_ptr->m_answer = htonl(_login_ans_ptr->m_answer );
		}
		break;			
		case CREATE_GROUP_CONVERSATION_ANS:
		{
			create_ans_ptr = _value;
			create_ans_ptr->m_type = _type;
			create_ans_ptr->m_answer = htonl(create_ans_ptr->m_answer);
		}
		break;
		case JOIN_CONVERSATION_ANS:
		{
			create_ans_ptr = _value;
			create_ans_ptr->m_type = _type;
			create_ans_ptr->m_answer = htonl(create_ans_ptr->m_answer);
		}
		break;
		/*case LEAVE_CONVERSATION_ANS: 
		{
			create_ans_ptr = _value;
			create_ans_ptr->m_type = _type;
			create_ans_ptr->m_answer = htonl(create_ans_ptr->m_answer);
		
		}*/
		break;
	}
	
	return 0;
}


int UnPack( void* _value, Protocol_Type* _type )
{
	Register_Ans* _reg_ans_ptr;
	Login_Ans* _login_ans_ptr;
	Create_Group_Ans* create_ans_ptr;
	
	unsigned char copy = *( (char*) _value );
	*_type=  (Protocol_Type) copy;

	switch( *_type )
	{
		case REGISTER_REQ:
		case LOGIN_REQ:
		break;
		
		case CREATE_GROUP_CONVERSATION_REQ:
		case JOIN_CONVERSATION_REQ:
		break;
		
		
		/*answers*/
		case LOGIN_ANS:
		{
			_login_ans_ptr = _value;
			_login_ans_ptr->m_answer = ntohl( _login_ans_ptr->m_answer );
		}
		break;		
		case REGISTER_ANS:
		{
			_reg_ans_ptr = _value;
			_reg_ans_ptr->m_answer = ntohl( _reg_ans_ptr->m_answer );
		}
		break;
		case CREATE_GROUP_CONVERSATION_ANS:
		{
			create_ans_ptr = _value;		
			create_ans_ptr->m_answer = ntohl(create_ans_ptr->m_answer);
		}
		break;
		case JOIN_CONVERSATION_ANS:
		{
			create_ans_ptr = _value;		
			create_ans_ptr->m_answer = ntohl(create_ans_ptr->m_answer);
		}
		break;
		case LEAVE_CONVERSATION_ANS:
		{
			create_ans_ptr = _value;		
			create_ans_ptr->m_answer = ntohl(create_ans_ptr->m_answer);
		}
		break;
	}
	
	
	
	return 0;

}
