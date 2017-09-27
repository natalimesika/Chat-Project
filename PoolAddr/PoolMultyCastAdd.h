#ifndef __POOL_ADDRESS_H__
#define __POOL_ADDRESS_H__

typedef struct IP_Pool_Addr IP_Pool_Addr;


typedef enum IP_Addr_Err{

	ADDR_NOT_EXIST, 
	ADDR_INSERT_FAIL,
	ADDR_SUCCESS

}IP_Addr_Err;



IP_Pool_Addr* CreateIP_Multecaste();

void DestroyIP_Multycaste( IP_Pool_Addr* _addr );

IP_Addr_Err GetIPAddress( IP_Pool_Addr* _addr, char* _ipAddr );

IP_Addr_Err ReturnIPAddress( IP_Pool_Addr* _addr ,char** _Ipaddress);

#endif /* __POOL_ADDRESS_H__ */

