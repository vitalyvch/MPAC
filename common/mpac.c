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
 * Name:           mpac.c
 * Author:         Abdul Waheed
 * Date:           April 13, 2008
 * Desription:     Initialization of common MPAC Tools related servies

*/

#include <stdio.h>
#include <sys/resource.h>

#include "mpac.h"

/**
* Any MPAC tools related services initial setup is handled by
* this function. 
*/
int mpac_init (struct mpac_handle* handle)
{
  struct rlimit rlim;
  

  if (handle == NULL)
    {
      fprintf(stderr, "mpac_init: null handle passed \n");
      return MPAC_FAILURE;
    }

  /* get the limit on the number of open fds */
  if ((getrlimit(RLIMIT_NOFILE, &rlim)) < 0)
    {
      fprintf(stderr, "mpac_init: getrlimit failed \n");
      return MPAC_FAILURE;
    }
  handle->fd_limit      = rlim.rlim_cur;      // rlim.rlim_max for max
  handle->num_core = mpac_thread_manager_No_of_cores();
  handle->res_gtd = mpac_time_overhead_gtd();
  handle->res_rtc = mpac_time_overhead_rtc();
  handle->res_swrtc = mpac_resolution_swrtc();
  handle->loop_overhead = mpac_time_loop_overhead();
  //Hassan - handle->min_measureable_duration = mpac_time_min_measurable();
  // Added by Hassan
  handle->res_clk_gtd = mpac_resolution_clock_gtd();
  handle->res_clk_rtc = mpac_resolution_clock_rtc();
  handle->min_measureable_duration_gtd = mpac_time_min_measurable_gtd(
            handle->res_clk_gtd, handle->loop_overhead, handle->res_gtd);
  handle->min_measureable_duration_rtc = mpac_time_min_measurable_rtc(
            handle->res_clk_rtc, handle->loop_overhead, handle->res_rtc);
  return MPAC_SUCCESS;
}

void mpac_do_nothing()
{
/*
    Relax
*/
}
/**
* Any MPAC tools related services clean up at the end is handled by
* this function. 
*/
int mpac_exit()
{
  return MPAC_SUCCESS;
}
