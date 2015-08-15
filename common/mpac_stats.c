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
 * File:   mpac_stats.c
 * Author: Ghulam Mustafa
 */

#include "mpac.h"
#include "mpac_stats.h"


/** NOTE: As these functions uses math library so 
 * we need to add -lm in our linker command.*/

double mpac_stats_mean(double *data, int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  double	total = 0.0;
  int	a;
  for (a = 0; a < n; a++)
    total += data[a];
  
  return total/(double)n;
}

double mpac_stats_median(double *data, int n)
{    
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  
  qsort(data, n, sizeof(double), mpac_stats_cmp);
  if (n % 2) return data[(n/2)];
  return (data[(n/2)-1] + data[n/2]) / 2;
}


double mpac_stats_var(double *data,int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  double	total = 0.0;
  double	mean = mpac_stats_mean(data, n);
  int a;
  for (a = 0; a < n; a++)
    total += pow((data[a] - mean),2);
  
  return total/n;
}

double mpac_stats_std(double *data,int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  return (sqrt(mpac_stats_var(data, n)));
}

double mpac_stats_mode(double *data, int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;

  int a, freq=1, cfreq=1;
  double mode, current, next;
  qsort(data, n, sizeof(double),mpac_stats_cmp);

  current = mode = data[0];

  for (a = 1; a < n; ++a)
  {
    next = data[a];
    if (current != next)
    {
      current = next;
      cfreq = 0;
    }
    cfreq++;
    if (cfreq > freq)
    {
      mode = current;
      freq = cfreq;
    }
  }
  return mode;
}

int mpac_stats_conf_intr(double *ci,double *data,int n, double z)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  double mean = mpac_stats_mean(data, n);
  double std = mpac_stats_std(data,n);
  ci[0] = mean + z*(std/sqrt(n));
  ci[1] = mean - z*(std/sqrt(n));
  return MPAC_SUCCESS;
}

//Compare function required to call qsort() from stdlib.h

int mpac_stats_cmp(const void *a, const void *b)
{
  double* x= (double*) a; 
  double* y= (double*) b;
  if (*x < *y) return -1;
  if (*x > *y) return 1;
  return MPAC_SUCCESS;
}

double mpac_stats_min(double *data,int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  double min= data[0];
  int a; 
  for (a = 1; a < n; a++)
  {
    if (data[a] < min) 
      min = data[a];
  }
  return min;
}

double mpac_stats_max(double *data,int n)
{
  if (n < 1 || data == NULL) return MPAC_FAILURE;
  double max= data[0];
  int a; 
  for (a = 1; a < n; a++){
    if (data[a] > max) 
      max = data[a];
  }
  return max;
}

