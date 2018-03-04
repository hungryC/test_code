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
#include "quene.h"

unsigned char pkt_buf[SOCKET_BUF_SIZE];
unsigned int g_packet_len;

extern void *tg_recv_pkt_thread(void);
extern TGNL_MSG_UDP_HDR *tg_reader_next_packet(void);
extern void tg_reader_quene_packet(void);
extern int tg_netlink_init(void);
extern TG_QUEUE g_pkt_quene_list;
extern int g_current_time;

int tgnl_sockfd = -1;

struct iovec tgnl_iov_recv;
struct msghdr tgnl_msg_recv_hdr;
struct nlmsghdr *tgnlh_recv = NULL;

SEM_TYPE sem_mutex;
TNMP_SEM_TYPE g_sem_mutex = &sem_mutex;
SEM_TYPE p_sem_mutex;
TNMP_SEM_TYPE g_empty_sem_mutex = &p_sem_mutex;   // 空的时候，对消费者不可进
SEM_TYPE c_sem_mutex;
TNMP_SEM_TYPE g_full_sem_mutex = &c_sem_mutex;   // 满的时候，对生产者不可进
u8 g_audit_buf[2048]={0};

T_PKT_STAT *g_stat_table = NULL;

int tg_pkt_stat_init(void)
{
    g_stat_table = (T_PKT_STAT *)TNMP_MALLOC(STAT_MALLOC_VALID_OFFSET(sizeof(T_PKT_STAT)));
    
    if(g_stat_table == NULL)
    {
        printf("stat table malloc fail\n\r");
        return -1;
    }
    
    memset(g_stat_table, 0, sizeof(T_PKT_STAT));
}
int tg_pkt_stat_vty(void)
{
    T_PKT_STAT *pstat = PKT_STAT_GET(0);
    pstat->showpkt ++;
    printf ("Send packet stats:\n\r");
    printf ("====================================\n\r");
    printf ("Receive total packets          : %-10lld \n\r", pstat->rxcount);
    printf ("Receive show packets           : %-10lld\n\r", pstat->showpkt);
    printf ("Receive inquene packets        : %-10lld \n\r", pstat->inquene);
    printf ("Receive outquene packets       : %-10lld\n\r", pstat->outquene);
    printf ("Receive fullquene counts       : %-10lld\n\r", pstat->fullpkt);
    printf ("Receive outpkt packets         : %-10lld\n\r", pstat->outpkt);
    printf ("====================================\n\r");
    
    return 0;
    
}
int sal_get_current_time(void)
{
    return time(NULL);
}

TNMP_SEM_TYPE sal_sem_create(TNMP_SEM_TYPE sem_id, int params)
{
    sem_init(sem_id,0,params);
    return sem_id;
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
int tg_shm_reader_init(void)
{
    init_quene();
    tg_netlink_init();
    tg_pkt_stat_init();
}
int tg_netlink_init(void)
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
    printf("tg share reader init successfully!\n\r");
    printf("%s %d pid=%d\n\r",__FUNCTION__,__LINE__,getpid());
    
    g_empty_sem_mutex = sal_sem_create(g_empty_sem_mutex, MAX_LIST_NUM);
    g_full_sem_mutex = sal_sem_create(g_full_sem_mutex, 0);
   
   
    //read_quene_thread_init();
    sal_create_thread(&tg_recv_pkt_thread, 0);
	return 0;
}

void *tg_recv_pkt_thread(void)
{
    TGNL_MSG_UDP_HDR *p_tgnl_msg_recv = NULL;
    printf("%s %d pid=%d\n\r",__FUNCTION__,__LINE__,getpid());
    while(1)
    {
       
        
        p_tgnl_msg_recv = tg_reader_next_packet();
        
        if(p_tgnl_msg_recv == NULL)
        {
            continue;
        }
        switch(p_tgnl_msg_recv->s_id)
        {
            case TGNL_AUDIT_STAT_SHOW_ID_S:
                tg_pkt_stat_vty();
                break;
            default:
                sal_sem_down(g_empty_sem_mutex);
                pkt_in_quene(&g_pkt_quene_list, p_tgnl_msg_recv->data, p_tgnl_msg_recv->data_len);
                sal_sem_up(g_full_sem_mutex);
                break;
        }
        
        
    }
}
#if 0
int read_quene_thread_init(void)
{
    sal_create_thread(&tg_reader_quene_packet, 0);
    return 0;
}

void tg_reader_quene_packet(void)
{
    
    TGNL_MSG_HDR *pPktNode = NULL;
    int pktId = 0;
    T_PKT_STAT *pstat = PKT_STAT_GET(0);
    
    while(1)
    {
        sal_sem_down(g_full_sem_mutex);
        pktId = pkt_out_quene(&g_pkt_quene_list);
        sal_sem_up(g_empty_sem_mutex);
        if (pktId < 0)
        {
            return ;
        }
        pPktNode = &g_pkt_quene_list.pPkt[pktId];
        if(NULL == pPktNode)
        {
            return ;
        }
        pstat->outpkt ++;
        if(sal_get_current_time() - g_current_time > 5)
        {
            dump_pkt(pPktNode->data,pPktNode->data_len);
            g_current_time = sal_get_current_time();
        }
        memset_pkt_queue(pktId);
        
    }
    
    return ;
}
#endif

TGNL_MSG_HDR *tg_shm_reader_next_packet(void)
{
    
    TGNL_MSG_HDR *pPktNode = NULL;
    int pktId = 0;
    T_PKT_STAT *pstat = PKT_STAT_GET(0);

    sal_sem_down(g_full_sem_mutex);
    pktId = pkt_out_quene(&g_pkt_quene_list);
    sal_sem_up(g_empty_sem_mutex);
    if (pktId < 0)
    {
        return NULL;
    }
    memcpy(g_audit_buf, &g_pkt_quene_list.pPkt[pktId], 2048);
    
    pPktNode = (TGNL_MSG_HDR *)g_audit_buf;
    
    if(NULL == pPktNode)
    {
        return NULL;
    }
    pstat->outpkt ++;
  
    memset_pkt_queue(pktId);
  
    return pPktNode;
}

#if 1
TGNL_MSG_UDP_HDR *tg_reader_next_packet(void)
{
    TGNL_MSG_UDP_HDR *p_tgnl_msg_recv = NULL;
    T_PKT_STAT *pstat = PKT_STAT_GET(0);
    unsigned int recv_len = 0;
    tgnlh_recv->nlmsg_pid = 0;
   
    memset((void *)NLMSG_DATA(tgnlh_recv), 0 ,SOCKET_BUF_SIZE);

    recv_len = recvmsg(tgnl_sockfd, &tgnl_msg_recv_hdr, MSG_WAITALL);
    
    p_tgnl_msg_recv = (TGNL_MSG_UDP_HDR *)NLMSG_DATA(tgnlh_recv);

    if(NULL == p_tgnl_msg_recv)
    {
        return 0;
    }
    
    pstat->rxcount ++;
   
    return p_tgnl_msg_recv;
}
#endif


