/**************************************************************************
 *
 * Copyright 2008 HPCNL-KICS, UET, Lahore, PAKISTAN.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification,are permitted provided that the following conditions 
 * are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY HPCNL-KICS ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL HPCNL-KICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of HPCNL-KICS.
 *
 ***************************************************************************** 
 *  
 * Sniff.c
 *
 *  Created on: Sep 28, 2009
 *      Author: Ghulam Mustafa, Abdul Qadeer
 *
 *  Reference and Acknowledgement:
 *
 *      Some code snippets are adapted from the following sites
 *
 *      http://www.security-freak.net/raw-sockets/sniffer.c
 *      http://www.tcpdump.org/pcap.htm
 */

#include "mpac.h"
#include "mpac_sniffer.h"



void mpac_sniffer_print_config(sniff_cntx* cntx)
{
  fprintf(stdout,"mpac_sniffer_app configuration\n");
  fprintf(stdout,"======================================================= \n");
  
  fprintf(stdout," Execution Mode                   : %d \n",cntx->execution_mode);
  fprintf(stdout," Sniffing duration                : %d \n",cntx->sniff_time);
  fprintf(stdout," Number of worker threads         : %d \n", cntx->numwrkr);
  fprintf(stdout," Packet queue length              : %lu \n", cntx->Q_len);
  fprintf(stdout," Protocol                         : TCP \n");
  fprintf(stdout," Sender port No.                  : %d \n", cntx->t5_sourcePort);
  fprintf(stdout," Receiver port No.                : %d \n", cntx->t5_destinationPort);
  fprintf(stdout," Sender's IP address              : %s \n", cntx->t5_sourceIp);
  fprintf(stdout," Receiveer's IP address           : %s \n", cntx->t5_destinationIp);
  fprintf(stdout," Interface to sniff               : %s \n", cntx->deviceName);
  if (cntx->affinity)
  fprintf(stdout," Core Affinity Support            : Enabled \n");
  else
  fprintf(stdout," Core Affinity Support            : Disabled \n");  
  fprintf(stdout,"======================================================= \n");
/*
  fprintf(stdout,"==========| Press Ctrl-C to stop sniffing |============ \n");
  fprintf(stdout,"======================================================= \n");
*/

}

void mpac_sniffer_default_config(sniff_cntx* cntx)
{

  cntx->affinity            = 0;
  cntx->execution_mode      = 3;
  cntx->Q_len               = 1000;
  cntx->numwrkr             = 1;  
  cntx->sniff_time          = 10;
  cntx->t5_sourcePort       = 54321;
  cntx->t5_destinationPort  = 54321;
  cntx->t5_protocolType     = 0x06; //????
  cntx->verbose             = 0;
  cntx->appPayload          = NULL;
  cntx->ethHeader           = NULL;
  cntx->ipHeader            = NULL;
  cntx->ipHeaderSize        = 0;
  cntx->tcpHeader           = NULL;
//  cntx->empty                = 1;
  
  strcpy(cntx->toFind, "google");
  strcpy(cntx->deviceName, "lo");
  strcpy(cntx->t5_sourceIp,"127.0.0.1");
  strcpy(cntx->t5_destinationIp,"127.0.0.1");
/*
  cntx->t5_sourceIp = NULL;
  cntx->t5_destinationIp = NULL;
*/
  strcpy (cntx->errorMessage, "No Error" );
  }

void mpac_sniffer_usage(char *prog_name)
{
  fprintf(stdout,"========================================================\n");
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  //e:q:d:n:p:P:i:I:a:f:vh
  printf("-e          Execution mode [3-5] \n");
  printf("            (Default: 3)\n");
  printf("-q          Packet queue length\n");
  printf("            (Default: 1000)\n");
  printf("-d          Sniffing duration (in seconds)\n");
  printf("            (Default: 10 seconds)\n");
  printf("-n          Number of worker threads \n");
  printf("            (Default: 1)\n");
  printf("-p          Sender Port Number \n");
  printf("            (Default: 54321)\n");
  printf("-P          Receiver Port Number \n");
  printf("            (Default: 54321)\n");
  printf("-i          Sender IP Address \n");
  printf("            (Default: 127.0.0.1)\n");
  printf("-I          Receiver IP Address \n");
  printf("            (Default: 127.0.0.1)\n");
  printf("-a          Core Affinity: [1 | 0] i.e. Enabled or Disabled, respectively \n");
  printf("            (Default: 0)\n");
  printf("-f          Interface to sniff [eth0 | eth1 | ...] \n");
  printf("            (Default: eth0)\n");
  printf("-v          Verbose output \n");
  printf("-h          For help \n");
  fprintf(stdout,"======================================================== \n");
/*
  fprintf(stdout,"==========| Press Ctrl-C to stop sniffing |============= \n");
  fprintf(stdout,"======================================================== \n");
*/
}

int mpac_sniffer_parse_cmdl(int argc, char **argv, sniff_cntx* cntx)
{
  int c;
  mpac_sniffer_default_config(cntx);
while ((c = getopt(argc, argv, "e:q:d:n:p:P:i:I:a:f:vh")) != -1)
{
 switch (c)
 {
  case 'e':
      if (atoi(optarg) < 3 || atoi(optarg) > 5) {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): Illegal execution mode\n");
          exit (MPAC_FAILURE);
      }
      else   cntx->execution_mode = atoi(optarg);      
      break;

  case 'q':
      if (atol(optarg) < 0 || atol(optarg) > pow(2,8*sizeof(long))) {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): Illegal packet queue length\n");
          exit (MPAC_FAILURE);
      }
      else   cntx->Q_len = atol(optarg);
      break;

 case 'd':
      if (atoi(optarg) < 1) {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): Illegal sniffing duration\n");
          exit (MPAC_FAILURE);
      }
      else   cntx->sniff_time = atoi(optarg);
      break;

  case 'n':
      if(atoi(optarg) < 1)
      {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): No. of worker threads should be at least 1 \n");
          exit(MPAC_FAILURE);
      }
      else cntx->numwrkr = atoi(optarg);
      break;

  case 'p':
      if (atoi(optarg) < 0)
      {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): Sender port should be positive integer\n");
          exit (MPAC_FAILURE);
      }
      else
          cntx->t5_sourcePort = atoi(optarg);
      break;

  case 'P':
      if (atoi(optarg) < 0)
      {
          fprintf(stderr, "mpac_sniffer_parse_cmdl(): Receiver port should be positive integer\n");
          exit (MPAC_FAILURE);
      }
      else
          cntx->t5_destinationPort = atoi(optarg);
      break;

  case 'i':
      strcpy(cntx->t5_sourceIp, optarg);
      break;

 case 'I':
     strcpy(cntx->t5_destinationIp, optarg);
     break;
     
 case 'a':
     if( atoi(optarg) != 0 && atoi(optarg) != 1 )
     {
         fprintf(stderr, "mpac_sniffer_parse_cmdl: Illegal affinity option\n");
         exit(MPAC_FAILURE);
     }
     cntx->affinity = atoi(optarg);
     break;

 case 'f':
      strcpy(cntx->deviceName, optarg);
      break;

 case 'v':
     cntx->verbose = 1;
     break;

 case 'h':
     mpac_sniffer_usage(argv[0]);
     exit (MPAC_SUCCESS);
     
 case '?':
     mpac_sniffer_usage(argv[0]);
     return MPAC_FAILURE;

  default:
      mpac_sniffer_usage(argv[0]);
      return MPAC_FAILURE;
 }
}
  return MPAC_SUCCESS;
}

wrkr_cntx** mpac_sniffer_configure(sniff_cntx* scntx)
{
  int i;
  wrkr_cntx** cntx;
  

  cntx = (wrkr_cntx**) malloc(scntx->numwrkr * sizeof(wrkr_cntx*));
  if (cntx == NULL) {
      fprintf(stderr,"mpac_sniffer_configure(): Not enough memory \n " );
      return MPAC_NULL;
  }

  for (i = 0; i < scntx->numwrkr; i++) {
      cntx[i]=(wrkr_cntx*) malloc(sizeof (wrkr_cntx));
      if (cntx[i] == NULL) {
	  fprintf(stderr,"mpac_sniffer_congifure[i]: Not enough memory \n " );
	  return MPAC_NULL;
	}
      cntx[i]->num_pckts_processed  = 0;
      cntx[i]->bytes                = 0;
      cntx[i]->wrkrID               = i;
      cntx[i]->numWrkr              = scntx->numwrkr;
      cntx[i]->Q_len                = 0;
 //     cntx[i]->geti                 = NULL;
//      cntx[i]->puti                 = NULL;
  }

  
  return cntx;
}

int mpac_sniffer_arg_handler(int argc, char** argv, sniff_cntx* cntx)
{
    int ret;
    if(argc < 2){        
      mpac_sniffer_default_config(cntx);                                        
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{
    if((ret = mpac_sniffer_parse_cmdl(argc, argv, cntx))!= MPAC_SUCCESS)
      {
	  fprintf(stderr, "mpac_sniffer_arg_handler(): failed to parse commandline arguments \n");
	  mpac_sniffer_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }

   mpac_sniffer_print_config((sniff_cntx *)cntx);
    return MPAC_SUCCESS;
}

int mpac_sniff_cntx_init(sniff_cntx* cntx)
{
    if(cntx->deviceName == NULL)
    {
        printf("context->deviceName is null!\n");//cntx->errorMessage = "context->deviceName is null!";
        return -1;
    }
    /*create raw socket  */
    if((cntx->rawSock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))== -1)
    {
        printf("Can not create raw socket!\n");//cntx->errorMessage = "Can not create raw socket!";
        return -1;
    }


	/*Bind raw socket to an interface  */

	struct sockaddr_ll sll;
	struct ifreq ifr;

	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));

	/*Get the Interface Index  */
	strncpy((char *)ifr.ifr_name, cntx->deviceName, IFNAMSIZ);
	if((ioctl(cntx->rawSock, SIOCGIFINDEX, &ifr)) == -1){
		printf("Error getting Interface index!\n");//cntx->errorMessage = "Error getting Interface index!";
		return -1;
	}

	/* Bind our raw socket to this interface */

	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);

	if((bind(cntx->rawSock, (struct sockaddr *)&sll, sizeof(sll)))== -1){
		printf("Error binding raw socket to interface!\n");//cntx->errorMessage = "Error binding raw socket to interface!";
		return -1;
	}        
	return 0;
}

int mpac_sniff_rawpacket(sniff_cntx* context)
{    
	struct sockaddr_ll aPacket;
	socklen_t aPacketSize = sizeof(aPacket);

	if((context->rawPacketLength = recvfrom(context->rawSock,
					   context->rawPacket,
					   BUFFER_SIZE,
					   0,
					   (struct sockaddr*)&aPacket,
					   &aPacketSize)) == -1){

		perror("[recvfrom] returned -1\n");//context->errorMessage = "[recvfrom] returned -1!";
		return -1;
	}
	context->ethHeader = (mpac_ethheader*)(context->rawPacket);
	context->ethHeader->ether_type = ntohs(context->ethHeader->ether_type);
	return 0;
}

int mpac_sniff_ippacket(sniff_cntx* context)
{
    /*get a packet at data link layer level*/
    /*context->ethHeader = (EthernetHeader*)(context->rawPacket);*/
    do{
        if(mpac_sniff_rawpacket(context) != 0)
        {
            return -1;
        }
    }
    while(context->ethHeader->ether_type != ETHER_TYPE_IP);
    return 0;
}

int mpac_sniff_extract_ipheader(sniff_cntx* context){

	//if(mpac_sniff_ippacket(context) != 0){
	//	return -1;
	//}

	context->ipHeader = (mpac_ipheader*)(context->rawPacket + SIZE_ETHERNET);

	context->ipHeaderSize = IP_HL(context->ipHeader)*4;

	if (context->ipHeaderSize < 20) {
		printf("Invalid IP header length!\n");//context->errorMessage = "Invalid IP header length!";
		return -1;
	}

	context->ipHeader->ip_len = ntohs(context->ipHeader->ip_len);
	context->ipHeader->ip_id = ntohs(context->ipHeader->ip_id);
	context->ipHeader->ip_sum = ntohs(context->ipHeader->ip_sum);

	return 0;
}

int mpac_sniff_extract_tcpheader(sniff_cntx* context){

	do{
		if(mpac_sniff_extract_ipheader(context) != 0){
			return -1;
		}
	}
	while(context->ipHeader->ip_p != IP_PROTOCOL_TYPE_TCP);

	//printf("Tcp packet found \n");



	context->tcpHeader = (mpac_tcpheader*)(context->rawPacket + SIZE_ETHERNET + context->ipHeaderSize);
	context->tcpHeaderSize = TH_OFF(context->tcpHeader)*4;
	if (context->tcpHeaderSize < 20){
		printf("Invalid TCP header length!\n");//context->errorMessage = "Invalid TCP header length!";
		return -1;
	}
        uint16_t sport = ntohs(context->tcpHeader->th_sport);;//context->tcpHeader->th_sport;
	context->tcpHeader->th_sport = sport;
	context->tcpHeader->th_dport = ntohs(context->tcpHeader->th_dport);
	context->tcpHeader->th_seq = ntohl(context->tcpHeader->th_seq);
	context->tcpHeader->th_ack = ntohl(context->tcpHeader->th_ack);
	context->tcpHeader->th_win = ntohs(context->tcpHeader->th_win);
	context->tcpHeader->th_sum = ntohs(context->tcpHeader->th_sum);
	context->tcpHeader->th_urp = ntohs(context->tcpHeader->th_urp);


	return 0;
}

int mpac_sniff_tcppacket_5tuple_comparison(sniff_cntx* context){

	if(mpac_sniff_extract_tcpheader(context) != 0){
			return -1;
	}

	int similar = 1;

	if(strcmp(context->t5_sourceIp,inet_ntoa(context->ipHeader->ip_src)) != 0){
		similar = 0;
        if (GM_DBG) printf("1\n");
	}

	if(strcmp(context->t5_destinationIp,inet_ntoa(context->ipHeader->ip_dst)) != 0){
		similar = 0;
        if (GM_DBG)                 printf("2\n");
	}

	if(context->ipHeader->ip_p != context->t5_protocolType){
		similar = 0;
                if (GM_DBG)         printf("3\n");
	}

	if(context->tcpHeader->th_sport != context->t5_sourcePort){
		similar = 0;
                        if (GM_DBG) printf("4\n");
                        if (GM_DBG) printf("[%d,%d]\n",context->tcpHeader->th_sport, context->t5_sourcePort);
	}

	if(context->tcpHeader->th_dport != context->t5_destinationPort){
		similar = 0;
                        if (GM_DBG) printf("5\n");
                        if (GM_DBG) printf("[%d,%d]\n",context->tcpHeader->th_dport,context->t5_destinationPort);
	}


	if(similar == 0) return -1;
        else return 0;

}

int mpac_sniff_extract_payload(sniff_cntx* context){

	if(mpac_sniff_extract_tcpheader(context) != 0){
		return -1;
	}

	context->appPayload = (uint8_t *)(context->rawPacket + SIZE_ETHERNET +
		                           context->ipHeaderSize + (uint8_t)context->tcpHeaderSize);

	return 0;


}

int mpac_sniff_find_stringinpayload(sniff_cntx* context){
	if(mpac_sniff_extract_payload(context) != 0){
		return -1;
	}

	int i = 0;
	int payloadLength = context ->rawPacketLength -
						SIZE_ETHERNET -
						context->ipHeaderSize -
						context->tcpHeaderSize;

	int range = payloadLength - context->toFindStringSize + 1;

	for(i = 0; i < range; i++){
		int result = memcmp(context->appPayload + i, context->toFind, context->toFindStringSize);
		if(result == 0){
			printf("the result is %d \n", result);
			return 0;
		}
	}
	return -1;
}

int mpac_sniff_do_some_work(sniff_cntx* cntx){

/*
    if(mpac_sniff_extract_payload(cntx) != 0){
		return -1;
	}

	int i = 0;
	int payloadLength = cntx ->rawPacketLength -SIZE_ETHERNET -
				cntx->ipHeaderSize - cntx->tcpHeaderSize;
*/
        double temp = 1.0;
        //printf("Length=%d\n", payloadLength);

        int k = 0;
        for(k = 0; k < 100000; k++){
            temp = temp + 1;
            temp = exp(temp);
        
        }
        printf("temp= %0.2f\n", temp);
	return 0;
}

int mpac_print_packet_hx(sniff_cntx* context){
	unsigned char *p = context->rawPacket;
	int len = context->rawPacketLength;

	printf("\n\n---------Packet---Starts----\n\n");

	while(len--){
		printf("%.2x ", *p);
		p++;
	}

	printf("\n\n--------Packet---Ends-----\n\n");

	return 0;

}

int mpac_print_ethheader(sniff_cntx* context){

	printf("\n\n---------Ethernet Header Starts----\n\n");

	printf("Destination MAC address            :");
	int len = ETHER_ADDR_LEN;
        if (GM_DBG) printf("_(ethheader)_\n");
	unsigned char* p = context->ethHeader->ether_dhost;

	while(len--){
		printf("%.2x ", *p);
		p++;
	}
	printf("\nSource MAC address                 :");
	len = ETHER_ADDR_LEN;

	p = context->ethHeader->ether_shost;
	while(len--){
		printf("%.2x ", *p);
		p++;
	}
	printf("\nEtherenet Type (In host mem layout):");
	len = ETHER_TYPE;

	printf("\n\n---------Ethernet Header Ends----\n\n");
        return 0;

}

int mpac_print_ipheader(sniff_cntx* context){
	printf("\n\n---------IP Header Starts----\n\n");

	printf("Version                       : %d \n", IP_V(context->ipHeader));
	printf("Header length                 : %d \n", context->ipHeaderSize);
	printf("Type of service               : %d \n", context->ipHeader->ip_tos);
	printf("Datagram length               : %d \n", context->ipHeader->ip_len);
	printf("Identication no.              : %d \n", context->ipHeader->ip_id);
	printf("Offset                        : %d \n", IP_OFF(context->ipHeader));
	printf("Reserved fragment bit         : %d \n", RF(context->ipHeader));
	printf("Dont fragment bit             : %d \n", DF(context->ipHeader));
	printf("More fragment bit             : %d \n", MF(context->ipHeader));
	printf("Time to live                  : %d \n", context->ipHeader->ip_ttl);
	printf("Protocol                      : %d \n", context->ipHeader->ip_p);
	printf("Checksum                      : %d \n", context->ipHeader->ip_sum);
	printf("Source IP                     : %s\n",inet_ntoa(context->ipHeader->ip_src));
	printf("Destination IP                : %s\n",inet_ntoa(context->ipHeader->ip_dst));



	printf("\n\n---------IP Header Ends----\n\n");

	return 0;
}

int mpac_print_tcpheader(sniff_cntx* context){
	printf("\n\n---------TCP Header Starts----\n\n");

	printf("Source port                   : %d \n", context->tcpHeader->th_sport);
	printf("Destination port              : %d \n", context->tcpHeader->th_dport);
	printf("Sequence No                   : %u \n", context->tcpHeader->th_seq);
	printf("Ack No                        : %u \n", context->tcpHeader->th_ack);
	printf("Flags                         : %d \n", context->tcpHeader->th_flags);
	printf("URG flag                      : %d \n", TCP_URG(context->tcpHeader));
	printf("ACK flag                      : %d \n", TCP_ACK(context->tcpHeader));
	printf("PSH flag                      : %d \n", TCP_PSH(context->tcpHeader));
	printf("RST flag                      : %d \n", TCP_RST(context->tcpHeader));
	printf("SYN flag                      : %d \n", TCP_SYN(context->tcpHeader));
	printf("FIN flag                      : %d \n", TCP_FIN(context->tcpHeader));
	printf("Window Size                   : %d \n", context->tcpHeader->th_win);
	printf("Checksum                      : %d \n", context->tcpHeader->th_sum);
	printf("Urgent Pointer                : %d \n", context->tcpHeader->th_urp);


	printf("\n\n---------TCP Header Ends----\n\n");

	return 0;
}

int mpac_sniff_closedown(sniff_cntx* context){

	if (close(context->rawSock) == -1){
		printf("Failed to close the socket!\n");//context->errorMessage = "Failed to close the socket!";
		return -1;
	}

	return 0;
}

void mpac_sniffer_output (int nwrkrs, long Q, int lab, double time, double pp, double tp, double drp, long long int totalpckts)
{
/*
    double temp1, temp2;
    temp1 = (double)bp/time;
    temp2 = (double)temp1 * 8/1000000.0;
*/
    printf("mpac_sniffer_app finished successfully. \n\n");
    printf("=================\n");
    printf("     Results     \n");
    printf("=================\n");
    printf("#Threads, Elapsed time (sec), #packet captured, #packet processed, #packet dropped, Throughput (packets/sec), THroughput (mbps), Packet queue size, Lab#\n");
    printf("%d, %.2lf, %llu, %.2lf, %.2lf, %0.2lf, %0.2lf, %lu, %d\n", nwrkrs, time, totalpckts, pp, drp, pp/time, tp, Q, lab);
}

