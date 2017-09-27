#ifndef __Server_Manager_H__
#define __Server_Manager_H__


typedef struct ServerChat ServerChat;

ServerChat* CreateChat( Params _params);

void DestroyChat( ServerChat* _serverChat );



#endif /* __Server_Manager_H__ */

