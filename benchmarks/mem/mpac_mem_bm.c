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
 * File:    mpac_mem_bm.c
 * Author:  Hassan Jamal
 * Created on May 25, 2008, 6:34 PM
 */

#include"mpac.h"
#include"mpac_mem.h"

struct mpac_mem_config_t 	 mem_config;
struct mpac_mem_context_t **mem_context;
mpac_thread_manager_t 	 tm;
pthread_attr_t 		 atr;

int main(int argc, char** argv)
{       
    unsigned long int i, j;
    int ret_val;
    double	totalTP = 0.0, 
		minmax = FLT_MAX,
		loop_o = 0.0,
		time_o = 0.0,
		mm_duration = 0.0,
		res = 0.0;

    mpac_mem_arg_handler(argc , argv, &mem_config);
    mem_context = mpac_mem_configure(mem_config);

    printf("Computing Overheads: Please wait ...\n");

    struct mpac_handle handle;

    ret_val = mpac_init(&handle);
    if(ret_val != MPAC_SUCCESS)
      {
        printf("Failure: mpac_init failed \n");
        exit(ret_val);
      }

    mm_duration = handle.min_measureable_duration_gtd;	//usec
    loop_o = handle.loop_overhead;   			//usec
    time_o = handle.res_gtd;				//usec
    res = handle.res_clk_gtd;			 	//usec

	double *maxtime;
	maxtime = (double*)malloc(mem_context[0]->num_reps_per_thr * sizeof(double));
  

for (i=0;i<mem_config.num_thrs;i++)
  {
    for (j=0;j<mem_context[0]->num_reps_per_thr;j++)
      mem_context[i]->roundtime[j] = 0.0;
  }
for (i=0;i < mem_context[0]->num_reps_per_thr;i++) maxtime[i] = 0.0;

/*
Global case - Actual work
*/
if(mem_config.bm_uc == 'g'){
        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               mem_config.num_thrs, 
                               &atr,
                               mem_config.aff,
                               mpac_mem_mtm_gt,                              
                               (void**)mem_context);        

    
    for (i=0; i<mem_config.num_thrs; i++)
      	totalTP +=  mem_context[i]->threadsData;

//    printf("gtime1 = %f , gtime2 = %f \n",gtime1, gtime2);

    double gtime = gtime2 - gtime1; 
    totalTP = totalTP/gtime;

    gtime = (gtime2-gtime1) * 1000000.0; 	//Elapsed global time in usec
    gtime = gtime - (loop_o + time_o); 		//Overhead removal

          if (gtime < 0) {
                fprintf(stderr, "mpac_mem_bm: Time duration very Low!"
		      "Increase Duration of Test. \n");
	      exit(MPAC_FAILURE);
                }          

          if (mm_duration >= gtime ) {
		  printf("==========================================================\n");
                  printf("  Elapsed time may not be precise! For measurement to be  \n"
		         "  precise, the elapsed time should atleast be %.2lf usec. \n", mm_duration);
		  printf("==========================================================\n");
                } 

     /* print the results */
    mpac_mem_output(totalTP,gtime/mem_config.num_reps, gtime);
  }
/*
Local case - Acutal work
*/
else if (mem_config.bm_uc == 'l'){

        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               mem_config.num_thrs, 
                               &atr,
                               mem_config.aff,
                               mpac_mem_mtm_lt,                              
                               (void**)mem_context);        

 for (i=0; i<mem_config.num_thrs; i++)
		totalTP +=  mem_context[i]->threadsData;
  
  for (j=0; j<mem_config.num_reps; j++)
    {
      for (i=0; i<mem_config.num_thrs; i++)
        {          
	    maxtime[j] = mpac_max(maxtime[j], mem_context[i]->roundtime[j]);
            
        }   
    } 

    for (j=0; j< mem_config.num_reps; j++)
      {	
        minmax = mpac_min(minmax,maxtime[j]);
      }

//    printf("gtime1 = %f , gtime2 = %f \n",gtime1, gtime2);
    double gtime = gtime2 - gtime1; 

    totalTP = totalTP/minmax;  
    minmax = minmax * 1000000.0;	// usec
    minmax =  minmax - (loop_o + time_o); // Overhead removal

          if (minmax < 0) {
                fprintf(stderr, "mpac_mem_bm: Time duration very Low!"
		      "Increase Duration of Test. \n");
	      exit(MPAC_FAILURE);
                }      

          if (mm_duration >= minmax ) {
		  printf("==========================================================\n");
                  printf("  Elapsed time may not be precise! For measurement to be  \n"
		         "  precise, the elapsed time should atleast be %.2lf usec. \n", mm_duration);
		  printf("==========================================================\n");
                }  

   /* print the results */
    mpac_mem_output(totalTP,minmax, gtime*1000000.0);
  }
 else 
   {
  fprintf(stderr,"Invalid Use Case \n " );
  exit(MPAC_FAILURE);                    
   }
  /* clean up */
  for (i = 0; i < mem_config.num_thrs; i++)
    {
      free(mem_context[i]->roundtime);
      free(mem_context[i]);
      }
free(mem_context);
free(maxtime);
return MPAC_SUCCESS;
}
