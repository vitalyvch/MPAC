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
 * File:   mpac_net_bm.c
 * Author: Ghulam Mustafa, Umair Naushad
 *
 * Created on July 19, 2008
 */

#ifdef DBG_MPAC_NET
#define DBG(x) x
#else
#define DBG(x)
#endif


#include"mpac.h"
#include"mpac_net.h"
#include "mpac_io_tcp.h"

struct mpac_net_config_t     net_config;
struct mpac_net_context_t  **net_context;
mpac_thread_manager_t        tm;
pthread_attr_t               atr;
struct mpac_io_net_handler   h;


int main(int argc, char** argv)
{       
  int i;
  mpac_signal_sighandler(SIGINT, mpac_net_cleanup);
  mpac_signal_sighandler(SIGALRM, mpac_net_setsigflag);
  
  mpac_net_arg_handler(argc , argv, &net_config);
  net_context = mpac_net_configure(net_config);

  /*Senders*/
  if (net_config.com_type == MPAC_IO_NET_TYPE_SEND)
    {
      /*TCP Sender*/
      if(net_config.protocol_num == MPAC_TCP)    
	{
	  DBG(printf("TCP SENDER 1"););
          if(net_config.torm == 'T')
          {            
	      mpac_signal_setitimer(net_config.interval);
	      mpac_thread_manager_startj(&tm,
					 net_config.num_thrs,
					 &atr,
					 net_config.aff,
					 mpac_net_tcpsender_timed,
					 (void**)net_context);
          if(NET_DBG) printf("Back to main thread\n");
          mpac_net_tcp_report(net_config,net_context);
          return MPAC_SUCCESS;
          }
          else if(net_config.torm == 'M')
          {
              if(net_context[0]->num_wrkrs == 1)
	    mpac_net_tcpsender(net_context[0]);
	  else if (net_context[0]->num_wrkrs > 1)
	    {	      
	      mpac_thread_manager_startj(&tm,
					 net_config.num_thrs,
					 &atr,
					 net_config.aff,
					 mpac_net_tcpsender,
					 (void**)net_context);
              if(NET_DBG) printf("Back to main thread\n");
	    }
	  mpac_net_tcp_report(net_config,net_context);
          return MPAC_SUCCESS;
          }
	  
	}//TCP ends
      /*UDP Sender*/
      else if(net_config.protocol_num == MPAC_UDP)
	{
          if (net_config.torm == 'M')
          {
              if(net_context[0]->num_wrkrs == 1)
	    mpac_net_udpsender(net_context[0]);
	  else if (net_context[0]->num_wrkrs > 1)
            mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_udpsender,
                                       (void**)net_context);
              if(NET_DBG) printf("Back to main thread\n");
              mpac_net_udp_report(net_config,net_context);
              return MPAC_SUCCESS;
              
          }
          else if(net_config.torm == 'T')
          {
              mpac_signal_setitimer(net_config.interval);
              mpac_thread_manager_startj(&tm,                
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_udpsender_timed,
                                       (void**)net_context);
              if(NET_DBG) printf("Back to main thread\n");
              mpac_net_udp_report(net_config,net_context);
              return MPAC_SUCCESS;
          }
      }
    }//SENDER ends
  /*Receivers*/
  else if (net_config.com_type == MPAC_IO_NET_TYPE_RECV)
    {          
      if(net_config.protocol_num == MPAC_TCP)
	{
          mpac_io_net_init(&h, net_config.ip, 
			   net_config.port_num,			   
			   MPAC_IO_NET_TYPE_RECV, 
			   net_config.msg_sz, net_config.buf_size);	 
	  mpac_io_tcp_open_receiver (&h);
          for (i = 0; i < net_config.num_thrs; i++)
          {
              mpac_io_tcp_accept_receiver (&h);
              net_context[i]->h->accept_fd = h.accept_fd;
          }
          if(net_config.torm == 'T')
          {
            mpac_signal_setitimer(net_config.interval);
            mpac_thread_manager_startj(&tm,  
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_tcpreceiver_timed,
                                       (void**)net_context);
            if(NET_DBG) printf("Back to main thread\n");
            mpac_net_cleanup();
           return MPAC_SUCCESS;
          }
          else if(net_config.torm == 'M')
          {
              mpac_thread_manager_startj(&tm,
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_tcpreceiver,
                                       (void**)net_context);
            if(NET_DBG) printf("Back to main thread\n");
           mpac_net_cleanup();
           return MPAC_SUCCESS;
          }                      
	}
    
      /*UDP Receiver*/ 
      else if(net_config.protocol_num == MPAC_UDP)
      {
          if (net_config.torm == 'T')
          {
              mpac_signal_setitimer(net_config.interval+2);
              mpac_thread_manager_startj(&tm,
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_udpreceiver_timed,
                                       (void**)net_context);
            if(NET_DBG) printf("Back to main thread\n");
            mpac_net_cleanup();
           return MPAC_SUCCESS;
          }
          else if(net_config.torm == 'M')
          {
/*
              mpac_thread_manager_startj(&tm,
                                       net_config.num_thrs,
                                       &atr,
                                       net_config.aff,
                                       mpac_net_udpreceiver,
                                       (void**)net_context);
*/
              mpac_net_udpreceiver(net_context[0]);
            if(NET_DBG) printf("Back to main thread\n");
            mpac_net_cleanup();
           return MPAC_SUCCESS;
          }
      }	        
    }
  return MPAC_SUCCESS;
}

void mpac_net_cleanup()
{
  int i;
  printf ("\nFreeing resources...\n");
  if((net_config.com_type == MPAC_IO_NET_TYPE_RECV) && (net_config.protocol_num == 0))
  {
    close (h.fd);
    free(h.buff);
    free(h.recv_host);
  }
    
  if ((net_config.com_type == MPAC_IO_NET_TYPE_RECV) &&
     (net_config.protocol_num == MPAC_UDP))
  {
      //free(net_context[0]->h->buff);
      free(net_context[0]->h->recv_host);
      free(net_context[0]->h);
      free(net_context[0]);
  }
  else
  {
      for (i = 0; i < net_config.num_thrs; i++)
      {
      if(net_config.com_type ==  MPAC_IO_NET_TYPE_RECV)
      free(net_context[i]->h->buff);
      free(net_context[i]->h->recv_host);
      free(net_context[i]->h);
      free(net_context[i]);
      }
      free(net_context);
  }  
  printf ("Shutdown successfully.\n");
  if (net_config.com_type == MPAC_IO_NET_TYPE_RECV)
    exit (0);
}
