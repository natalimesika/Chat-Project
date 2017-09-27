#ifndef __LIST_SHEREG_FILES_H__
#define __LIST_SHEREG_FILES_H__


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


#endif 

