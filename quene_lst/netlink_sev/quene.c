#include<stdio.h>
#include<stdlib.h>
#include "quene.h"

#define MAX_LIST_NUM 1024
TG_QUEUE g_pkt_quene_list = {0};
extern T_PKT_STAT *g_stat_table;


/***********************************************
 init a empty quene;
************************************************/
int init_quene(void)
{
    return 0;
}

bool full_quene(TG_QUEUE *pkt_quene)
{
	if(pkt_quene->front == ((pkt_quene->rear+1) % MAX_LIST_NUM))     /*判断循环队列是否满，留一个预留空间不用*/
	{
         return true;
    }
	else
		return false;
}

bool empty_quene(TG_QUEUE *pkt_quene)
{
    //printf("%s %d front = %d rear = %d\n\r",__FUNCTION__,__LINE__,pkt_quene->front,pkt_quene->rear);
	if(pkt_quene->front == pkt_quene->rear)     /*判断是否为空*/
	{
        return true;
    }	
	else
		return false;
}

int pkt_in_quene(TG_QUEUE *pkt_quene, u8 *data_buf, u32 data_len)
{
    int temp;
    T_PKT_STAT *pstat = PKT_STAT_GET(0);
	if(full_quene(pkt_quene))
	{
        printf("The queue is full!\n\r");
        pstat->fullpkt ++;
        return -1;
    }
    pstat->inquene ++;
    memcpy(pkt_quene->pPkt[pkt_quene->rear].data, data_buf, data_len);
    pkt_quene->pPkt[pkt_quene->rear].data_len = data_len;
    temp = pkt_quene->rear;
    temp = ((temp + 1) % MAX_LIST_NUM);
	pkt_quene->rear = temp;
	return 0;
	
}

int pkt_out_quene(TG_QUEUE *pkt_quene)
{
    int temp;
    T_PKT_STAT *pstat = PKT_STAT_GET(0);
    if(empty_quene(pkt_quene))
    {
        return -1;
    }
    pstat->outquene ++;
    temp = pkt_quene->front;
    pkt_quene->front = ((temp + 1) % MAX_LIST_NUM);
    
    return temp;
}

int memset_pkt_queue(int pktId)  
{  
    memset((void *)&g_pkt_quene_list.pPkt[pktId], 0, sizeof(TGNL_MSG_HDR)); 
    return 0;
}  

