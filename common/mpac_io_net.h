/* 
 * File:   mpac_io_net.h
 * Author: Ghulam Mustafa, Umair Naushad
 *
 * Created on July 15, 2008, 11:47 AM
 */

#ifndef _MPAC_IO_NET_H
#define	_MPAC_IO_NET_H


#include "mpac.h"

#define MPAC_IO_NET_BUF_SZ 5120

#define MPAC_IO_NET_TYPE_SEND 1
#define MPAC_IO_NET_TYPE_RECV 2

#define MPAC_TCP 0
#define MPAC_UDP 1
#define MPAC_RAW 2

#define MPAC_SOCK_QLEN 5

struct mpac_io_net_handler
{
  int side;                         //Send or Recieve
  int fd;                           //File Descriptor
  int msg_sz;
  unsigned int sockopt;             // Flag (whether use sockopt or not)
  unsigned int bytes_read;           // for receiver side only
  int accept_fd;                    // for receiver side only
  struct sockaddr_in * recv_host;
  int buff_size;
  char* buff;
  struct sockaddr   accept_sock;    // for receiver side only
  socklen_t         accept_sock_sz; // for receiver side only (unsigned int)
};


int mpac_io_net_init     (struct mpac_io_net_handler* h, 
                          const char *hostname,
                          int port, //service name or port No.                          
                          int cside,            //communication side (sending/recieveing)
                          int msgsize,
                          unsigned long buff_sz);
char* mpac_io_net_getipaddr(char*);
int mpac_io_net_getportno(char* , char* );
int mpac_io_net_getprotono(char* );
char* mpac_io_net_getdn(char* );

#endif	/* _MPAC_IO_NET_H */

