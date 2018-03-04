#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "tgshmr.h"

extern void dump_pkt(unsigned char *pbuf, int len);
extern int init_quene(void);
extern int sal_get_current_time(void);
extern int tg_reader_quene_packet(void);
extern int read_quene_thread_init(void);
int g_current_time = 0;

int main(void)
{
    int test_time ,count = 0;
    
    TGNL_MSG_HDR *p_tgnl_msg_recv = NULL;
    test_time = sal_get_current_time();
    tg_shm_reader_init();
    
    while(1)
    {
        p_tgnl_msg_recv = tg_shm_reader_next_packet();
       
        if(sal_get_current_time() - g_current_time > 5)
        {
            dump_pkt(p_tgnl_msg_recv->data,p_tgnl_msg_recv->data_len);
            g_current_time = sal_get_current_time();
        }
        
    }
    
    return 0;
    
}

void dump_pkt(unsigned char *pbuf, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		printf("%02x ", pbuf[i]);
		if (i%16 == 15)
		{
			printf("\n");
		}
	}
	printf("\n\n");
}


