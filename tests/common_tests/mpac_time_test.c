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
 
  Name:                mpac_time_test.c
  Author:              Abdul Waheed
  Date:                April 13, 2008
  Description:         Test for timing functions

*/

#include <stdio.h>

#include "mpac.h"

int main (int argc, char ** argv)
{
  int retVal;
  int i;
  double t1, t2, el;

  struct mpac_handle handle;
  printf("Testing mpac_get_clk function \n");
  retVal = mpac_init(&handle);
  if(retVal != MPAC_SUCCESS)
    {
      printf("Failure: mpac_init failed \n");
      exit(MPAC_FAILURE);
    }
  
  if ((t1 = mpac_time_ggettime()) < 0)
    {
      printf("Failure: mpac_get_clk returned error time: %10.2f\n", t1);
      exit (MPAC_FAILURE);
    }
  printf("Start time: %10.2f \n", t1);


  for(i=0;i<100000000;i++)
    i*i;

  if ((t2 = mpac_time_ggettime()) < 0)
    {
      printf("Failure: mpac_get_clk returned error time: %10.2f\n", t1);
      exit (MPAC_FAILURE);
    }
  printf("Stop time: %10.2f \n", t1);
  el = t2 - t1;
  printf("Elapsed time: %10.2f \n", el);

  return 0;
}

