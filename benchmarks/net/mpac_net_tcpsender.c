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
 * File:   mpac_net_tcpsender.c
 * Author: Ghulam Mustafa, Faiza Iftikhar
 *
 */

#include"mpac.h"
#include "mpac_net.h"

#ifdef DBG_MPAC_NET
#define DBG(x) x
#else
#define DBG(x)
#endif

void* mpac_net_tcpsender(void* arg)
{
  struct mpac_net_context_t * net_context = 
                                  (struct mpac_net_context_t*) arg;
  double  stop_time = 0.0,
          start_time = 0.0;
  net_context->bytes_x = 0;
  net_context->u_time = 0.0;
  char msg[net_context->h->msg_sz];
  
  int   message_count, num_thrs, i;
  
  num_thrs = net_context->num_wrkrs;
  message_count = mpac_net_getmc()/num_thrs;
 
  mpac_net_generatemsg(msg,net_context->h->msg_sz);
  
  //for (i = 0; i < net_context->h->msg_sz; i++)
   //             msg[i] = htonl(msg[i]);

  net_context->h->buff = msg;  

  mpac_io_tcp_open_sender (net_context->h);  
  start_time = mpac_time_ggettime();
  printf("Test in progress...\n");
  while(message_count > 0)
    {
      if(message_count <= 0) break;
        mpac_io_tcp_write (net_context->h);
        message_count--;
        
        if(NET_DBG) printf("Thread%d: %d messages to send\n",
		   net_context->thr_id, message_count);
    }
   stop_time = mpac_time_ggettime();
   net_context->u_time = stop_time - start_time;
   
  mpac_io_tcp_close (net_context->h);
  close(net_context->h->fd);
  return MPAC_NULL;
}
