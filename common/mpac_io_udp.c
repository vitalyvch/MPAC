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
 * File:   mpac_io_udp.c
 * Author: Ghulam Mustafa, Umair Naushad, Faiza Iftikhar
 */

#include "mpac.h"
#include "mpac_io_udp.h"

/**
 *  Sender side API to open a stream socket to receiver end point.
 */
int mpac_io_udp_open_sender (struct mpac_io_net_handler * h)
{
    if (h == NULL)
    {
        fprintf(stderr, "mpac_io_udp_open_sender: null handle passed\n");
        return MPAC_FAILURE;
    }
    
    if ( (h->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
    {
        herror("Error Number = ");
        perror("mpac_io_udp_open_sender: failed to open socket: ");
        return MPAC_FAILURE;
    }    
    
    // adjust socket send and receive buffers
    if (h->sockopt)
    {
        if ((setsockopt(h->fd, SOL_SOCKET, SO_SNDBUF,
                    &(h->buff_size), sizeof(h->buff_size)))<0)
         {
        perror("mpac_io_udp_open_sender: setsockopt faild for sendbuf: ");
        return MPAC_FAILURE;
         }
        if ((setsockopt(h->fd, SOL_SOCKET, SO_RCVBUF,
                    &(h->buff_size), sizeof(h->buff_size)))<0)
        {
        perror("mpac_io_udp_open_sender: setsockopt faild for recvbuf: ");
        return MPAC_FAILURE;
        }
        if(connect(h->fd, (struct sockaddr *) h->recv_host,
                                    sizeof(*h->recv_host))<0)
    {
      perror("mpac_io_udp_open_sender: connect failed: ");
      return MPAC_FAILURE;
    }
    }
    return MPAC_SUCCESS;
}


int mpac_io_udp_send (struct mpac_io_net_handler * h)
{
  int len;
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_udp_send: null handle passed\n");
      return MPAC_FAILURE;
    } 
  len=sendto(h->fd, (char*)h->buff, h->msg_sz, 0,
             (struct sockaddr *) (h->recv_host),sizeof(*h->recv_host));
  if (len < 0)
    {
      perror("mpac_io_udp_send: write failed: ");
      return MPAC_FAILURE;
    } 
   h->bytes_read = len;
  return MPAC_SUCCESS;
}


int mpac_io_udp_open_receiver (struct mpac_io_net_handler * h)
{
  struct sockaddr_in recvr;
  int flag = 1;
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_udp_open_receiver: null handle passed\n");
      return MPAC_FAILURE;
    }

  /* open udp socket */
  if ((h->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0)
    {
      perror("mpac_io_udp_open_receiver: failed to open socket: ");
      return MPAC_FAILURE;
    } 
   
    recvr.sin_family = AF_INET;
    recvr.sin_port   = (unsigned short)h->recv_host->sin_port; 
    recvr.sin_addr.s_addr = INADDR_ANY; //IP to binary
    setsockopt(h->fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    memset(&(recvr.sin_zero), '\0', 8);
    if ((bind(h->fd, (struct sockaddr *)&recvr, sizeof(recvr)))<0)  
      {
	perror("mpac_io_udp_open_receive: bind failed: ");
	close(h->fd);
	return MPAC_FAILURE;
      }

    // set any socket propoerties here
    if (h->sockopt)
    {
     setsockopt(h->fd, SOL_SOCKET, SO_SNDBUF, &h->buff_size, sizeof(h->buff_size));
     setsockopt(h->fd, SOL_SOCKET, SO_RCVBUF, &h->buff_size, sizeof(h->buff_size));
    }   
    return MPAC_SUCCESS;
}

int mpac_io_udp_receive (struct mpac_io_net_handler * h)
{
  int actual_size;

  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_udp_receive: null handle passed\n");
      return MPAC_FAILURE;
    }
  
  actual_size= recvfrom(h->fd, (char*) h->buff, h->msg_sz, 0,
			(struct sockaddr *) h->recv_host, 
			&h->accept_sock_sz);
  if(actual_size<0)
    {
      perror("mpac_io_udp_receive: read failed: ");
      return MPAC_FAILURE;
    }
  h->bytes_read = actual_size;
  
  return MPAC_SUCCESS;
}


int mpac_io_udp_close (struct mpac_io_net_handler * h)
{

  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_udp_close: null handle passed\n");
      return MPAC_FAILURE;
    }
  if(h->side == MPAC_IO_NET_TYPE_SEND)
      shutdown(h->fd, SHUT_WR);
  else
  {
      if((close(h->fd))<0)
    {
      perror("mpac_io_udp_close: close failed for send socket: ");
      return MPAC_FAILURE;
    }
  }
  
  return MPAC_SUCCESS;
}
