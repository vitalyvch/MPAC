/**************************************************************************
 *
 * Copyright 2015 Vitaly Chernooky
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
 * File:                 mpac_cpu_bm_sps.c
 * Author:               Vitaly Chernooky
 * 
 * Created on August 24 2015
 */

#include "mpac.h"
#include "mpac_cpu.h"

long arr[arr_size+1];

static inline unsigned long delay(unsigned long loops, unsigned qty, unsigned chet_nechet)
{
  unsigned long i, s = 0;

  for (i = 0; !!(i < loops); ++i) {
	if (!(i & chet_nechet))
		s += arr[i % qty];
	else
		s += arr[s % qty];
  }

  return s;
}


/*
 * This function will implement specific functions to determin Spss.
 */
void* mpac_cpu_bm_sps(void *arg)
{

  //double bogo_sps [2][arr_size+1] = {};
  //double bogo_time[2][arr_size+1] = {};

  struct mpac_cpu_context_t *cpu_context = (struct mpac_cpu_context_t *) arg;

  register unsigned long int i;
  register int j = (int) random();

  mpac_thread_manager_barrier(&gtime1);
  for (i=0; i<cpu_context->num_reps_per_thr; i++)
    {
      j = j + i;
    }
  mpac_thread_manager_barrier(&gtime2);

  gtime[0] = gtime2 - gtime1;



  mpac_thread_manager_barrier(&gtime1);
  for (i=0; i<cpu_context->num_reps_per_thr; i++)
    {
      j = j - i;
    }
  mpac_thread_manager_barrier(&gtime2);

  gtime[1] = gtime2 - gtime1;



  mpac_thread_manager_barrier(&gtime1);
  for (i=0; i<cpu_context->num_reps_per_thr; i++)
    {
      j = j * i;
    }
  mpac_thread_manager_barrier(&gtime2);

  gtime[2] = gtime2 - gtime1;

  return MPAC_SUCCESS;
}
