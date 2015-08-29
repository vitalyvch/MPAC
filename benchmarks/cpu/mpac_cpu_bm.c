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
 * File:    mpac_cpu_bm.c
 * Author:  M. Hasan Jamal
 * Created on November 10, 2008
 */

#ifdef DBG_MPAC_CPU
#define DBG(x) x
#else
#define DBG(x)
#endif

#include"mpac.h"
#include"mpac_cpu.h"

struct mpac_cpu_config_t     cpu_config;
struct mpac_cpu_context_t  **cpu_context;
mpac_thread_manager_t tm;
pthread_attr_t atr;

int main(int argc, char** argv)
{       
    int j;
    double *totalTP;
 
  mpac_cpu_arg_handler(argc , argv, &cpu_config);
  cpu_context = mpac_cpu_configure(cpu_config);
  gtime  = (double*)malloc(6 * sizeof(double)); 

//floating point case - Actual work

if(cpu_config.bm_uc == 'f'){
        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               cpu_config.num_thrs, 
                               &atr,
                               cpu_config.aff,
                               mpac_cpu_bm_fl,                              
                               (void**)cpu_context);     

 totalTP  = (double*)malloc(5 * sizeof(double));


    for (j=0; j<5; j++)
      totalTP[j] = (cpu_config.num_reps * cpu_config.num_thrs)/gtime[j];

    for (j=0; j<5; j++)
      gtime[j] = gtime[j]/cpu_config.num_reps;

     /* print the results */
    mpac_cpu_output(totalTP,gtime, 1);
  }

/*
integer case - Acutal work
*/
else if (cpu_config.bm_uc == 'i'){

        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               cpu_config.num_thrs, 
                               &atr,
                               cpu_config.aff,
                               mpac_cpu_bm_int,                              
                               (void**)cpu_context);        

 totalTP  = (double*)malloc(3 * sizeof(double)); 
    
    for (j=0; j<3; j++)
      totalTP[j] = (cpu_config.num_reps * cpu_config.num_thrs)/gtime[j];

    for (j=0; j<3; j++)
      gtime[j] = gtime[j]/cpu_config.num_reps;

     /* print the results */
    mpac_cpu_output(totalTP,gtime, 2);
      
  }

/*
integer case - Acutal work
*/
else if (cpu_config.bm_uc == 'l'){

        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               cpu_config.num_thrs, 
                               &atr,
                               cpu_config.aff,
                               mpac_cpu_bm_lo,                              
                               (void**)cpu_context);        

 totalTP  = (double*)malloc(3 * sizeof(double)); 

    for (j=0; j<3; j++)
      totalTP[j] = (cpu_config.num_reps * cpu_config.num_thrs)/gtime[j];

    for (j=0; j<3; j++)
      gtime[j] = gtime[j]/cpu_config.num_reps;

     /* print the results */
    mpac_cpu_output(totalTP,gtime, 3);
      
  }

  /*
   * Sps case - Acutal work
   */
  else if (cpu_config.bm_uc == 's'){
    srandom(time(NULL));

    for (j=0; !!(j<=arr_size); ++j)
	  arr[j] = random();

    mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               cpu_config.num_thrs, 
                               &atr,
                               cpu_config.aff,
                               mpac_cpu_bm_sps,                              
                               (void**)cpu_context);        

    totalTP  = (double*)malloc(6 * sizeof(double)); 

    for (j=0; j<6; j++)
      totalTP[j] = (cpu_config.num_reps * cpu_config.num_thrs)/gtime[j];

    for (j=0; j<6; j++)
      gtime[j] = gtime[j]/cpu_config.num_reps;

    /* print the results */
    mpac_cpu_output(totalTP, gtime, 4);
  }

 else 
   {
  fprintf(stderr,"Invalid Use Case \n " );
  exit(MPAC_FAILURE);                    
   }

return MPAC_SUCCESS;
}

