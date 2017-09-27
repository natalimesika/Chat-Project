#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HashMap.h"
#include "list.h"
#include "list_itr.h"
#include "innar.h"
#include "mu_test.h"


/***********************************/
	/* HASH*/
/*create by Natali*/
/*UpDate: 11.01.2017*/
/***********************************/


/***********************************/
/* USER FUNCTION*/
/***********************************/
size_t HashFunc(const void* _key);

unsigned long hash(unsigned char *str);

int EqualityFunctionForString(const void* _firstKey, const void* _secondKey);

size_t NextPrimeFinder(size_t _num);

int	action(const void* _key, void* _value, void* _context);

void MykeyDestroy(void* _key);

void MyvalDestroy(void* _value);

/*the key is void pointer*/


size_t HashFunc(const void* _key)
{
	size_t res;
	
	res = hash(_key);
	
	return res;
}


unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c == *str++)
	{
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

void MykeyDestroy(void* _key)
{
	free(_key);
	
	return;
}

void MyvalDestroy(void* _value)
{
	free(_value);
	
	return;
}
/*comper between two keys in the for insert and in the remove
return 1 if the is duplicate and 0 if not */
int EqualityFunctionForString(const void* _firstKey, const void* _secondKey)
{
	if(0 == strcmp( (char*)_firstKey, (char*)_secondKey));
	{
		return 1;
	}
	return 0;

}

/*action on one element, i give the user key, value and contex*/

int	action(const void* _key, void* _value, void* _context)
{
	printf("%s" , _key);
	printf("%s" , _value);
	
	return 1;
}

size_t NextPrimeFinder(size_t _num)
{
    size_t i, j, flag;
   
    if(1 == _num)
    {
        return 2;
    }
   
    for(i = _num; i < (_num *_num) ; ++i) /*  from user size till next prime*/
    {
        flag = 0;

        for(j = 2; j < i; ++j)
        {
            if(0 == i % j)
            {
                flag = 1;
            }
        }
       
        if(0 == flag)
        {
            return i;
        }
    }
}


void elementDestroy(void* _item)
{
    if(_item != NULL)
    {
        free(_item);
        return;
    }
}

/***********************************/
/* main*/
/***********************************/
UNIT(HashMap_Create_check)

	HashMap* newHash;
	
	newHash =  HashMap_Create(12, HashFunc, EqualityFunctionForString);
	ASSERT_THAT(NULL != newHash);
	/*destroy*/
	
END_UNIT

UNIT(HashMap_insert_check)

	HashMap* newHash;
	Map_Result res;
	char* key = "Natali";
	char* val = "Omer";
	
	newHash =  HashMap_Create(12, HashFunc, EqualityFunctionForString);
	ASSERT_THAT(NULL != newHash);
	
	res = HashMap_Insert(newHash, key, val);
	ASSERT_THAT(MAP_SUCCESS == res);
	
	
	/*destroy*/
	
END_UNIT

TEST_SUITE(Test For Hash)  /* main */
   
    TEST(HashMap_Create_check)
	TEST(HashMap_insert_check)  
	  
END_SUITE 

