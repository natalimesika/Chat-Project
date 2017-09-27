#include "list.h" 

#include <stdio.h> 
#include <stdlib.h> 

#include "mu_test.h" 


/***********************************/
	/* LinkedList*/
/*create by Natali*/
/*UpDate: 30.12.2016*/
/***********************************/


typedef struct Node Node;

struct Node
{
    void* m_data;
    Node* m_next;
    Node* m_prev;
    
};

struct List
{
     Node head;
     Node tail;
};

List* List_Create()
{

    List* mylist;
    
    mylist = (List*)malloc(sizeof(List));
    
    if(NULL == mylist)
    {
        return NULL;
    }
    
    mylist->head.m_next = &(mylist->tail);
    mylist->head.m_prev = NULL;
    
    mylist->tail.m_prev = &(mylist->head); 
    mylist->tail.m_next = &(mylist->tail);;
   
    return mylist;
}
/*
1. iterator that run and sent the data to the function
2. this iterator free the nodes that on the list
*/

void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item))
{
    Node* iter = NULL;
   /* Node* nextIter;*/
    
    if(NULL == _pList || NULL == *_pList )
    {
        return;
    }
    for( iter = (*_pList)->head.m_next;
		 iter != &((*_pList)->tail);
		 iter = iter->m_next ,free(iter->m_prev) )
    {
		if(NULL != _elementDestroy)
		{
			_elementDestroy(iter-> m_data);
		}
	}

    free( *_pList );
    
    (*_pList) = NULL;

    return;
}

List_Result List_PushHead(List* _list, void* _item)
{
   
	Node* newNode = NULL;

    if(NULL == _list)
    {
    	return LIST_UNINITIALIZED_ERROR;
    }

    if(NULL == _item) 
    {
        return LIST_NULL_ELEMENT_ERROR;
    }
    
    newNode = (Node*)malloc(sizeof(Node));
    
    if(NULL == newNode)
    {
        return LIST_ALLOCATION_ERROR;
    }
   
    newNode->m_next = _list->head.m_next;
    newNode->m_prev = &(_list->head);

    _list->head.m_next = newNode;
    newNode->m_next->m_prev = newNode;
	
	newNode->m_data = _item;	
	
	return LIST_SUCCESS;
}


List_Result List_PushTail(List* _list, void* _item)
{
	Node* newNode;
	
    if(NULL == _list)
    {
    	return LIST_UNINITIALIZED_ERROR;
    }

	if(NULL == _item) 
    {
        return LIST_NULL_ELEMENT_ERROR;
    }
	
	newNode = (Node*)malloc(sizeof(Node));
	
	newNode->m_prev = _list->tail.m_prev;
	newNode->m_next = &(_list->tail);
	
	(_list->tail.m_prev)->m_next = newNode;
	newNode->m_next->m_prev = newNode;
	
	newNode->m_data = _item;	
	
    return LIST_SUCCESS;
}

List_Result List_PopHead(List* _list, void** _pItem)
{
    Node* removeFromH;

    if(NULL == _list || NULL ==  _pItem)
    {
    	return LIST_UNINITIALIZED_ERROR;
    }
    
    if(_list->head.m_next == &(_list->tail) )
	{
		return LIST_IS_EMPTY;
	}
	
    removeFromH = _list->head.m_next;
    
    *_pItem = removeFromH->m_data;
	
    _list->head.m_next = removeFromH->m_next;
    _list->head.m_next->m_prev = &(_list->head);
    
    free(removeFromH);
    	
    return LIST_SUCCESS;
}


List_Result List_PopTail(List* _list, void** _pItem)
{
	 Node* removeFromT;
	
    if(NULL == _list || NULL ==  _pItem)
    {
    	return LIST_UNINITIALIZED_ERROR;
    }
    
    if(_list->head.m_next == &(_list->tail) )
	{
		return LIST_IS_EMPTY;
	}
	
    removeFromT = _list->tail.m_prev;
    
    *_pItem = removeFromT->m_data;
	
	removeFromT->m_prev->m_next = &(_list->tail);
    _list->tail.m_prev = removeFromT->m_prev;
    
    free(removeFromT);

    return LIST_SUCCESS;
}

size_t List_Size(const List* _list)
{
	Node* counterTemp;
	int counter;
	
	if(NULL ==  _list) 
    {
        return 0;
    }
    counter = 0;
    
    for( counterTemp = _list->head.m_next;
		 counterTemp != &(_list->tail);
		 counterTemp = counterTemp->m_next )
    {
    	counter++;
    }

    return counter;
}




