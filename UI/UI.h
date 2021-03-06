#ifndef __UI_PROJECT_H__
#define __UI_PROJECT_H__

typedef struct Client_Interface Client_Interface;


Client_Interface* CreateCommunication( int _serverPort, const char* _serverIP);

void Destroy_UI( Client_Interface* _ui);

int RunUI(Client_Interface* _ui);

#endif
