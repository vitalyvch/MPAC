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
 * Name:           mpac_time.c
 * Author:         Abdul Waheed, Ghulam Mustafa, M. Hasan Jamal
 * Date:           April 13, 2008 
 * Desription:     Timing services
*/

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "mpac.h"
#include "mpac_time.h"


/**
 *  This function returns the current system time as a double percision
 *  value in micro seconds. 
 */

double mpac_time_ggettime_usec()
{
  
  struct timeval tp;
  if(gettimeofday(&tp, 0)) 
    {
      fprintf(stderr, "mpac_time_ggettime: gettimeofday failed \n");
      return MPAC_FAILURE;
    }
  return ( (double) tp.tv_sec* 1000000.0 + (double) tp.tv_usec);
}

/**
 *  This function returns the current system time as a double percision
 *  value in nano seconds. 
 */
double mpac_time_rgettime_nsec()
{
  struct timespec cur_time;
  if(clock_gettime(CLOCK_REALTIME, &cur_time)) 
    {
      fprintf(stderr, "mpac_time_rgettime: clock_gettime failed \n");
      return MPAC_FAILURE;
    }
  return (double)(cur_time.tv_sec * 1000000000.0 + cur_time.tv_nsec);
}

/**
 *  This function returns the current system time as a double percision
 *  value in seconds. 
 */

double mpac_time_ggettime()
{ 
  struct timeval tp;
  if(gettimeofday(&tp, 0)) 
    {
      fprintf(stderr, "mpac_time_ggettime: gettimeofday failed \n");
      return MPAC_FAILURE;
    }  
  return ( (double) tp.tv_sec + (double) tp.tv_usec/ 1000000.0); // returns sec
}

/**
 *  This function returns the current system time as a double percision
 *  value in seconds. 
 */
double mpac_time_rgettime()
{
  struct timespec cur_time;
  if(clock_gettime(CLOCK_REALTIME, &cur_time))
    {
    fprintf(stderr, "mpac_time_rgettime: clock_gettime failed \n");
    return MPAC_FAILURE;
    }
  return ((double) cur_time.tv_sec + (double) cur_time.tv_nsec/1000000000.0); // return sec
}
/*
Idea for mpac_time_loop_overhead() is adapted from LMBench
*/
double mpac_time_loop_overhead()
{
  double st1 = 0.0, st2 = 0.0, tt1,
            et1 = 0.0, et2 = 0.0, tt2;
  register int tmp1, tmp2;
  int i, N = 1000000000;
  st1 = mpac_time_ggettime();
  for (i = 0;  i < N ; i++)
    {
      tmp1 = i*i + 56;
    }
  et1 = mpac_time_ggettime();
  tt1 = et1 - st1;
  sleep(1);
  st2 = mpac_time_ggettime();
  for (i = 0;  i < N ; i++)
    {
      tmp2 = i*i + 37;
      tmp1 = i*i +75+tmp2;
    }
  et2 = mpac_time_ggettime();
  tt2 = et2 - st2;
  return (double)((2*tt1 - tt2)/(double)N) * 1000000.0; //returns usec
}
/*Hassan
double mpac_time_min_measurable()
{
  struct mpac_resval time;
   mpac_resolution_rtc(&time, 20);
  return (10 * ( mpac_resolution_clock_res() - mpac_time_loop_overhead() - time.adiff));
}
*/

double  mpac_time_wctime(void* (*func)(void*),void* arg)
{
  time_t strt;
  int i;
  strt=time(NULL);
  (*func)(arg);
  return difftime(time(NULL),strt);  //in sec
}
/*
 * mpac_time_overhead_rtc() returns timing overhead of
 * clock_gettime(CLOCK_REALTIME, struct timespec*) on micro-seconds
*/
double mpac_time_overhead_rtc()
{
  struct timespec start;
  struct timespec current;
  struct timespec ts, diff;
  int i;
  if (clock_gettime(CLOCK_REALTIME, &start)) return -1;
  for (i = 0; i < 1000000; i++)
  {
    clock_gettime(CLOCK_REALTIME, &ts);
  }
  if (clock_gettime(CLOCK_REALTIME, &current)) return -1;
  if (current.tv_nsec < start.tv_nsec)
  {
      --current.tv_sec;
      current.tv_nsec += 1000000000;

  }
  diff.tv_sec  = current.tv_sec - start.tv_sec;
  diff.tv_nsec = current.tv_nsec - start.tv_nsec;
  return (double)(1000000.0*diff.tv_sec + (diff.tv_nsec/1000.0))/1000000.0;
}
/*
 mpac_time_overhead_gtd() returns timing overhead of gettimeofday() in micro-seconds
*/
double mpac_time_overhead_gtd()
{
  struct timeval tv, diff;

  struct timeval tend;
  struct timeval tsta;
  int i;
  if (gettimeofday(&tsta, NULL)) return -1;
  for (i = 0; i < 1000000; i++)
  {
    gettimeofday(&tv, NULL);
  }
  if (gettimeofday(&tend, NULL)) return -1;
  if (tend.tv_usec < tsta.tv_usec)
  {
      --tend.tv_sec;
      tend.tv_usec += 1000000;      
  }
  diff.tv_sec  = tend.tv_sec - tsta.tv_sec;
  diff.tv_usec = tend.tv_usec - tsta.tv_usec;
  return (double)(1000000.0 * diff.tv_sec + diff.tv_usec)/1000000.0;
}

int mpac_time_frt_gtd(struct timeval *tv, void* (*func)(void*),void* arg)
{
  struct timeval* v = (struct timeval*)tv;

  struct timeval tend;
  struct timeval tsta;
  int i;
  if (gettimeofday(&tsta, NULL)) return -1;
  for (i = 0; i < 1000000; i++){
    (*func)(arg);
  }
  if (gettimeofday(&tend, NULL)) return -1;
  v->tv_sec  = (tend.tv_sec - tsta.tv_sec)/1000000;
  v->tv_usec = (tend.tv_usec - tsta.tv_usec)/1000000;
  return MPAC_SUCCESS;
}

int mpac_time_frt_rtc(struct timespec *ts, void* (*func)(void*),void* arg)
{
  struct timespec start;
  struct timespec current;
  struct timespec* v = (struct timespec*)ts;
  int i;
  if (clock_gettime(CLOCK_REALTIME, &start)) return -1;
  for (i = 0; i < 1000000; i++){
    (*func)(arg);
  }                
  if (clock_gettime(CLOCK_REALTIME, &current)) return -1;
  
  v->tv_sec  = (current.tv_sec - start.tv_sec)/1000000.0;
  v->tv_nsec = (current.tv_nsec - start.tv_nsec)/1000000.0;
  return MPAC_SUCCESS;
}

/*
Added by Hassan
*/
double mpac_time_min_measurable_gtd(double res_clk, double loop_o, double time_o)
{
  double mm_t = 10 * ( res_clk - loop_o - time_o );
  return mm_t;
}

double mpac_time_min_measurable_rtc(double res_clk, double loop_o, double time_o)
{
  double mm_t = 10 * ( res_clk - loop_o - time_o );
  return mm_t;
}
