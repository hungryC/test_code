#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

extern void tg_shm_writer_init(void);
extern int  tg_shm_writer_index(void);
extern int tg_shm_writer_send(const unsigned char *pbuf, int length);

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
    int loop=1000000;
	unsigned char test_buf[2044] = {0};
	unsigned int now_time,end_time,period_time;
	tg_shm_writer_init();
    now_time = time(NULL);
	printf("shem write start time %s\n\r",time_fmt());
	while (loop--)
	{
		sprintf(test_buf, "test%d", tg_shm_writer_index());

		tg_shm_writer_send(test_buf, 2044);
		
		//usleep(10000);
	}
    end_time = time(NULL);
    period_time = end_time - now_time;
	printf("shem write finish time %s period time %d\n\r",time_fmt(),period_time);
	return 0;
	
}

