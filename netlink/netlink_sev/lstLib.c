/**************************************************************************************************************
**  IAG --- Integrated application gateway system
**  lstLib.c - double linked list
**  
***************************************************************************************************************/
/*  
* next node of list is the first node, prev node of list is the last node  
* prev of first node is NULL, next of last node is NULL   
*/   
   
#include <stdlib.h>   
#include "lstLib.h"   
   
#define HEAD    node.next       /* first node in list */   
#define TAIL    node.prev       /* last node in list */   
   
void lstLibInit(void)   
{   
    return;   
}   
   
void lstInit(LIST *pList)   
{   
    pList->HEAD = NULL;   
    pList->TAIL = NULL;   
    pList->count = 0;   
}   
   
NODE *lstFirst(LIST *pList )   
{   
    return (pList->HEAD);   
}   
   
NODE *lstLast(LIST *pList)   
{   
    return (pList->TAIL);   
}   
   
NODE *lstNext(NODE *pNode)   
{   
    return (pNode->next);   
}   
   
NODE *lstPrev(NODE *pNode)   
{   
    return (pNode->prev);   
}   
   
int lstCount(LIST *pList)   
{   
    return (pList->count);   
}   
   
/*  
* lstInsert - insert a node in a list after a specified node  
*  
* this routine inserts a specified node in a specified lst.  
* the new node is placed following the list node <pPrev>.  
* if <pPrev> is NULL, the node is inserted at the head of the list.  
*/   
   
void lstInsert(LIST *pList, NODE *pPrev, NODE *pNode)   
{   
    NODE *pNext;   
   
    if (pPrev == NULL)   
    {   
        /* new node is to be first in list */   
        pNext = pList->HEAD;   
        pList->HEAD = pNode;   
    }   
    else   
    {   
        /* make prev node point fwd to new */   
        pNext = pPrev->next;   
        pPrev->next = pNode;   
    }   
   
    if (pNext == NULL)   
    {   
        /* new node is to be last in list */   
        pList->TAIL = pNode;   
    }   
    else   
    {   
        /* make next node point back to new */   
        pNext->prev = pNode;    
    }   
   
    /* set pointers in new node */   
    pNode->next = pNext;   
    pNode->prev  = pPrev;   
   
    /* update node count */   
    pList->count++;   
}   
   
void lstAdd(LIST *pList, NODE *pNode)   
{   
    lstInsert(pList, pList->TAIL, pNode);   
}   
   
void lstDelete(LIST *pList, NODE *pNode)   
{   
    if(pNode->prev == NULL)   
        pList->HEAD = pNode->next;   
    else   
        pNode->prev->next = pNode->next;   
   
    if(pNode->next == NULL)   
        pList->TAIL = pNode->prev;   
    else   
        pNode->next->prev = pNode->prev;   
   
    pList->count--;   
}   
   
/* returns the node number of a specified node (the first node is 1), or -1, if the node is not found */   
   
int lstFind(LIST *pList, NODE *pNode)   
{   
    NODE *pNextNode;   
    int index = 1;   
   
    pNextNode = lstFirst(pList);   
   
    while ((pNextNode != NULL) && (pNextNode != pNode))   
    {   
        index++;   
        pNextNode = lstNext(pNextNode);   
    }   
   
    if (pNextNode == NULL)   
        return (-1);   
       
    return (index);   
}   
   
/* turns any list into an empty list. It also frees up memory used for nodes. */   
   
void lstFree(LIST *pList)   
{   
    NODE *p1, *p2;   
       
    if (pList->count <= 0)   
        return;   
       
    p1 = pList->HEAD;   
       
    while(p1 != NULL)   
    {   
        p2 = p1->next;   
        free ((char *)p1);   
        p1 = p2;   
    }   
       
    pList->count = 0;   
    pList->HEAD = pList->TAIL = NULL;   
} 



