#include "PoolMultyCastAdd.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GenericQueue.h"

#define CAPACITY 50
#define SUBNET_IP "229.1."
#define STRING_SIZE 17

/*TODO 

change names*/

struct IP_Pool_Addr
{
	Queue* m_queue_addr;
	size_t m_poolSize;
};


static int InsertAllIPAddrToList(IP_Pool_Addr* _addr);

IP_Pool_Addr* CreateIP_Multecaste()
{
	IP_Pool_Addr* newQueue = NULL;
	
	newQueue = (IP_Pool_Addr*)malloc(sizeof( IP_Pool_Addr));
	if( NULL == newQueue)
	{
		return NULL;
	}
	
	newQueue->m_queue_addr = QueueCreate( CAPACITY );
	if( NULL == newQueue->m_queue_addr)
	{
		return NULL;
	}
	
	newQueue->m_poolSize = CAPACITY;
	
	InsertAllIPAddrToList( newQueue );
	
	return newQueue;
}

void DestroyIP_Multycaste( IP_Pool_Addr* _addr )
{
	if( NULL == _addr)
	{
		return;
	}

	QueueDestroy( _addr->m_queue_addr );

	free( _addr );
}

IP_Addr_Err GetIPAddress( IP_Pool_Addr* _addr, char* _ipAddr )
{
	QueueErr err;
	char* res;
	
	if( NULL == _addr)
	{
		return ADDR_INSERT_FAIL;
	}

	err = QueueRemove( _addr->m_queue_addr, (void*)&res );
	if( err != QERR_OK)
	{
		return ADDR_INSERT_FAIL;	
	}
	
	strcpy( _ipAddr,res );

	_addr->m_poolSize--;
	
	return ADDR_SUCCESS;

}	

IP_Addr_Err ReturnIPAddress( IP_Pool_Addr* _addr ,char** _Ipaddress)
{
	QueueErr res;
	
	if( NULL == _addr || NULL == *_Ipaddress || NULL == _Ipaddress)
	{
		return ADDR_NOT_EXIST;
	}
	
	res = QueueInsert( _addr->m_queue_addr , *_Ipaddress);
	if( res != QERR_OK)
	{
		QueueDestroy( _addr->m_queue_addr );
		return ADDR_INSERT_FAIL;	
	}

	*_Ipaddress = NULL;
	
	return ADDR_SUCCESS;

}
/********************************************************************************/
static int InsertAllIPAddrToList(IP_Pool_Addr* _addr)
{

	QueueErr res;
	int i;
	char* newIP;
	unsigned char addr1 = 1;
	unsigned char addr0 = 0;
	
	for (i = 0; i < CAPACITY; ++i)
	{
		newIP = (char*)malloc( sizeof(char) * STRING_SIZE);
		if( NULL == newIP)
		{
			QueueDestroy( _addr->m_queue_addr );
			return -1;
		}
		
		sprintf( newIP,"229.1.%d.%d",addr0, addr1 );
		
		res = QueueInsert( _addr->m_queue_addr , newIP);
		if( res != QERR_OK)
		{
			QueueDestroy( _addr->m_queue_addr );
			return -1;	
		}
		
		addr1++;
		if( addr1 == 255)
		{
			addr1 = 1;
			addr0++;
		}
	}
	
	
	return 1;
}








