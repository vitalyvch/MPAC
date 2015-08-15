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
 
  Name:           mpac_io_tcp.c
  Author:         Abdul Waheed, Ghulam Mustafa
  Date:           April 13, 2008 
  Desription:     TCP stream socket based network I/O services

*/

#include "mpac.h"
#include "mpac_io_tcp.h"

/**
 *  Sender (Client) side API to open a stream socket to receiver end point.
 */
int mpac_io_tcp_open_sender (struct mpac_io_net_handler * h)
{
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_open_sender: null handle passed\n");
      return MPAC_FAILURE;
    }

  /* open stream socket */
  if ((h->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
    {
      perror("mpac_io_tcp_open_sender: failed to open socket: ");
      return MPAC_FAILURE;
    }
  
  /* adjust socket send and receive buffers */
  if (h->sockopt)
  {
      if ((setsockopt(h->fd, SOL_SOCKET, SO_SNDBUF, &h->msg_sz,
                                                sizeof(h->msg_sz)))<0)
      {
      perror("mpac_io_tcp_open_sender: setsockopt faild for sendbuf: ");
      return MPAC_FAILURE;
      }
      if ((setsockopt(h->fd, SOL_SOCKET, SO_RCVBUF, &h->buff_size,
                                             sizeof(h->buff_size)))<0)
        {
          perror("mpac_io_tcp_open_sender: setsockopt faild for recvbuf: ");
          return MPAC_FAILURE;
        }
  }
  
  /* connect to the receiver end point */
  if(connect(h->fd, (struct sockaddr *) h->recv_host, 
                                    sizeof(*h->recv_host))<0)
    {
      perror("mpac_io_tcp_open_sender: connect failed: ");
      return MPAC_FAILURE;
    }
  
  return MPAC_SUCCESS;
}

/*TCP Send*/

int mpac_io_tcp_send (struct mpac_io_net_handler * h)
{
  int len;
  int write_len = h->msg_sz;
  
 if ((len = send(h->fd, (char *) &h->buff, write_len,0)) != write_len) 
 {
     perror("mpac_io_tcp_send(): send call failure");
     printf("mpac_io_tcp_send(): Bytes Sent: %d\n", len);
     exit(1);
  }
  return MPAC_SUCCESS;
}

/* Server Side Open */
int mpac_io_tcp_open_receiver (struct mpac_io_net_handler * h)
{    
  struct sockaddr_in recvr;

  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_open_receiver: null handle passed\n");
      return MPAC_FAILURE;
    }
  
  /* open tcp stream socket */
  if ((h->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
    {
      perror("mpac_io_tcp_open_receiver: failed to open socket: ");
      return MPAC_FAILURE;
    }

  /* bind a name to the socket */
  recvr.sin_family = AF_INET;
  recvr.sin_port   = h->recv_host->sin_port;
  recvr.sin_addr.s_addr = INADDR_ANY; 

    /* set any socket propoerties here */
  if(h->sockopt)
    {
      setsockopt(h->fd, SOL_SOCKET, SO_SNDBUF, &h->msg_sz, sizeof(h->msg_sz));  
      setsockopt(h->fd, SOL_SOCKET, SO_RCVBUF, &h->buff_size, sizeof(h->buff_size));
    }
  if ((bind(h->fd, (struct sockaddr *)&recvr, sizeof(recvr)))<0)
    {
      perror("mpac_io_tcp_open_receiver: bind failed: ");
      close(h->fd);
      return MPAC_FAILURE;
    }

  /* listen on the receive socket */
  if ((listen(h->fd, MPAC_SOCK_QLEN))<0)
    {
      perror("mpac_io_tcp_open_receiver: listen failed: ");
      close(h->fd);
      return MPAC_FAILURE;
    }
  return MPAC_SUCCESS;
}

int mpac_io_tcp_write (struct mpac_io_net_handler * h)
{
  int i, len;
  int write_len = h->msg_sz;
  
  for (i=0, len=0; i < h->msg_sz; )
    {
      if ((len = write(h->fd, (char *) &h->buff[i], write_len)) < 0)
	{
	  perror("mpac_io_tcp_write: write failed: ");
	  return MPAC_FAILURE;
	}
      i += len;
      write_len -= len;
    }

  return MPAC_SUCCESS;
}

/* Server Side Open */

int mpac_io_tcp_read (struct mpac_io_net_handler * h)
{
     int i, len, write_len;
  write_len = h->msg_sz;
  
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_read: null handle passed\n");
      return MPAC_FAILURE;
    }

  h->bytes_read = 0;
  for (i=0, len=0; i < h->msg_sz; )
    {
      if ((len =read(h->accept_fd, 
		     (char *) &h->buff[i], write_len)) < 0)
	{
	  perror("mpac_io_tcp_read: read failed: ");
	  return MPAC_FAILURE;
	}
      i += len;
      write_len -= len;
    }
  return MPAC_SUCCESS;
}

int mpac_io_tcp_accept_receiver (struct mpac_io_net_handler * h)
{
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_accept: null handle passed\n");
      return MPAC_FAILURE;
    }

  /* block to accept the incoming connection request */
  if((h->accept_fd = accept(h->fd, &h->accept_sock, &h->accept_sock_sz))<0)
    {
      perror("mpac_io_tcp_accept: accept failed: ");
      return MPAC_FAILURE;
    }

  /* return to process the accepted connection */
  return MPAC_SUCCESS;
} 

int mpac_io_tcp_receive (struct mpac_io_net_handler * h)
{ 
  int i, len, write_len;
  write_len = h->msg_sz;
  
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_receive: null handle passed\n");
      return MPAC_FAILURE;
    }

  h->bytes_read = 0;
  for (i=0, len=0; i < h->msg_sz; )
    {
      if ((len =recv(h->accept_fd,
		     (char *) &h->buff[i], write_len, 0)) < 0)
	{
	  perror("mpac_io_tcp_receive: read failed: ");
	  return MPAC_FAILURE;
	}
      i += len;
      write_len -= len;
    }
   return MPAC_SUCCESS;
}


int mpac_io_tcp_close (struct mpac_io_net_handler * h)
{
  if (h == NULL)
    {
      fprintf(stderr, "mpac_io_tcp_close: null handle passed\n");
      return MPAC_FAILURE;
    }
  
  if(h->side == MPAC_IO_NET_TYPE_SEND)
    {
      if((close(h->fd))<0)
	{
	  perror("mpac_io_tcp_close: close failed for send socket: ");
	  return MPAC_FAILURE;
	}
      return MPAC_SUCCESS;
    }
  else
    {
      if((close(h->accept_fd))<0)
	{
	  perror("mpac_io_tcp_close: close failed for recv socket: ");
	  return MPAC_FAILURE;
	}
      return MPAC_SUCCESS;
    }
}
