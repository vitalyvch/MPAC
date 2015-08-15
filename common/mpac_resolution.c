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
 * File:   mpac_resolution.c
 * Author: Ghulam Mustafa, M. Hasan Jamal
 */

#include"mpac.h"

int mpac_resolution_gtd(struct mpac_resval *rv, int nd)
{
  if ( nd < 1) return MPAC_FAILURE;
  
  struct mpac_resval* v = (struct mpac_resval*)rv; 
  int a, fcalls = 1, done = 0;
  long total = 0;
  long diffs[nd];
  struct timeval tfirst, tcurr;
  if (gettimeofday(&tfirst, NULL)) return MPAC_FAILURE;
  
  while (done < nd)
  {
    fcalls++;
    if (gettimeofday(&tcurr, NULL)) return MPAC_FAILURE;
    
    diffs[done] =  (tcurr.tv_sec - tfirst.tv_sec) +
                                   (tcurr.tv_usec - tfirst.tv_usec)/1000000.0;
    if (diffs[done] > 1.0E-9)
    {
      done++;
      tfirst = tcurr;
    }
  }
  v->itr=fcalls;
  for (a = 0; a < nd; a++)
      total += diffs[a];
  
  v->adiff=total/(double)nd;  // seconds
  
  return MPAC_SUCCESS;
}

int mpac_resolution_rtc(struct mpac_resval* rv, int nd)
{   
  if ( nd < 1) return MPAC_FAILURE;
  
  int a, ncalls = 1, ndone = 0;
  long total = 0;
  long tdiffs[nd];
  struct timespec start;
  struct timespec current;
  struct mpac_resval* v = (struct mpac_resval*)rv;
  
  if (clock_gettime(CLOCK_REALTIME, &start)) return MPAC_FAILURE;
  while (ndone < nd)
  {
    ncalls++;
    if (clock_gettime(CLOCK_REALTIME, &current)) return MPAC_FAILURE;
    tdiffs[ndone] =  (current.tv_sec - start.tv_sec) + 
                              (current.tv_nsec - start.tv_nsec) / 1000000000.0;
    if (tdiffs[ndone] > 1.0E-9)
    {
      ndone++;
      start.tv_sec = current.tv_sec;
      start.tv_nsec = current.tv_nsec;
    }
  }
  v->itr=ncalls;
  for (a = 0; a < nd; a++)
    total += tdiffs[a];
  
  v->adiff=total/(double)nd; // seconds
  return 0;
}

/** NOTE: As clock_* functions are in the real time library so 
 * we need -lrt in our linker statement.*/

double mpac_resolution_swrtc()
{
  struct timespec st;
  double sum=0;
  double res;
  int i;
  for (i = 0; i < 1000000; i++) {
    clock_getres(CLOCK_REALTIME, &st);
    sum=sum+st.tv_sec +st.tv_nsec/1000000000.0; //seconds
  }
 res = sum/1000000.0;
 return res;
}

/* Hassan
double mpac_resolution_clock_res()
{
  unsigned char d = 0x1a;
register int N = 2, sum = 0, c, y = d;
  double starttime, endtime;
  int j;
struct mpac_resval rv;
mpac_resolution_rtc(&rv, 20);
  double time_overhead = rv.adiff;
  //printf ("%.6f\n", time_overhead);
do
  {
    starttime = mpac_time_rgettime();
    for (j=0; j<N; j++)
      {
        c = j;
        if (c == y)
          sum = sum + c;
        else
          y = y + 1;
      }
    endtime =  mpac_time_rgettime() - starttime;
    N = N << 1;
  } while ( endtime - time_overhead < 0);
// to avoid compiler optimizations
int temp = c+ y+sum;
  return endtime; //returns sec
}
*/
//Added by Hassan
double mpac_resolution_clock_rtc()
{
  unsigned char d = 0x1a;
register int N = 2, sum = 0, c, y = d;
  double starttime, endtime;
  int j;

  double time_overhead = mpac_time_overhead_rtc()/1000000.0;

do
  {
    starttime = mpac_time_rgettime();
    for (j=0; j<N; j++)
      {
        c = j;
        if (c == y)
          sum = sum + c;
        else
          y = y + 1;
      }
    endtime =  mpac_time_rgettime() - starttime;
    N = N << 1;
  } while ( endtime - time_overhead < 0);
// to avoid compiler optimizations
int temp = c+ y+sum;
  return (endtime*1000000.0); //returns usec
}
double mpac_resolution_clock_gtd()
{
  unsigned char d = 0x1a;
register int N = 2, sum = 0, c, y = d;
  double starttime, endtime;
  int j;

  double time_overhead = mpac_time_overhead_gtd()/1000000.0;

do
  {
    starttime = mpac_time_ggettime();
    for (j=0; j<N; j++)
      {
        c = j;
        if (c == y)
          sum = sum + c;
        else
          y = y + 1;
      }
    endtime =  mpac_time_ggettime() - starttime;
    N = N << 1;
  } while ( endtime - time_overhead < 0);
// to avoid compiler optimizations
int temp = c+ y+sum;
  return (endtime*1000000.0); //returns usec
}
