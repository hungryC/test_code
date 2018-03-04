#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h> 
#include "nl_sev.h"

#define MAX_LIST_NUM 1024
unsigned char pkt_buf[SOCKET_BUF_SIZE];
unsigned int g_packet_len;

LIST g_recv_pkt_list;
TGNL_MSG_HDR *g_list_pkt = NULL;
extern void *tg_recv_pkt_thread(void);
extern int tg_reader_lst_packet(void);
extern TGNL_MSG_UDP_HDR *tg_shm_reader_next_packet(void);
TNMP_SEM_TYPE sal_sem_create(TNMP_SEM_TYPE sem_id)
{
    sem_init(sem_id,0,1);
    return sem_id;
}

int sal_get_current_time(void)
{
    return time(NULL);
}

void sal_sem_release(TNMP_SEM_TYPE sem)
{
    sem_post(sem);
}
void sal_sem_down(TNMP_SEM_TYPE sem)
{
    sem_wait(sem);
}

void sal_sem_up(TNMP_SEM_TYPE sem)
{
    sem_post(sem);
}
int sal_create_thread(void *func, int param)
{
    pthread_t ntid;
    pthread_create(&ntid, NULL, func, NULL);
    return ntid;
}

void pkt_list_init()
{
    int nSize;
    nSize = sizeof(TGNL_MSG_HDR)*MAX_LIST_NUM;
    g_list_pkt = (TGNL_MSG_HDR*)TNMP_MALLOC(nSize);
    if(g_list_pkt == NULL)
    {
        printf("%s %d g_list_pkt malloc fail!\n\r",__FUNCTION__,__LINE__);
    }
     
    memset((void *)g_list_pkt, 0, (sizeof(TGNL_MSG_HDR)*MAX_LIST_NUM));
   
   // printf("%s %d g_list_pkt[0].isUsed = %d\n\r",__FUNCTION__,__LINE__,g_list_pkt[0].isUsed);
  
    lstInit(&g_recv_pkt_list);
    
    
}
TGNL_MSG_HDR *lst_get_free_node(void)
{
    int i;
   
    for(i = 0;i <MAX_LIST_NUM; i++)
    {
        if(!g_list_pkt[i].isUsed)
        {
            g_list_pkt[i].isUsed = 1;
            return &g_list_pkt[i];
        }
    }
   
    return NULL;
}
void lst_reset_pkt_node(TGNL_MSG_HDR *pktnode)
{
    memset((void *)pktnode, 0, sizeof(TGNL_MSG_HDR));
}
void lst_recv_pkt_inquene(TGNL_MSG_HDR *pktnode)
{
    lstAdd(&g_recv_pkt_list, (NODE *)pktnode);
   
}

void lst_recv_pkt_dequene(TGNL_MSG_HDR *pktnode)
{
    lstDelete(&g_recv_pkt_list, (NODE *)pktnode);
    lst_reset_pkt_node(pktnode);
}
TGNL_MSG_HDR *lst_recv_pkt_get_first(void)
{
    TGNL_MSG_HDR *pPktnode = NULL;
    pPktnode = (TGNL_MSG_HDR *) lstFirst(&g_recv_pkt_list);
   
    return pPktnode;
}

int tg_shm_reader_init(void)
{
	struct sockaddr_nl dest_addr;
	int rv = 0;
	pthread_t pid;

	tgnl_sockfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TGNL_TNMPD);
	if(-1 == tgnl_sockfd)
	{
		printf("can't create netlink socket!");
		return -1;
	}
	
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = PF_NETLINK;
	dest_addr.nl_pid = 0;
	dest_addr.nl_groups = 1;

	rv = bind(tgnl_sockfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
	if(-1 == rv)
	{
	  printf("can't bind sockfd with sockaddr_nl!");
	  return -1;
	}
   
	tgnlh_recv = (struct nlmsghdr *)malloc(NLMSG_SPACE(TGNL_MAX_PAYLOAD));
	if (NULL == tgnlh_recv)
	{
		return -1;
	}

	tgnlh_recv->nlmsg_pid = 0;
	tgnlh_recv->nlmsg_type = 0;
	tgnlh_recv->nlmsg_flags = 0;

	memset(&tgnl_iov_recv, 0, sizeof(struct iovec));
	tgnl_iov_recv.iov_base = (void *)tgnlh_recv;
    tgnl_iov_recv.iov_len = TGNL_MAX_PAYLOAD;

	memset(&tgnl_msg_recv_hdr, 0, sizeof(struct msghdr));
    tgnl_msg_recv_hdr.msg_name = (void *)&dest_addr;
    tgnl_msg_recv_hdr.msg_namelen = sizeof(dest_addr);
	tgnl_msg_recv_hdr.msg_iov = &tgnl_iov_recv;
	tgnl_msg_recv_hdr.msg_iovlen = 1;
    
    sal_create_thread(&tg_recv_pkt_thread, 0);
	return 0;
}

void *tg_recv_pkt_thread(void)
{
    int i;
    TGNL_MSG_HDR *pPktNode;
    TGNL_MSG_UDP_HDR *p_tgnl_msg_recv = NULL;
    while(1)
    {
        p_tgnl_msg_recv = tg_shm_reader_next_packet();
      
        pPktNode = lst_get_free_node();
        
        if(pPktNode == NULL)
        {
            printf("%s %d node don't free!\n\r",__FUNCTION__,__LINE__);
            continue;
        }
        memcpy(pPktNode->data, p_tgnl_msg_recv->data, p_tgnl_msg_recv->data_len);
        pPktNode->data_len = p_tgnl_msg_recv->data_len;
        
        lst_recv_pkt_inquene(pPktNode);
       
    }
}

int tg_reader_lst_packet(void)
{
    TGNL_MSG_HDR *pPktNode = NULL;
    
    pPktNode = lst_recv_pkt_get_first();
    if (NULL == pPktNode)
    {
        return -1;
    }
    printf("%s %d len = %d\n\r",__FUNCTION__,__LINE__,pPktNode->data_len);
    lst_recv_pkt_dequene(pPktNode);
    return 0;
}

u64 rxcount = 0;
#if 1
TGNL_MSG_UDP_HDR *tg_shm_reader_next_packet(void)
{
    TGNL_MSG_UDP_HDR *p_tgnl_msg_recv = NULL;
   
    unsigned int recv_len = 0;
    tgnlh_recv->nlmsg_pid = 0;
    
    memset((void *)NLMSG_DATA(tgnlh_recv), 0 ,SOCKET_BUF_SIZE);

    recv_len = recvmsg(tgnl_sockfd, &tgnl_msg_recv_hdr, MSG_WAITALL);
    
    p_tgnl_msg_recv = (TGNL_MSG_UDP_HDR *)NLMSG_DATA(tgnlh_recv);
    if(NULL == p_tgnl_msg_recv)
    {
        return 0;
    }
   
    return p_tgnl_msg_recv;
}
#endif


