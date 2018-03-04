#ifndef __TGSHM_H__
#define __TGSHM_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define TG_SHM_MTU (2048)
#define TG_SHM_BLOCK_NUM (5120)
#define TG_SHM_MAX_SIZE (TG_SHM_BLOCK_NUM*TG_SHM_MTU)
#define TG_PACKET_SHM_KEY (5689)
#define TG_SHM_SEM "/tg_shm_sem"


typedef struct
{
	u8 block[TG_SHM_MTU];
}T_SHM_BLOCK;

typedef struct tg_shm_header
{
	int len;
	unsigned char data[0];
}T_SHM_HEADER;




#endif

