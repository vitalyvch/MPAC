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
 * File:   mpac_net_udpreceiver.c
 * Author: Ghulam Mustafa
 *
 * Created on July 28, 2008, 8:40 PM
 */

#include"mpac.h"
#include "mpac_net.h"

#ifdef DBG_MPAC_NET
#define DBG(x) x
#else
#define DBG(x)
#endif

void* mpac_net_udpreceiver_timed(void* arg)
{
 struct mpac_net_context_t * net_context =
   (struct mpac_net_context_t*) arg;
/*
 int i,sel_ret, nfds,
*/
 int msgcount, num_thrs, f = 0;
 //fd_set readyset;

 msgcount = 0;
 num_thrs = net_context->num_wrkrs;
 //FD_ZERO(&readyset);

 
/*
 while(num_thrs)
   {
*/
     mpac_io_udp_open_receiver (net_context->h);
     printf("Test in progress...\n");
/*
     FD_SET(net_context->h->fd, &readyset);
     nfds = net_context->h->fd;
     if ( (sel_ret = select(nfds+1, &readyset, NULL, NULL, NULL)) < 0)
       {
	 if (errno == EINTR)	continue;
	 else  perror("select failed");
       }
     for(i = 0; i <= nfds; i++)
       {
	 if(FD_ISSET(i, &readyset))
         {
	   if(i == net_context->h->fd)
	     {
               
*/
	       while(!f)
		 {
		   mpac_io_udp_receive(net_context->h);
		   msgcount++;
                   f = mpac_net_getsigflag();
		   if(NET_DBG) printf ("Thread%d: %d messages received\n",
                           net_context->thr_id ,msgcount);
		 }
/*
	     }
	   if(!f) break;
	 }
       }
*/
/*
     num_thrs--;
*/
     mpac_io_udp_close (net_context->h);
     //if(!f) continue;
/*
   } 
*/
 return NULL;
}
