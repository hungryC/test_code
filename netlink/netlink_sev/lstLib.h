/**************************************************************************************************************
**  STS --- system transfer station 
**  lstLib.h --- double linked lst header file
**  
***************************************************************************************************************/  
#ifndef __LSTLIB_H__
#define __LSTLIB_H__
typedef struct node 
{ 
    struct node *next; 
    struct node *prev; 
}NODE; 
 
typedef struct 
{ 
    NODE node; 
    int count; 
}LIST; 

extern void lstLibInit(void); 
extern NODE *lstFirst(LIST *pList); 
extern NODE *lstLast(LIST *pList); 
extern NODE *lstNext(NODE *pNode); 
extern NODE *lstPrev(NODE *pNode); 
extern int lstCount(LIST *pList); 
extern int lstFind(LIST *pList, NODE *pNode); 
extern void lstAdd(LIST *pList, NODE *pNode); 
extern void lstDelete(LIST *pList, NODE *pNode); 
extern void lstFree(LIST *pList); 
extern void lstInit(LIST *pList); 
extern void lstInsert(LIST *pList, NODE *pPrev, NODE *pNode); 

#endif


