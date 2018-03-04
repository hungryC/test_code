#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

extern void tg_shm_reader_init(void);
extern int tg_shm_reader_next_packet(unsigned char **ppbuf);

char time_str[64];

char *time_fmt(void)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); /*取得当地时间*/
    sprintf (time_str, "%d-%d-%d %02d:%02d:%02d" , (1900+p->tm_year),( 1 + p->tm_mon), p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);
    return time_str;
}

int main(void)
{
	unsigned char *pbuf;
	int len;
	int i=10000;
    
	tg_shm_reader_init();

    printf("shem read start time %s\n\r",time_fmt());
	while (1)
	{
		len = tg_shm_reader_next_packet(&pbuf);

		//printf("length: %d, %s\n", len,  pbuf);
		
	}
    printf("shem read finish time %s\n\r",time_fmt());
}







