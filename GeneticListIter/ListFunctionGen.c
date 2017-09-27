#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "list_itr.h"
#include "innar.h"
#include "list_functions.h"

/*
	All Function for Linked List

	Create by Natali Mesika
	Update 03.01.2017
*/

/***********find first***********/

ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	ListItr iterator;
	
	if(NULL == _begin || NULL == _end )
	{
		return NULL;
	}
	
	iterator = _begin;
	
	if(NULL != _predicate)
	{
		while(!ListItr_Equals(iterator, _end))
		{
			if(0 != _predicate(ListItr_Get(iterator), _context))
			{
				iterator = ListItr_Next(iterator);
			}
			else
			{
				return iterator;
			}
		}
	}
	
	return _end;
}
/***********ListItr CountIf***********/
size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	
	size_t counter = 0;
	ListItr iterator;
	
	if(_predicate != NULL)
	{
		for(iterator = _begin ; !ListItr_Equals(iterator, _end); iterator = ListItr_Next(iterator) )
		{
			if(0 != _predicate( ListItr_Get(iterator), _context) )
			{
				counter++;
			}
		
			return counter;
		}
	}
	return 0;
}


/***********ListItr_ForEach***********/

ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	ListItr iterator;
	
	if(NULL == _begin || NULL == _end )
	{
		return NULL;
	}
	
	if(NULL == _action)
	{
		return _end;		
	}

	iterator = _begin;
	
	while(!ListItr_Equals(iterator, _end))
	{
		if( 0 != _action (ListItr_Get(iterator), _context) )
		{			
			return iterator;
		}
		iterator = ListItr_Next(iterator);
	}
	
	return _end;
}

/***********ListItr Sort***********/

void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less)
{
	void*	prevValue	= NULL;
	void*	currValue	= NULL;
	
	if(NULL == _begin || NULL == _end )
	{
		return;
	}
	
	while(!ListItr_Equals(_begin, _end))
	{
		prevValue =  ListItr_Get(_begin);
		currValue =  ListItr_Get( ListItr_Next(_begin) );
		
		if(0 != _less( prevValue, currValue ) )
		{
				ListItr_Set(_begin, currValue );
				ListItr_Set( ListItr_Next(_begin), prevValue );
		}
		
		_begin = ListItr_Next(_begin);
	}
}
/***********ListItr Splice***********/
ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end)
{

	ListItr iterator;
	
	if(NULL == _begin || NULL == _end || NULL == _dest)
	{
		return _end;
	}	

	for(iterator = _begin ; !ListItr_Equals(iterator, _end);  )
	{
		iterator = ListItr_Next(iterator);
		
		ListItr_Next( ListItr_InsertBefore(_dest, ListItr_Remove( ListItr_Prev( iterator ) ) ) );
	}
	
	return _dest;
	
}

/***********ListItr Cut***********/
List* ListItr_Cut(ListItr _begin, ListItr _end)
{

	ListItr iter = _begin;
	List* newList;
	newList = List_Create();
	
	for( iter = _begin; iter != _end; iter = ListItr_Next(iter) )
	{
		List_PushHead(newList,  ListItr_Remove(iter));
	}
	
	return 	newList;
}
/***********ListItr Unique***********/
List* ListItr_Unique(ListItr _begin, ListItr _end, EqualsFunction _equals)
{
	List* newList;
	void* val_1;
	void* val_2;
	ListItr iter = _begin;
	
	newList = List_Create();

	for( iter = _begin; iter != _end; iter = ListItr_Next(iter) )
	{
		val_1 = ListItr_Get(iter);
		
		iter = ListItr_Next(iter);

		for( iter = _begin; iter != _end; iter = ListItr_Next(iter) )
		{
			val_2 = ListItr_Get(iter);
			
			if(_equals(val_1, val_2))
			{
				List_PushHead(newList,  ListItr_Remove(iter));
			}
		}
		
	}
	
	return 	newList;
}













