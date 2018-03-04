#ifndef __NL_SEV_H__
#define __NL_SEV_H__

#include <semaphore.h> 
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/socket.h>

#define TGNL_MAX_PAYLOAD 2048
#define SOCKET_BUF_SIZE  2000
#define MAX_LIST_NUM 1024

#define NETLINK_TGNL_TNMPD 31 /*与内核<linux/netlink.h>定义保持一致*/

#define TNMP_SEM_TYPE   sem_t *
#define SEM_TYPE sem_t

#define TNMP_MALLOC malloc
#define TNMP_FREE  free

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct
{
	u8      m_id; /*主ID*/
	u8      s_id; /*子ID*/
	u8 		feedback;
	u8 		sub_cmd;
	u16     data_len;
	u16		reserved2;
	u8      data[0];
}TGNL_MSG_UDP_HDR;

typedef struct
{
    u64     rxcount;
    u64     fullpkt;
    u64     inquene;
    u64     outquene;
    u64     outpkt;
    u64     showpkt;
}T_PKT_STAT;

extern T_PKT_STAT *g_stat_table;
#define PKT_STAT_GET(v) &g_stat_table[v];
#define STAT_MALLOC_VALID_OFFSET(a)     ((a) + (1024 - ((a) % 1024)))    

typedef enum{
    TGNL_AUDIT_PACKET_SEND_ID_S = 0x01,
    TGNL_AUDIT_USR_AUTH_USER_ID_S = 0x02,
    TGNL_AUDIT_STAT_SHOW_ID_S = 0x03,
    TGNL_AUDIT_PACKET_UNKNOW
}TGNL_AUDIT_ID_S;

typedef enum{
	TGNL_CONFIG_ID_M = 0x01,
    TGNL_USER_ID_M = 0x02,
	TGNL_L2TABLE_ID_M = 0x03,
	TGNL_RADIUS_ID_M = 0x04,
	TGNL_AUDIT_ID_M = 0x05,
    TGNL_MAX_ID_M = 0xff,
}TGNL_MAIN_ID;

#endif

