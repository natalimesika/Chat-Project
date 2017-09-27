#include "GenericQueue.h"
#include "mu_test.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#define NUM_FOR_RANDOM 30


/***********************************/
/* main*/
/***********************************/
UNIT(Queue_create_normal)

	Queue* newQueue;
	
	newQueue = QueueCreate(18);
	ASSERT_THAT(NULL != newQueue);
	
END_UNIT

UNIT(Queue_create_With_1_on_size)

	Queue* newQueue;
	
	newQueue = QueueCreate(1);
	ASSERT_THAT(NULL == newQueue);
	
END_UNIT

UNIT(Queue_create_With_0_on_size)

	Queue* newQueue;
	
	newQueue = QueueCreate(0);
	ASSERT_THAT(NULL == newQueue);
	
END_UNIT

UNIT(Queue_destroy_normal)

	Queue* newQueue;
	
	newQueue = QueueCreate(10);
	ASSERT_THAT(NULL != newQueue);
	
	QueueDestroy(newQueue);
	ASSERT_THAT(NULL != newQueue);
END_UNIT

/*
UNIT(Queue_destroy_NULL_queue)

	Queue* newQueue;
	
	newQueue = QueueCreate(0);
	ASSERT_THAT(NULL == newQueue);
	
	QueueDestroy(newQueue);
	ASSERT_THAT(NULL == newQueue);
END_UNIT
*/


UNIT(Queue_insert_int_normal_1_item)

	Queue* newQueue;
	QueueErr err;
	int num = 722;
	
	newQueue = QueueCreate(10);
	ASSERT_THAT(NULL != newQueue);
	
	err = QueueInsert(newQueue,&num);
	ASSERT_THAT(err == QERR_OK);
	
END_UNIT


UNIT(Queue_insert_int_normal_random_numbers)

	Queue* newQueue;
	QueueErr err;
	int i;
	int* item;

	newQueue = QueueCreate(NUM_FOR_RANDOM);
	ASSERT_THAT(NULL != newQueue);
	
	srand(time(NULL));
	
	for(i = 0; i < NUM_FOR_RANDOM; ++i)
	{
		*item = rand()%50;
		err = QueueInsert(newQueue, &item);
	}

	ASSERT_THAT(0 > QueueIsEmpty(newQueue));
	
END_UNIT

UNIT(Queue_insert_int_normal_3_item)

	Queue* newQueue;
	QueueErr err;
	int num = 722;
	int num1 = 3234;
	int num2 = 234;
	
	newQueue = QueueCreate(10);
	ASSERT_THAT(NULL != newQueue);
	
	err = QueueInsert(newQueue,&num);
	ASSERT_THAT(err == QERR_OK);
	
	err = QueueInsert(newQueue,&num1);
	ASSERT_THAT(err == QERR_OK);
	
	err = QueueInsert(newQueue,&num2);
	ASSERT_THAT(err == QERR_OK);
	
END_UNIT




TEST_SUITE(Test For Queue)  /* main */
   
    TEST(Queue_create_normal)
	TEST(Queue_create_With_1_on_size)  
	TEST(Queue_create_With_0_on_size)  
	TEST(Queue_destroy_normal)
	TEST(Queue_insert_int_normal_1_item)
	TEST(Queue_insert_int_normal_random_numbers)
	TEST(Queue_insert_int_normal_3_item)
	
	  
END_SUITE


 
