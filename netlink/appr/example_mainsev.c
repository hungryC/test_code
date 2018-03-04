#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "tgshmr.h"
extern void dump_pkt(unsigned char *pbuf, int len);

extern int sal_get_current_time(void);
extern int tg_reader_lst_packet(void);
extern void pkt_list_init();
int main(void)
{
    pkt_list_init();
    tg_shm_reader_init();
   
    while(1)
    {
        tg_reader_lst_packet();
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


