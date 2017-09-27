#include "GenericQueue.h"

#include <stdio.h>
#include <stdlib.h>


#define MEGIC_NUM 873613

/*
	Create by: Natali Mesika
		16.01.2017
*/

struct Queue
{
	void** m_queueArray;
	size_t m_size;	  
	size_t m_head;
	size_t m_tail;
	size_t m_nItems;    /*=0*/
	size_t m_magicNumber;
	
};

Queue* QueueCreate(size_t _size)
{
	Queue* newQueue = NULL;
	
	if(_size <= 1)
	{
		return NULL;
	}

	newQueue = malloc( sizeof(Queue) );
	if(NULL == newQueue)
	{
		return NULL;
	}
	
	newQueue->m_queueArray =(void**)calloc(_size, sizeof(void*) );
	
	if(NULL == newQueue->m_queueArray)
	{
		free(newQueue);
		
		return NULL;
	}
	
	newQueue->m_magicNumber = MEGIC_NUM;
	newQueue->m_size = _size;
	
	return newQueue;
}

void QueueDestroy(Queue* _queue)
{
	size_t i;
	size_t nItems = _queue->m_nItems;
	
	if(NULL == _queue || _queue->m_magicNumber != MEGIC_NUM )
	{
		return;
	}

	for(i = 0; i < nItems; ++i )
	{
		free( _queue->m_queueArray[i] );
		_queue->m_queueArray[i] = NULL;
	}
	
	free(_queue);
	_queue = NULL;
	
	return;
}


QueueErr QueueInsert(Queue* _queue, void* _item)
{

	if(NULL == _queue || NULL == _queue->m_queueArray || _queue->m_magicNumber != MEGIC_NUM || NULL == _item)
	{
		return QERR_NOT_INITIALIZED;
	}
	
	if(_queue->m_nItems == _queue->m_size)
	{
		return QERR_OVERFLOW;
	}
	
	/*put an item on the tail*/
	_queue->m_queueArray [_queue->m_tail] = _item;
	
	_queue->m_tail = (_queue->m_tail + 1) % (_queue->m_size);
	
	++(_queue->m_nItems);

	return QERR_OK;
}

QueueErr QueueRemove(Queue* _queue, void** _item)
{
	if(NULL == _queue ||  NULL == _queue->m_queueArray || _queue->m_magicNumber != MEGIC_NUM )
	{
		return QERR_NOT_INITIALIZED;
	}

	if(0 == _queue->m_size)
	{
		return QERR_UNDERFLOW;
	}
	
	*_item = _queue->m_queueArray [_queue->m_head];
	
	_queue->m_head = (_queue->m_head + 1) % _queue->m_size;
	
	--(_queue->m_nItems);
	
	return QERR_OK;
}

int QueueIsEmpty(Queue* _queue)
{

	if(NULL == _queue ||  NULL == _queue->m_queueArray || _queue->m_magicNumber != MEGIC_NUM)
	{
		return -1;
	}

	if(_queue->m_nItems == 0 && _queue->m_head == _queue->m_tail)
	{
		return 1;
	}
	
	return 0;
	
}

