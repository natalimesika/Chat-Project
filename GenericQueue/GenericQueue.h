#ifndef __GENERIC_QUEUE_H__
#define __GENERIC_QUEUE_H__

#include <stddef.h>

typedef enum
{
	QERR_OK,
	QERR_NOT_INITIALIZED,
	QERR_UNDERFLOW,
	QERR_OVERFLOW
} QueueErr;

typedef struct Queue Queue;

Queue* QueueCreate(size_t _size);

void QueueDestroy(Queue* _queue);

QueueErr QueueInsert(Queue* _queue, void* _item);

QueueErr QueueRemove(Queue* _queue, void** _item);

int QueueIsEmpty(Queue* _queue);


#endif /*__GENERIC_QUEUE_H__*/
