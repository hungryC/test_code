#ifndef __TGSHR_H__
#define __TGSHR_H__
#include <linux/types.h>
#include <asm/byteorder.h>

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
	u16     data_len; /*数据长度*/  
	u16		reserved2;
	u8      data[0];    /*数据内容*/
}TGNL_MSG_UDP_HDR;


typedef struct T_datagram_audit
{
	u8	datagram_direction;	////数据报文 方向	0发，1收 2是未知方向
	u8	datagram_type;		////数据类型，0数据报文，1用户上线信息 2是用户下线
	u8	reserve[6];
    u8  auditdata[0];
}T_DATA_AUDIT;

struct ether_header {
    u8 dmac[6];
    u8 smac[6];
    u16 etype;
} __attribute__((packed));

struct tcphdr
  {
    u16 th_sport;		/* source port */
    u16 th_dport;		/* destination port */
    u32 th_seq;		/* sequence number */
    u32 th_ack;		/* acknowledgement number */
#if defined (__BIG_ENDIAN_BITFIELD)
    u8 th_off:4,     /* data offset */
        th_x2:4;      /* (unused) */
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    u8 th_x2:4,     /* (unused) */
        th_off:4;      /* data offset */
#endif
    u8 th_flags;
#define TH_FIN	0x01
#define TH_SYN	0x02
#define TH_RST	0x04
#define TH_PUSH	0x08
#define TH_ACK	0x10
#define TH_URG	0x20
    u16 th_win;		/* window */
    u16 th_sum;		/* checksum */
    u16 th_urp;		/* urgent pointer */
};

struct ipfakehdr {
	u32	       zero1;          /* unused */
       u32         zero2;          /* unused */
	u8     	ih_x1;		/* unused */
	u8	       ip_p;	       /* protocol */
	u16  	tcp_len;		/* Data len + tcphdr len */
	u32         src_ip;		/* source internet address */
	u32	       dst_ip;		/* destination internet address */
};


struct tcpiphdr {
    struct ipfakehdr ipfh;
    struct tcphdr     tcph;
};

struct ip {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ip_hl:4,
		ip_v:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	ip_v:4,
  		ip_hl:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
	u8 ip_tos;			/* type of service */
    u16 ip_len;			/* total length */
    u16 ip_id;			/* identification */
    u16 ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
    u8 ip_ttl;			/* time to live */
    u8 ip_p;			/* protocol */
    u16 ip_sum;			/* checksum */
    u32 ip_src, ip_dst;	/* source and dest address */
};

extern int tg_shm_reader_init(void);
extern TGNL_MSG_UDP_HDR *tg_shm_reader_next_packet(void);

#endif
