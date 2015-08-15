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
 * File:   mpac_net.c
 * Author: Ghulam Mustafa, Umair, Faiza Iftikhar
 *
 * Created on July 18, 2008, 11:49 PM
 */

#include "mpac.h"
#include "mpac_net.h"
#include "mpac_io_net.h"

static unsigned int mpac_net_msgcount = 0;
static pthread_mutex_t  mpac_net_mclock = PTHREAD_MUTEX_INITIALIZER;
static int sigflag = 0;
void mpac_net_usage(char *prog_name)
{
  fprintf(stdout,"========================================================\n");
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  
  printf("-c          Communication side: [S | R ] i.e. sender or receiver  \n");
  printf("            (Default: R)\n");
  printf("-e          Execution mode: [T | M ] i.e. Time-based or Message-count-based, respectively  \n");
  printf("            (Default: T)\n");
  printf("-d          Test duration (in seconds)  [Irrelevent in message-count-based measurement]\n");
  printf("            (Default: 10 seconds)\n");
  printf("-b          Size of receive buffer (in bytes)\n");
  printf("            (Default: 87380)\n");
  printf("-s          Size of Message/send buffer (in bytes)  \n");
  printf("            (Default: 16384)\n");
  printf("-l          Level of Concurrency (number of threads) \n");
  printf("            (Default: 1)\n");
  printf("-t          Protocol Identifier:[ 0 | 1 ] i.e. TCP or UDP, respectively \n");
  printf("            (Default: 0)\n");
  printf("-p          Port Number \n");
  printf("            (Default: 54321)\n");
  printf("-i          IP Address \n");
  printf("            (Default: 127.0.0.1)\n");
  printf("-a          Core Affinity: [1 | 0] i.e. Enabled or Disabled, respectively \n");
  printf("            (Default: 0)\n"); 
  printf("-m          Number of messages [Irrelevent in time-based measurement] \n");
  printf("            (Default: 550000)\n");
  printf("-h          For help \n");
  fprintf(stdout,"========================================================\n");

}

void mpac_net_print_config (struct mpac_net_config_t * net_config)
{
  //fprintf(stdout,"======================= \n");
  fprintf(stdout,"mpac_net_bm configuration\n");
  fprintf(stdout,"======================================================= \n");
  if(net_config->com_type == MPAC_IO_NET_TYPE_RECV)
  fprintf(stdout," Communication side                   : Reciever \n");
  if(net_config->com_type == MPAC_IO_NET_TYPE_SEND)
  fprintf(stdout," Communication side                   : Sender \n");
  fprintf(stdout," Size of receive buffer               : %ld Bytes\n", net_config->buf_size);
  fprintf(stdout," Size of send buffer                  : %li Bytes \n", net_config->msg_sz);
  fprintf(stdout," Number of threads                    : %d \n", net_config->num_thrs);  
  //fprintf(stdout," Protocol ID (0=>TCP, 1=>UDP)         : %d \n", net_config->protocol_num);
    if(net_config->protocol_num == 0)
  fprintf(stdout," Protocol                             : TCP \n");
  if(net_config->protocol_num == 1)
  fprintf(stdout," Protocol                             : UDP \n");
  fprintf(stdout," Port number                          : %d \n", net_config->port_num);
  fprintf(stdout," IP address                           : %s \n", net_config->ip);
  if (net_config->aff)
  fprintf(stdout," Core Affinity Support                : Enabled \n");
  else
  fprintf(stdout," Core Affinity Support                : Disabled \n");
  fprintf(stdout," Message Size                         : %li Bytes \n", net_config->msg_sz);
  if(net_config->torm == 'M')
  {
  fprintf(stdout," Execution mode                       : Message-count-based\n");
  fprintf(stdout," Number of messages                   : %d \n", net_config->num_msg);
  }
  if(net_config->torm == 'T')
  {
  fprintf(stdout," Execution mode                       : Time-based\n");
  fprintf(stdout," Test duration                        : %d \n", net_config->interval);
  }
  fprintf(stdout,"======================================================= \n");
  if(net_config->com_type == MPAC_IO_NET_TYPE_RECV &&
                           net_config->torm == 'M' &&
                    net_config->protocol_num == MPAC_UDP)
  {
  fprintf(stdout,"If necessary, Press Cntl-C, for graceful termination.  \n");
  fprintf(stdout,"======================================================= \n");
  }
}

void mpac_net_default_config (struct mpac_net_config_t * net_config)
{
    
  net_config->num_cores     =   mpac_thread_manager_No_of_cores();  
  net_config->com_type      =   MPAC_IO_NET_TYPE_RECV;     //server
  net_config->buf_size      =   87380;  //bytes
  net_config->num_thrs      =   1;
  net_config->protocol_num  =   0;   //TCP
  net_config->port_num      =   54321;
  strcpy(net_config->ip,"127.0.0.1");
  net_config->aff           =   0;
  net_config->num_msg       =   550000;
  net_config->msg_sz        =   16384;  //bytes
  net_config->sockopt       =   0;
  net_config->torm          =   'T';
  net_config->interval      =   10.0;  //seconds
  }

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */

int mpac_net_parse_cmdl(int argc, char **argv, struct mpac_net_config_t *net_config)
{
  int c;
  /* start with the default values */
  mpac_net_default_config(net_config);
while ((c = getopt(argc, argv, "c:b:l:t:p:i:a:s:m:e:d:h")) != -1){
 switch (c){
  case 'c':
          if (optarg[0] == 'R')
            {
            net_config->com_type = MPAC_IO_NET_TYPE_RECV;
           // net_config->buf_size = 87380;
            }
          else if (optarg[0] == 'S')
            {
            net_config->com_type=MPAC_IO_NET_TYPE_SEND;
          //  net_config->buf_size = 16384;
            }
          else
            {
	      fprintf(stderr, "In mpac_net_parse_cmdl(): Communication side should be either R or S\n");
	      exit (MPAC_FAILURE);
	    }
            break;
  case 'b':
          if (atoi(optarg) < 1)
            {
              fprintf(stderr, "In mpac_net_parse_cmdl(): Illegal buffer size\n");
	      exit (MPAC_FAILURE);
            }
          else
            {
              net_config->buf_size = atol(optarg);
              net_config->sockopt = 1;
            }
            break;                
  case 'l':
          if(atoi(optarg) < 1)
              {
	      fprintf(stderr, "In mpac_net_parse_cmdl(): Level of concurrency should be at least 1 thread \n");
	      exit(MPAC_FAILURE);
	    }
            else net_config->num_thrs = atoi(optarg);            
/*
          if (net_config->com_type == MPAC_IO_NET_TYPE_RECV &&
                       net_config->protocol_num == MPAC_UDP &&
                                        net_config->torm == 'M')
            net_config->num_thrs = 1;
*/
	  break;
          
  case 't':          
	  if( atoi(optarg) != 0 && atoi(optarg) != 1 && atoi(optarg) != 2){
	      fprintf(stderr, "mpac_net_parse_cmdl: Incorrect Protocol identifier\n");		      
	      exit(MPAC_FAILURE);
	    }
          net_config->protocol_num = atoi(optarg);
	  break;
          
  case 'p':
	  if( atoi(optarg) < 0){
	      fprintf(stderr, "mpac_net_parse_cmdl: Invalid port number\n");
	      exit(MPAC_FAILURE);
	    }
          net_config->port_num = atoi(optarg);
	  break;
          
  case 'i':	  
          strcpy(net_config->ip, optarg);
	  break;                 
 case 'a':
           if( atoi(optarg) != 0 && atoi(optarg) != 1 ){
	      fprintf(stderr, "mpac_net_parse_cmdl: Illegal affinity option\n");		      
	      exit(MPAC_FAILURE);
	    }          
          net_config->aff = atoi(optarg);
	  break;
          
 case 's':
	  if( atoi(optarg) < 1){
	      fprintf(stderr, "mpac_net_parse_cmdl: Illegal messages size\n");
	      exit(MPAC_FAILURE);
	    }
          net_config->msg_sz = atol(optarg);
          net_config->sockopt= 1;
	  break;
          
 case 'm':
	  if( atoi(optarg) < 1){
	      fprintf(stderr, "mpac_net_parse_cmdl: Illegal number of messages\n");
	      exit(MPAC_FAILURE);
	    }
          net_config->num_msg = atoi(optarg);
	  break;

case 'e':
	  if( optarg[0] != 'T' && optarg[0] != 'M'){
	      fprintf(stderr, "mpac_net_parse_cmdl: Unsupported execution mode, enter T or M\n");
	      exit(MPAC_FAILURE);
	    }
          net_config->torm = optarg[0];
	  break;

case 'd':
	  if( atoi(optarg) < 1){
	      fprintf(stderr, "mpac_net_parse_cmdl: Illegal number of seconds\n");
	      exit(MPAC_FAILURE);
	    }
          net_config->interval = atoi(optarg);
	  break;

 case 'h':
	  mpac_net_usage(argv[0]);
	  exit (MPAC_SUCCESS);
 case '?':
          mpac_net_usage(argv[0]);    
	  return MPAC_FAILURE;
          
  default:            
          mpac_net_usage(argv[0]);
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}

void mpac_net_output(double el_time, double mbps, double mps, double lpm){
  printf("mpac_net_bm finished successfully \n\n");
  printf("=================\n");
  printf("     Results     \n");
  printf("=================\n");
  printf("Elapsed time (sec),Throughput(Mbps),");
  printf("Throughput(messages/sec), Latency per message(usec)\n");
  printf("%.2lf,%.2lf,%.2lf,%.2lf\n", el_time, mbps, mps,lpm * 1000000.0);
}

int mpac_net_arg_handler(int argc, char** argv, struct mpac_net_config_t* net_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      //mpac_net_usage(argv[0]);
      mpac_net_default_config(net_config);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{      
    if((ret = mpac_net_parse_cmdl(argc, argv, net_config))!= MPAC_SUCCESS)
      {
	  fprintf(stderr, "mpac_net: failed to parse commandline args \n");
	  mpac_net_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
    mpac_net_print_config((struct mpac_net_config_t *)net_config);
    return MPAC_SUCCESS;
}


struct mpac_net_context_t** 
mpac_net_configure(struct mpac_net_config_t net_config)
{
  int i;
  struct mpac_net_context_t** net_context;
  
  mpac_net_setmc (net_config.num_msg);                       
  net_context=(struct mpac_net_context_t**)
    malloc(net_config.num_thrs*sizeof (struct mpac_net_context_t*));

  if (net_context == NULL)
    {
      fprintf(stderr,"mpac_net_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }
  for (i = 0; i < net_config.num_thrs; i++)
    {
      net_context[i]=(struct mpac_net_context_t*)
	malloc(sizeof(struct mpac_net_context_t));
      if (net_context[i] == NULL)
	{
	  fprintf(stderr,"mpac_net_bm: Not enough memory \n " );
	  return MPAC_NULL;                    
	}
      net_context[i]->h = (struct mpac_io_net_handler*) 
	malloc (sizeof (struct mpac_io_net_handler));
      
        mpac_io_net_init(net_context[i]->h, net_config.ip, 
                         net_config.port_num,
                         net_config.com_type, 
			 net_config.msg_sz,
			 net_config.buf_size); 
      
      net_context[i]->n_msgs    = net_config.num_msg;    
      net_context[i]->num_wrkrs = net_config.num_thrs;
      net_context[i]->thr_id    = i;    
      net_context[i]->u_time    = 0.0;
      net_context[i]->bytes_x   = 0;    
      net_context[i]->h->accept_fd   = -1;
      net_context[i]->h->sockopt = net_config.sockopt;
      net_context[i]->interval  = net_config.interval;
    }
  return net_context;
}

/* Get message count */
int mpac_net_getmc() 
{
   return mpac_net_msgcount;     
}

/* Set message count - set once */
int mpac_net_setmc(int mc) 
{
  if (mpac_net_msgcount == 0)
    {
        mpac_net_msgcount = mc;  
        return MPAC_SUCCESS;
    }
  else {
      printf ("mpac_net_setmc(): Message Counter already set\n");
      return MPAC_FAILURE;
    }
   
}

/* Decrement message count */
int mpac_net_decrementmc(int nth) 
{
  if (nth == 1)
    {
      mpac_net_msgcount--;
      return MPAC_SUCCESS;      
    }
    
   int err;
   if ((err = pthread_mutex_lock(&mpac_net_mclock)))
      return err;
   mpac_net_msgcount--;    
   return pthread_mutex_unlock(&mpac_net_mclock);
}

int mpac_net_incrementmc(int* currentmc) 
{
   int err;
   if ((err = pthread_mutex_lock(&mpac_net_mclock)))
      return err;
   mpac_net_msgcount++; 
   *currentmc = mpac_net_msgcount;
   return pthread_mutex_unlock(&mpac_net_mclock);
}

/*Message generator*/
int mpac_net_generatemsg(char* buff,int size)
{    
    if (buff == NULL) return MPAC_FAILURE;

  int i;
  char data[] = "mpac.";

  for(i = 0 ; i < size ; i++)
       buff[i] = data[i%(strlen(data))];

  return MPAC_SUCCESS;
}


void mpac_net_init_recv (struct mpac_net_thdata* td)
{
  mpac_thread_manager_initlock(&td->fdlck,td->num_th);
  td->fds = (int*) calloc(td->num_th,sizeof(int));
  td->getfd = 0;
  td->setfd = 0;
  td->fd = -1;  
}

int mpac_net_setafd (struct mpac_net_thdata* td)
{
  pthread_mutex_lock(&td->fdlck.lock);
  td->fds[td->setfd] = td->fd;
  if ( ++(td->setfd) == td->num_th)
    td->setfd = 0;
  if (td->setfd == td->getfd)
    {
      printf ("setfd = getfd = %d", td->setfd);
      //return MPAC_FAILURE;                
    }
  pthread_cond_signal(&td->fdlck.go);
  pthread_mutex_unlock(&td->fdlck.lock);  
  return MPAC_SUCCESS;
}
 
int mpac_net_getafd (struct mpac_net_thdata* td)
{ 
  int afd; 
  pthread_mutex_lock(&td->fdlck.lock);
  while (td->getfd == td->setfd)
    pthread_cond_wait(&td->fdlck.go, &td->fdlck.lock);
   afd = td->fds[td->getfd];	
  if (++(td->getfd) == td->num_th)
    td->getfd = 0;
  pthread_mutex_unlock(&td->fdlck.lock);  
  return afd;	
}
void mpac_net_setsigflag()
{
    sigflag = 1;
    sleep(1);
}
int mpac_net_getsigflag()
{
    return sigflag;
}

void  mpac_net_tcp_report(struct mpac_net_config_t net_config, struct mpac_net_context_t  **net_context)
{
    int i;
  double t_time = 0.0, t_msgs = 0, mbps, mps, lpm;
    if (net_config.torm == 'T' )
    {
        for(i=0 ; i < net_config.num_thrs ; i++)
	       t_msgs    += net_context[i]->bytes_x;
          double temp1, temp2;
          t_time = net_config.interval;
          temp1 = (double)t_msgs / (1024.0*1024.0);
          temp2 = (double)net_config.msg_sz * 8.0;

	  mbps      = (temp1*temp2)/ (double) t_time;
	  mps       = ((double)t_msgs) / (double)t_time;
	  lpm       = 1.0/mps;

	  printf("Elapsed time: %f\n", t_time);
	  printf("num_msg: %f msg_sz: %ld mbps: %.21f mps: %.21f lpm: %.21f\n",
		 t_msgs, net_config.msg_sz, mbps, mps, lpm);
          fflush(stdout);
	  mpac_net_cleanup();
	  mpac_net_output( t_time, mbps,  mps, lpm);

    }
    else if(net_config.torm == 'M')
    {
        for(i=0 ; i < net_config.num_thrs ; i++)
	       t_time    += net_context[i]->u_time;

          t_time     = t_time/net_config.num_thrs;
          double temp1, temp2;
          long nm;
          nm = net_config.num_msg / net_config.num_thrs;
          nm = nm * net_config.num_thrs;
          temp1 = (double)nm / (1024.0*1024.0);
          temp2 = (double)net_config.msg_sz * 8.0;

	  mbps      = (temp1*temp2)/ (double) t_time;
	  mps       = ((double)nm) / t_time;
	  lpm       = 1.0/mps;

	  printf("Elapsed time: %f\n", t_time);
	  printf("num_msg: %d msg_sz: %ld mbps: %f mps: %f lpm: %f\n",
		 net_config.num_msg, net_config.msg_sz, mbps, mps, lpm);
	  mpac_net_cleanup();
	  mpac_net_output( t_time, mbps,  mps, lpm);

    }
}
void  mpac_net_udp_report(struct mpac_net_config_t net_config, struct mpac_net_context_t  **net_context)
{
    int i;
  double t_time = 0.0, t_msgs = 0, mbps, mps, lpm;
    if (net_config.torm == 'T' )
    {
            for(i=0 ; i < net_config.num_thrs ; i++)
                  t_msgs    += net_context[i]->bytes_x;
              double temp1, temp2;
              t_time = net_config.interval;
              temp1 = (double)t_msgs / (1024.0*1024.0);
              temp2 = (double)net_config.msg_sz * 8.0;

              mbps      = (temp1*temp2)/ (double) t_time;
              mps       = ((double)t_msgs) / (double)t_time;
              lpm       = 1.0/mps;

              printf("Elapsed time: %f\n", t_time);
              printf("num_msg: %f msg_sz: %ld mbps: %.21f mps: %.21f lpm: %.21f\n",
                                       t_msgs, net_config.msg_sz, mbps, mps, lpm);
              fflush(stdout);
              mpac_net_cleanup();
              mpac_net_output( t_time, mbps,  mps, lpm);

    }
    else if(net_config.torm == 'M')
    {
        for(i=0 ; i < net_config.num_thrs ; i++)
	       t_time    += net_context[i]->u_time;

          t_time     = t_time/net_config.num_thrs;
          long nm;
          nm = net_config.num_msg / net_config.num_thrs;
          nm = nm * net_config.num_thrs;
	  mbps      = (((double)nm) * ((double)net_config.msg_sz*8.0/(1024.0*1024.0)))/t_time;
	  mps       = ((double)nm) / t_time;
	  lpm       = 1.0/mps;
	  mpac_net_cleanup();
	  mpac_net_output( t_time, mbps,  mps, lpm);

    }
}
