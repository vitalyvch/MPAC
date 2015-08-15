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
 
  Name:                mpac_io_file_test.c
  Author:              Abdul Waheed
  Date:                April 13, 2008
  Description:         Test for file I/Ofunctions

*/

#include <stdio.h>

#include  "mpac.h"

int main (int argc, char ** argv)
{
  int retVal;
  int i;
  double t1, t2, el;
  unsigned char * buf;
  int buf_sz;

  if (argc !=2 )
    {
      printf ("Usage: %s <file name> \n", argv[0]);
      exit (-1);
    }

  printf("Testing mpac_io_file functions for: %s \n", argv[1]);
  retVal = mpac_io_file_length(argv[1]);
  if(retVal < 0 )
    {
      printf("Failure: mpac_io_file_length failed \n");
      exit(MPAC_FAILURE);
    }
  printf ("Size of file: %s is %d bytes \n", argv[1], retVal);

  if ((t1 = mpac_time_ggettime()) < 0)
    {
      printf("Failure: mpac_get_clk returned error time: %10.2f\n", t1);
      exit (MPAC_FAILURE);
    }
  printf("Start time: %10.2f \n", t1);


    if((buf = (unsigned char*) malloc(retVal)) == NULL)
      {
	printf("malloc failed for len: %d\n", retVal);
	exit(MPAC_FAILURE);
      }

  if ((buf_sz = mpac_io_file_read(argv[1], buf, retVal)) < 0)
    {
      printf("Failure: mpac_io_file_read failed\n");
      exit (MPAC_FAILURE);      
    }
  printf("Size of the read buffer: %d \n", buf_sz);
  printf("Buffer contents follow: \n%s\n", (char *)buf);

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

