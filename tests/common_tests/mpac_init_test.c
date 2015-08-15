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
 
  Name:                mpac_init_test.c
  Author:              Abdul Waheed, Ghulam Mustafa, M. Hasan Jamal
  Date:                April 13, 2008
  Description:         Test for mpac_init()

*/

#include <stdio.h>

#include "mpac.h"

int main (int argc, char ** argv)
{
  int ret_val;

  struct mpac_handle handle;

  printf("Testing mpac_init functions \n");
  ret_val = mpac_init(&handle);
  if(ret_val != MPAC_SUCCESS)
    {
      printf("Failure: mpac_init failed \n");
      exit(ret_val);
    }
  printf("mpac_init: current open fd limit: %d \n", handle.fd_limit);
  printf("mpac_init: number of processors : %i \n", handle.num_core);
  printf("mpac_init: Loop overhead : %f usec\n", handle.loop_overhead);
  printf("mpac_init: Minimum measureable task duration of gettimeofday: %f usec\n", handle.min_measureable_duration_gtd);
  printf("mpac_init: Minimum measureable task duration of realtime clock: %f usec\n", handle.min_measureable_duration_rtc);
  printf("mpac_init: Clock Resolution of gettimeofday: %f usec\n", handle.res_clk_gtd);
  printf("mpac_init: Clock Resolution of realtime clock: %f usec\n", handle.res_clk_rtc);
  printf("mpac_init: Overhead of gettimeofday() : %f usec\n", handle.res_gtd);
  printf("mpac_init: Overhead of realtime clock : %f usec\n", handle.res_rtc);
  printf("mpac_init: Resolution of system-wide realtime clock : %f usec\n", 1000000.0 * handle.res_swrtc);


  printf("Success: mpac_init works! \n");
  return ret_val;
}

