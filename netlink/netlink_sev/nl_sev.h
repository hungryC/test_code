#ifndef __NL_SEV_H__
#include <semaphore.h> 
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include "lstLib.h"
#define __NL_SEV_H__
#define TGNL_MAX_PAYLOAD 2048
#define SOCKET_BUF_SIZE  2000

#define NETLINK_TGNL_TNMPD 31 /*与内核<linux/netlink.h>定义保持一致*/


int tgnl_sockfd = -1;

struct nlmsghdr *tgnlh_recv = NULL;
struct iovec tgnl_iov_recv;

struct msghdr tgnl_msg_recv_hdr;

#define TNMP_SEM_TYPE   sem_t *
#define SEM_TYPE sem_t

#define TNMP_MALLOC malloc
#define TNMP_FREE  free

SEM_TYPE sem_recv_list;
TNMP_SEM_TYPE g_sem_recv_list = &sem_recv_list;

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


typedef enum{
    TGNL_AUDIT_PACKET_SEND_ID_S = 0x01,
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

typedef struct {
    NODE node;
    u8 data[2048];
    u16 isUsed;
    u16 data_len;
}TGNL_MSG_HDR;
extern TGNL_MSG_HDR *g_list_pkt;
#define lst_find(id) &g_list_pkt[id%MAX_LIST_NUM]

#endif

