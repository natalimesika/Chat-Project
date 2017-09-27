#include "list.h"
#include "list_itr.h"
#include "innar.h"

#include <stdio.h>
#include <stdlib.h>

#define ITR_IS_TAIL(itr) ( ( (Node*)(itr) )->m_next == (itr) )


/*
	Iterator for Linked List

	Create by Natali Mesika
	Update 30.12.2016
*/ 

/*find the begin of the list*/
ListItr ListItr_Begin(const List* _list)
{
	
	if(NULL == _list)
	{
		return NULL;
	}

	return _list->head.m_next;
}
ListItr ListItr_End(const List* _list)
{
	if (NULL == _list)
	{
		return NULL;
	}
	/*if list is empty - return the tail*/
	if ( (_list->tail).m_prev == &(_list->head) )
	{
		return (_list->tail).m_next;
	}
	
	return (_list->tail).m_next;
}

int ListItr_Equals(const ListItr _a, const ListItr _b)
{
	if (NULL == _a || NULL == _b)
	{
		return 0;
	}
	
	if ( ((Node*)_a)->m_data == ((Node*)_b)->m_data)
	{
		return 1;
	}
	
	return 0;
}

ListItr ListItr_Next(ListItr _itr)
{
	if (NULL == _itr)
	{
		return NULL; 
	}
	return ((Node*)_itr)->m_next; 
}

ListItr ListItr_Prev(ListItr _itr)
{
	Node* node; 
	if (NULL == _itr)
	{
		return NULL; 
	}
	node = ((Node*)_itr)->m_prev; 
	if (node->m_prev == node)
	{
		return _itr;
	}
	
	return ((Node*)_itr)->m_prev; 
}

void* ListItr_Get(ListItr _itr)
{
	/*what happens if get is pointing at the head?*/
	if (NULL == _itr || ((Node*)_itr)->m_next == (Node*)_itr)
	{
		return NULL; 
	}
	return ((Node*)_itr)->m_data; 
}


void* ListItr_Set(ListItr _itr, void* _element)
{
	void* item;
	if (NULL == _itr || NULL == _element)
	{
		return NULL; 
	}
	
	item = ((Node*)_itr)->m_data;
	((Node*)_itr)->m_data = _element; 	
	return item;
}

ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{
	Node* newElement; 
	if (NULL == _element || NULL == _itr)
	{
		return NULL; 
	}
	
	newElement = malloc(sizeof(Node*));
	
	if (NULL == newElement)
	{
		return NULL; 
	}
	newElement->m_data = _element; 
	newElement->m_next = (Node*)_itr; 
	newElement->m_prev =  ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev = newElement;
	newElement->m_prev->m_next = newElement; 
	
	return newElement; 
}

void* ListItr_Remove(ListItr _itr)
{
	Node* nodeToRemove;
	void* data;
	if (NULL == _itr || ListItr_Next(_itr) == _itr)
	{
		return NULL; 
	}
	
	data = ((Node*)_itr)->m_data; 
	nodeToRemove= _itr; 
	nodeToRemove->m_next->m_prev= nodeToRemove->m_prev;
	nodeToRemove->m_prev->m_next = nodeToRemove->m_next;
	
	free(_itr);
	
	return data;
}




