#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mu_test.h" 
#include "list_itr.h"
#include "list.h"


/*
	/******Test Linked List*****/

/*	Create by Natali Mesika
	Update 03.01.2017
*/

#define LOOPNUM 50

int* CreatElement(int data)
{
    int *iptr = malloc(sizeof(int));

	if(NULL == iptr)
	{
		return NULL;
	}
    *iptr = data;
    
    return iptr;
}

void elementDestroy(void* _item)
{
    if(_item != NULL)
    {
        free(_item);
        return;
    }
}

/****************************/
/*TEST FOR BEGIN*/
/****************************/

UNIT(check_ListItr_Begin_with_no_elements)

    List* newList;
	ListItr iter;
	ListItr nexIter;

    newList = List_Create(); 
	ASSERT_THAT(NULL != newList);
	
	iter = ListItr_Begin(newList);
	
	nexIter = ListItr_Next(iter);
	ASSERT_THAT(nexIter == iter);
	
    List_Destroy(&newList, NULL);
	ASSERT_THAT(NULL == newList);
	
END_UNIT


UNIT(check_ListItr_Begin_with_elements)

    List_Result err;
    List* newList;
	ListItr iter;
	
    int* num;
    int i;
    
	newList = List_Create(); 
	ASSERT_THAT(NULL != newList);

    srand(time(NULL));
    for(i = 0; i < LOOPNUM; ++i)
    {    
        num = CreatElement( rand() % 50);
        err = List_PushHead(newList, num);
        ASSERT_THAT(err == LIST_SUCCESS);
    }
	iter = ListItr_Begin(newList);
	ASSERT_THAT(NULL != iter);

	List_Destroy(&newList, elementDestroy);
	ASSERT_THAT(NULL == newList);
	
END_UNIT


/****************************/
/*TEST FOR END*/
/****************************/



UNIT(check_ListItr_End_with_no_elements)

    List* newList;
	ListItr iter;

    newList = List_Create(); 
	ASSERT_THAT(NULL != newList);
	
	iter = ListItr_Begin(newList);
	ASSERT_THAT(NULL != iter);	
	
	iter = ListItr_End(iter);
	ASSERT_THAT(iter == iter);
	
    List_Destroy(&newList, NULL);
	ASSERT_THAT(NULL == newList);
	
END_UNIT



/****************************/
/*TEST FOR Equals*/
/****************************/
UNIT(ListItr_Equals_a_is_NULL)
	int isEquals;
	ListItr b = 0;
	printf("\n");
	
	isEquals = ListItr_Equals(NULL,b);
	ASSERT_THAT(1 == isEquals);
END_UNIT

UNIT(ListItr_Equals_b_is_NULL)
	int isEquals;
	ListItr a = 0;

	isEquals = ListItr_Equals(a,NULL);
	ASSERT_THAT(1 == isEquals);
END_UNIT

UNIT(ListItr_Equals_NULL_a_and_b)
	int isEquals;

	isEquals = ListItr_Equals(NULL,NULL);
	ASSERT_THAT(1 == isEquals);
END_UNIT


UNIT(check_ListItr_Equals_2_iters_point_on_1_list)

    List* newList;
	ListItr iter;
	ListItr iter2;
	
    int* num;
    int isEquals;
    
	num = (int*)malloc(sizeof(int));
    *num = 10;
    
    newList = List_Create(); 
	ASSERT_THAT(NULL != newList);
   
    List_PushHead(newList,num );

    iter = ListItr_Begin(newList);
    iter2 = ListItr_Begin(newList);
    isEquals = ListItr_Equals(iter2, iter);
	ASSERT_THAT(isEquals == 1);
	
	List_Destroy(&newList, elementDestroy);
	ASSERT_THAT(NULL == newList);


END_UNIT

UNIT(check_ListItr_Equals_2_iters_point_on_2_lists)

    List* firstList;
    List* secondList;
	ListItr iter1List;
	ListItr iter2List;
	
    int* num;
    int isEquals;
    
	num = (int*)malloc(sizeof(int));
    *num = 10;
    
    firstList = List_Create(); 
	ASSERT_THAT(NULL != firstList);
	
	secondList = List_Create(); 
	ASSERT_THAT(NULL != secondList);
   
    List_PushHead(firstList,num );
    List_PushHead(secondList,num );

    iter1List = ListItr_Begin(firstList);
    iter2List = ListItr_Begin(secondList);
    isEquals = ListItr_Equals(iter1List, iter2List);
	ASSERT_THAT(isEquals == 0);
	
	List_Destroy(&secondList, elementDestroy);
	ASSERT_THAT(NULL == secondList);
	
	List_Destroy(&firstList, elementDestroy);
	ASSERT_THAT(NULL == firstList);

END_UNIT


/****************************/
/*TESTS FOR ITER NEXT*/
/****************************/

UNIT(Check_Next_iter)

    List* newList;
	ListItr fIterList;
	ListItr sIterList;
    int* num;
    int isEquals;
    
	num = (int*)malloc(sizeof(int));
    *num = 10;
    printf("\n");
    
    newList = List_Create(); 
	ASSERT_THAT(NULL != newList);
	
    List_PushHead(newList,num );
    
    fIterList = ListItr_Begin(newList);
    sIterList = ListItr_Begin(newList);
    
    fIterList  = ListItr_Next(fIterList);
    
    isEquals = ListItr_Equals(fIterList, sIterList);
	ASSERT_THAT(isEquals == 0);
	
    sIterList  = ListItr_Next(sIterList);
	
	isEquals = ListItr_Equals(fIterList, sIterList);
	ASSERT_THAT(isEquals == 1);
	
	List_Destroy(&newList, elementDestroy);
	ASSERT_THAT(NULL == newList);

END_UNIT


/****************************/
/*TESTS FOR ITER PREV*/
/****************************/

UNIT(Check_Next_prev)

    List* newList;
	ListItr fIterList;
	ListItr sIterList;
    int* num;
    int isEquals;
    
	num = (int*)malloc(sizeof(int));
    *num = 10;
    printf("\n");
    
    newList = List_Create(); 
	ASSERT_THAT(NULL != newList);
	
    List_PushHead(newList,num );
    
    fIterList = ListItr_Begin(newList);
    sIterList = ListItr_Begin(newList);
    
    fIterList  = ListItr_Next(fIterList);
    fIterList  = ListItr_Prev(fIterList);

	ASSERT_THAT(fIterList == fIterList);
	
	List_Destroy(&newList, elementDestroy);
	ASSERT_THAT(NULL == newList);

END_UNIT

/****************************/
/*TESTS FOR ITER ListItr_Get*/
/****************************/
 
/****************************/
/*TESTS FOR LIST FUNCTION*/
/****************************/

/****************************/
/*TESTS ListItr_FindFirst*/
/****************************/
UNIT(check_ListItr_End_with_elements)

    List_Result err;
    List* newList;
	ListItr iter;
	ListItr begin;
	ListItr end;
	
    int* num;
    int i;

    printf("\n");

	newList = List_Create(); 
	ASSERT_THAT(NULL != newList);

    srand(time(NULL));
    for(i = 0; i < LOOPNUM; ++i)
    {    
        num = CreatElement( rand() % 50);
        err = List_PushHead(newList, num);
        ASSERT_THAT(err == LIST_SUCCESS);
    }
	
	iter = ListItr_Begin(newList);
	ASSERT_THAT(NULL != iter);	
	
	iter = ListItr_End(iter);
	ASSERT_THAT(iter = iter);


	List_Destroy(&newList, elementDestroy);
	ASSERT_THAT(NULL == newList);
	
END_UNIT
 

 
/****************************/
/*TESTS*/
/****************************/

TEST_SUITE(check  iter list:) 
   
    TEST(check_ListItr_Begin_with_no_elements)
    TEST(check_ListItr_Begin_with_elements)
    
    TEST(check_ListItr_End_with_elements)
    TEST(check_ListItr_End_with_no_elements)
    
    TEST(ListItr_Equals_a_is_NULL)
    TEST(ListItr_Equals_b_is_NULL)
    TEST(ListItr_Equals_NULL_a_and_b)
	TEST(check_ListItr_Equals_2_iters_point_on_1_list)
    TEST(check_ListItr_Equals_2_iters_point_on_2_lists)
	
	TEST(Check_Next_iter)
	TEST(Check_Next_prev)
    
END_SUITE 













