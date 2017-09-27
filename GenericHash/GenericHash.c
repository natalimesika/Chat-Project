#include "HashMap.h"				

#include <stdio.h>						
#include <stdlib.h>	
					
#include "list.h"	
#include "list_itr.h"					

#define	MAGIC_NUMBER 52214221


/***********************************/
	/* HASH*/
/*create by Natali*/
/***********************************/
struct HashMap
{
	List** m_data;					
	size_t m_hashSize;			
	size_t m_allocSize;			
	size_t m_numOfItems;
	size_t m_magicNum;
																	
	HashFunction		m_hashFunc;
	EqualityFunction	m_keysEqualFunc;
};


struct DataItem
{
	void* m_value;
	void* m_key;
};

typedef struct DataItem DataItem_t;

struct Context
{
	KeyValueActionFunction	m_action;
	void* m_context;
	size_t m_counter;
};

typedef struct Context Context_t;

static int HashMapIsValid( const HashMap* _map )
{
	if ( NULL == _map || NULL == _map->m_data || MAGIC_NUMBER != _map->m_magicNum )
	{
		return 0;
	}
	
	return 1;
}



static int CheckIfPrime(const size_t _numToChec)
{
	size_t	i;
	int primeNum = 1;
	
	if ( 2 == _numToChec )
	{
		return primeNum;
	}
	
	if ( 0 == _numToChec % 2 )
	{
		return 0;
	}
	
	for ( i = 3 ; i*i <= _numToChec; i += 2 )
	{
		if ( 0 == _numToChec % i )
		{
			primeNum = 0;
			break;
		}
	}

	return primeNum;
}


static size_t FindPrimeLargerThan(size_t _number)
{
	while ( 0 == CheckIfPrime(_number) )
	{
		++_number;
	}
	
	return _number;
}


/*******************/
/*HashMap_Create*/
/*******************/
HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc )
{
	HashMap* newHash = NULL;
	List** hashTableArr = NULL;
	size_t newSize = 0;
	
	if ( 0 == _capacity || NULL == _hashFunc || NULL == _keysEqualFunc )
	{
		return NULL;	
	}
	
	newHash = calloc( 1 , sizeof( HashMap ) );
	if ( NULL == newHash )
	{
		return NULL;	
	}
	
	newSize = FindPrimeLargerThan( _capacity );
	
	hashTableArr = calloc( newSize , sizeof( List* ) );
	if ( NULL == hashTableArr )
	{
		free(newHash);
		return NULL;
	}
	
	newHash->m_data = hashTableArr;
	newHash->m_hashSize = _capacity;
	newHash->m_allocSize = newSize;
	newHash->m_hashFunc = _hashFunc;
	newHash->m_keysEqualFunc = _keysEqualFunc;
	newHash->m_magicNum = MAGIC_NUMBER;
	
	return newHash;
}


/*******************/
/*HashMap_Destroy*/
/*******************/
void HashMap_Destroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
	HashMap* localMap = NULL;
	List** hashTableArr	= NULL;
	DataItem_t* foundData = NULL;
	size_t hashSize = 0;
	size_t index = 0;
	
	if ( NULL == _map || ! HashMapIsValid(*_map) )
	{
		return;
	}
	
	localMap = (*_map);
	
	hashSize = localMap->m_allocSize;
	hashTableArr = localMap->m_data;
	
	while ( index < hashSize )
	{
		if ( NULL != hashTableArr[index] )
		{
			while ( 0 != List_PopHead( hashTableArr[index] , (void**) &foundData ) )
			{
				if ( NULL != _keyDestroy )
				{
					_keyDestroy( foundData->m_key );
				}
				if ( NULL != _valDestroy )
				{
					_valDestroy( foundData->m_value );
				}
				
				free( foundData );
			}
			
			List_Destroy( &hashTableArr[index] , NULL );
		}
		++index;
	}
	
	free( localMap->m_data );
	localMap->m_data = NULL;
	localMap->m_hashFunc = NULL;
	localMap->m_keysEqualFunc = NULL;
	localMap->m_hashSize = 0;
	localMap->m_allocSize = 0;
	localMap->m_magicNum = 0;
	
	free( localMap );
	
	(*_map) = NULL;
}


/*******************/
/*HashMap_Insert*/
/*******************/
Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
	EqualityFunction isEqual;
	List_Result err = LIST_SUCCESS;
	ListItr endItrator;
	ListItr itrator;
	List** hashTableArr	= NULL;
	DataItem_t* foundData = NULL;
	DataItem_t* DataItem = NULL;
	size_t index = 0;
	size_t hashSize = 0;
	if ( ! HashMapIsValid(_map)  )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	if ( NULL == _key )
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	hashSize		= _map->m_allocSize;
	index			= _map->m_hashFunc( _key );
	hashTableArr	= _map->m_data;
	isEqual			= _map->m_keysEqualFunc;
	
	
	index %= hashSize;
	if ( NULL == hashTableArr[index] )
	{
		hashTableArr[index] = List_Create();
	}
	else
	{
		endItrator = ListItr_End( hashTableArr[index] );
	
		for ( itrator = ListItr_Begin( hashTableArr[index] ) ;
			  ! ListItr_Equals( itrator , endItrator )		 ;
			  itrator = ListItr_Next( itrator ) )
		{
			foundData = ListItr_Get( itrator );
			if ( 1 == isEqual( foundData->m_key , _key ) )
			{
				break;
			}
		}
	
		if ( ! ListItr_Equals( itrator , endItrator ) )
		{

			return MAP_KEY_DUPLICATE_ERROR;
		}
	}
	
	DataItem = malloc( sizeof(DataItem_t) );
	if ( NULL == DataItem )
	{
		return MAP_ALLOCATION_ERROR;
	}
	
	DataItem->m_value	= (void*) _value;
	DataItem->m_key		= (void*) _key;
	
	err = List_PushHead( hashTableArr[index] , (void*) DataItem );
	if ( LIST_ALLOCATION_ERROR == err )
	{
		free( DataItem );
		return MAP_ALLOCATION_ERROR;
	}
	++(_map->m_numOfItems);
	return MAP_SUCCESS;
}



/*******************/
/*HashMap_Remove*/
/*******************/
Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
	EqualityFunction isEqual;
	ListItr endItrator;
	ListItr itrator;
	List** hashTableArr	= NULL;
	DataItem_t* foundData = NULL;
	size_t index = 0;
	size_t	hashSize = 0;
	
	if ( ! HashMapIsValid(_map) )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	if ( NULL == _searchKey )
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	hashSize = _map->m_allocSize;
	index = _map->m_hashFunc( _searchKey );
	hashTableArr = _map->m_data;
	isEqual = _map->m_keysEqualFunc;
	
	index %= hashSize;
	
	if ( NULL == hashTableArr[index] )
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}

	endItrator = ListItr_End( hashTableArr[index] );
	
	for ( itrator = ListItr_Begin( hashTableArr[index] ) ;
		  ! ListItr_Equals( itrator , endItrator )		 ;
		  itrator = ListItr_Next( itrator ) )
	{
		foundData = ListItr_Get( itrator );
		if ( 1 == isEqual( foundData->m_key , _searchKey ) )
		{
			break;
		}
	}
	
	if ( ListItr_Equals( itrator , endItrator ) )
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	foundData	= ListItr_Remove( itrator );
	(*_pKey)	= foundData->m_key;
	(*_pValue)	= foundData->m_value;
	
	free( foundData );
	
	--(_map->m_numOfItems);
	return MAP_SUCCESS;
}



/*******************/
/*HashMap_Size*/
/*******************/
size_t HashMap_Size(const HashMap* _map)
{
	if ( ! HashMapIsValid(_map) )
	{
		return 0;
	}

	return _map->m_numOfItems;
}


/*******************/
/*HashMap_Find*/
/*******************/
Map_Result HashMap_Find(const HashMap* _map, const void* __searchKey, void** _pValue)
{
	EqualityFunction	isEqual;
	ListItr				endItrator;
	ListItr				itrator;
	List**				hashTableArr	= NULL;
	DataItem_t*			foundData		= NULL;
	size_t				index			= 0;
	size_t				hashSize		= 0;

	if ( ! HashMapIsValid(_map) )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	if ( NULL == __searchKey )
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	hashSize = _map->m_allocSize;
	index = _map->m_hashFunc( __searchKey );
	hashTableArr = _map->m_data;
	isEqual = _map->m_keysEqualFunc;
	
	index %= hashSize;
	
	if ( NULL == hashTableArr[index] )
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}

	endItrator = ListItr_End( hashTableArr[index] );
	
	for ( itrator = ListItr_Begin( hashTableArr[index] ) ;
		  ! ListItr_Equals( itrator , endItrator )		 ;
		  itrator = ListItr_Next( itrator ) )
	{
		foundData = ListItr_Get( itrator );
		if ( 1 == isEqual( foundData->m_key , __searchKey ) )
		{
			break;
		}
	}

	if ( ListItr_Equals( itrator , endItrator ) )
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	(*_pValue)	= foundData->m_value;


	return MAP_SUCCESS;
}

