/**
 		mpac_sniffer.h
  @authors	Abdul Qadeer, Ghulam Mustafa
  @doc          Sample packet sniffer application.

*/

#ifndef _MPAC_SNIFFER_H
#define	_MPAC_SNIFFER_H

#include "mpac.h"
#include "mpac_sniffer.h"

#define GM_DBG 0
#define BUFFER_SIZE 17*1024
//#define RB_SIZE 100
#define ETHER_ADDR_LEN	6 /* Ethernet addresses are 6 bytes */
#define SIZE_ETHERNET 14  /* ethernet headers are always exactly 14 bytes */
#define ETHER_TYPE	2
#define ETHER_TYPE_IP 0X0800
#define IP_PROTOCOL_TYPE_TCP	0X06



#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)
#define IP_OFF(ip)		(((ip)->ip_off) & IP_OFFMASK)
#define RF(ip)			(((ip)->ip_off) & IP_RF)
#define DF(ip)			(((ip)->ip_off) & IP_DF)
#define MF(ip)			(((ip)->ip_off) & IP_MF)

#define TCP_FIN(tcp)		(((tcp)->th_flags) & TH_FIN)
#define TCP_SYN(tcp)		((((tcp)->th_flags) & TH_SYN)  >> 1)
#define TCP_RST(tcp)		((((tcp)->th_flags) & TH_RST)  >> 2)
#define TCP_PSH(tcp) 	    ((((tcp)->th_flags) & TH_PUSH) >> 3)
#define TCP_ACK(tcp) 	    ((((tcp)->th_flags) & TH_ACK)  >> 4)
#define TCP_URG(tcp)		((((tcp)->th_flags) & TH_URG)  >> 5)

/* Ethernet header */
typedef struct mpac_sniffer_ethernet
{
	uint8_t ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	uint8_t ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	uint16_t ether_type;                 /* IP? ARP? RARP? etc */
} mpac_ethheader;

/* IP header */
typedef	struct mpac_sniffer_ip
{
	uint8_t ip_vhl;                  /* version << 4 | header length >> 2 */
	uint8_t ip_tos;                  /* type of service */
	uint16_t ip_len;                 /* total length */
	uint16_t ip_id;                  /* identification */
	uint16_t ip_off;                 /* fragment offset field */
	#define IP_RF 0x8000            /* reserved fragment flag */
	#define IP_DF 0x4000            /* dont fragment flag */
	#define IP_MF 0x2000            /* more fragments flag */
	#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
	uint8_t ip_ttl;                  /* time to live */
	uint8_t ip_p;                    /* protocol */
	uint16_t ip_sum;                 /* checksum */
	struct in_addr ip_src,ip_dst;   /* source and dest address */
} mpac_ipheader;


/* TCP header */
typedef	struct mpac_sniffer_tcp
{
	uint16_t th_sport;	/* source port */
	uint16_t th_dport;	/* destination port */
	uint32_t th_seq;		/* sequence number */
	uint32_t th_ack;		/* acknowledgement number */

	uint8_t th_offx2;	/* data offset, rsvd */
	#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	uint8_t th_flags;
	#define TH_FIN 0x01
	#define TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define TH_CWR 0x80
	#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	uint16_t th_win;		/* window */
	uint16_t th_sum;		/* checksum */
	uint16_t th_urp;		/* urgent pointer */
} mpac_tcpheader;


typedef struct mpac_sniffer_context
{
	char errorMessage[50];

	char deviceName[10];
	int rawSock;
	unsigned char rawPacket[BUFFER_SIZE];
	int rawPacketLength;/*in bytes*/

	mpac_ethheader* ethHeader;
	mpac_ipheader* ipHeader;
	u_int ipHeaderSize;
	mpac_tcpheader* tcpHeader;
	u_int tcpHeaderSize;
	uint8_t* appPayload;

	char toFind[50];
	u_int toFindStringSize;

	/* 5 tuples for header comparison*/
	char t5_sourceIp[20];          /* In dotted quad notation*/
	char t5_destinationIp[20];     /* In dotted quad notation*/
	uint8_t t5_protocolType;     /* It will be matched against Protocol type in IP header*/
	uint16_t t5_sourcePort;      /* Transport layer source port*/
	uint16_t t5_destinationPort; /* Transport layer dest port*/

        long Q_len;
        int execution_mode;
        int numwrkr;
        int sniff_time;
        int affinity;
        int verbose;
        //volatile int empty;
} sniff_cntx;

//typedef struct mpac_rbuff_entry
//{
//    unsigned char rawPacket[BUFFER_SIZE];
//    int rawPacketLength;/*in bytes*/
//    int flag;
//} ring_buff;
//
//ring_buff rBuffer[RB_SIZE];

typedef struct mpac_sniffer_wrkr_context
{    
    long num_pckts_processed;
    long long int bytes;
    int wrkrID;
    int numWrkr;
    int Q_len;
    volatile int geti;
    volatile int puti;
} wrkr_cntx;



void mpac_sniffer_usage                    (char *prog_name);
int  mpac_sniff_cntx_init                  (sniff_cntx* cntx);
int  mpac_sniff_rawpacket                  (sniff_cntx* cntx);//1
int  mpac_sniff_ippacket                   (sniff_cntx* cntx);//2
int  mpac_sniff_extract_ipheader           (sniff_cntx* cntx);//3
int  mpac_sniff_extract_tcpheader          (sniff_cntx* cntx);//4
int  mpac_sniff_tcppacket_5tuple_comparison(sniff_cntx* cntx);//2-4 comparison (cpu warming)
int  mpac_sniff_extract_payload            (sniff_cntx* cntx);//5
int  mpac_sniff_find_stringinpayload       (sniff_cntx* cntx);//6

//printing routine
int mpac_print_packet_hx (sniff_cntx* cntx);
int mpac_print_ethheader (sniff_cntx* cntx);
int mpac_print_ipheader  (sniff_cntx* cntx);
int mpac_print_tcpheader (sniff_cntx* cntx);
int mpac_sniff_closedown (sniff_cntx* cntx);
int mpac_sniff_do_some_work(sniff_cntx* cntx);

wrkr_cntx** mpac_sniffer_configure (sniff_cntx* );
int  mpac_sniffer_arg_handler    (int argc, char** argv, sniff_cntx* cntx);
void mpac_sniffer_print_config   (sniff_cntx *cntx);
void mpac_sniffer_default_config (sniff_cntx *cntx);
int  mpac_sniffer_parse_cmdl     (int argc, char **argv, sniff_cntx *cntx);
void mpac_sniffer_output(int, long, int, double, double, double, double, long long int);
void* mpac_sniffer_worker (void* arg);
void* mpac_sniffer_time_keeper(int* duration);
void mpac_sniffer_cleanup();
#endif	/* _MPAC_SNIFFER_H */