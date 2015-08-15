/*
  mpac.h

*/

#ifndef MPAC_H
#define MPAC_H

#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif 

#ifndef __USE_GNU
#define __USE_GNU
#endif 

#define MPAC_SUCCESS 0
#define MPAC_FAILURE -1
#define MPAC_NULL NULL

#define NO 0
#define YES 1

#define mpac_min(a,b) ((a)<(b))?(a):(b)
#define mpac_max(a,b) ((a)>(b))?(a):(b)

#define MPAC_RD 0
#define MPAC_WR 1
#define MPAC_RW 2

#define mpac_K 1024
#define mpac_M mpac_K*mpac_K
#define mpac_G mpac_M*mpac_K
#define NO_OP() mpac_do_nothing()



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <limits.h>
#include <sys/select.h>
#include <sys/syscall.h>
#include <sched.h>
#include <float.h>

//#include <stdio.h>
//#include<sys/socket.h>
#include <arpa/inet.h>
#include<linux/if_ether.h>
#include<linux/if_packet.h>
#include<net/if.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <string.h>


#include "config.h"
#include "mpac_time.h"
#include "mpac_resolution.h"
#include "mpac_thread_manager.h"
#include "mpac_stats.h"
#include "mpac_io_file.h"
#include "mpac_io_net.h"
#include "mpac_io_tcp.h"
#include "mpac_io_udp.h" 
#include "mpac_io_raw.h"
#include "mpac_signal.h"
#include "mpac_cache.h"

struct mpac_handle
{
  int fd_out;
  int fd_err;
  int fd_limit;
  int num_core;
  double res_gtd;   /*Resolution of gettimeofday() in usec*/
  double res_rtc;   /*Resolution of clock_gettime() in nsec*/
  double res_swrtc; /*Resolution of sytem-wide realtime clock in nsec*/
  double loop_overhead;
  //Hassan-double min_measureable_duration;
  /*Added by Hassan*/
  double res_clk_gtd, res_clk_rtc;
  double min_measureable_duration_rtc, min_measureable_duration_gtd ;
} ;

int mpac_init (struct mpac_handle*);
void mpac_do_nothing();
int mpac_exit();

#endif
