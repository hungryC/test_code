#ifndef __QUEUE_H__  
#define __QUEUE_H__  
#include "nl_sev.h"

#define bool    int
#define true    1
#define false   0


typedef struct {
    u8 data[2044];
    u32 data_len;
}TGNL_MSG_HDR;

typedef struct queue   
{  
	TGNL_MSG_HDR pPkt[1024];  
	int front;    //ָ����е�һ��Ԫ��  
	int rear;    //ָ��������һ��Ԫ�ص���һ��Ԫ��  
}TG_QUEUE;  

extern int init_quene(void);
extern int pkt_in_quene(TG_QUEUE *pkt_quene, u8 *data_buf, u32 data_len);
extern int pkt_out_quene(TG_QUEUE *pkt_quene);
extern int memset_pkt_queue(int pktId);
#endif 
