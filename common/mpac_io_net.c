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
 * File:   mpac_io_net.c
 * Author: Ghulam Mustafa
 */

#include "mpac.h"
#include "mpac_io_net.h"
#include <netdb.h>


int mpac_io_net_init     (struct mpac_io_net_handler* h, 
                          const char *hostname,
                          int port,                          
                          int cside,
                          int msgsize,
                          unsigned long buff_sz)
{
  struct hostent * phe;		
  
  h->recv_host = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  memset(h->recv_host, 0, sizeof(struct sockaddr_in));
  h->recv_host->sin_family = AF_INET;
  
  /* Service name to port number */
  h->recv_host->sin_port = htons((u_short)port);
  
  /* Host name to IP address (dotted decimal) */
  if ( phe = gethostbyname(hostname) )
    memcpy(&h->recv_host->sin_addr, phe->h_addr, phe->h_length);
  else if ( (h->recv_host->sin_addr.s_addr = inet_addr(hostname))
	    == INADDR_NONE )
    printf("can't get %s host entry\n", hostname);
  
  /* Use protocol to choose a socket type */
/*
  if (strcmp(protocol, "udp") == 0) h->sockopt = SOCK_DGRAM;
  else if (strcmp(protocol, "tcp") == 0) h->sockopt = SOCK_STREAM;
  else if (strcmp(protocol, "raw") == 0) h->sockopt = SOCK_RAW;
  else printf ("Protocol not supported\n(Supported protocols are udp,tcp,raw)\n");        
*/
  h->sockopt = 0;
  h->accept_fd = -1;
  h->side = cside;
  h->msg_sz = msgsize;
  h->accept_sock_sz = sizeof(h->accept_sock);
  if(buff_sz < 1)
    {
      printf ("Invalid Buffer Size\n");
      return MPAC_FAILURE;
    }
  else h->buff_size = buff_sz;
  if(h->side == MPAC_IO_NET_TYPE_SEND)        
    h->buff = NULL;
  else
    h->buff = (char*) malloc (h->msg_sz);
  
  h->bytes_read = -1;
  h->fd = -1;
  
  return MPAC_SUCCESS;
}

/** mpac_io_net_getipaddr() returns a char pointer to the IP address
 * corresponding to the given host name */

char* mpac_io_net_getipaddr(char* domainName)
{
  struct hostent *ptrHostent;
  if( (ptrHostent=gethostbyname(domainName) ) != NULL)
    return (char*)inet_ntoa(*(struct in_addr *)ptrHostent->h_addr);
  else
    {
      herror("Error Number = ");        
      perror("\nmpac_io_net_getipaddr:  failed for gethostbyname\n");
      return "Invalid host name\n";
    }
}

/* mpac_io_net_getportno() returns port number according to given specification 
 * and -1 in case of any error */
int mpac_io_net_getportno(char* service, char* proto)
{
  struct servent *ptrServent;
  if(ptrServent=getservbyname(service,proto))
    {   
      return ntohs(ptrServent->s_port);
    }
  else
    {
      herror("Error Number = ");
      perror("mpac_io_net_getportno: failed for getserverbyname\n");
      return MPAC_FAILURE;
    }
}

/** This function will return proto number according to given specification
 * and -1 in case of any error. */
int mpac_io_net_getprotono(char* protoName)
{
  struct protoent *ptrProtoent;
  if(ptrProtoent=getprotobyname(protoName))   
    return ptrProtoent->p_proto;
  else
    {
      herror("Error Number = ");
      perror("mpac_io_net_getprotono: failed for getprotono\n");
      return MPAC_FAILURE;
    }
}

char* mpac_io_net_getdn(char* ipAddress)
{
  printf("%s",ipAddress);
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(3490);
  inet_aton(ipAddress,&(addr.sin_addr) );
 
  struct hostent *hp;
  
  hp= gethostbyaddr((void *) &(addr), sizeof(struct in_addr), AF_INET);
  
  if(hp!=NULL)
    {
      char *temp="localhost";
      printf("domain name = %s",temp);
    }
  else
    printf("error");
}
