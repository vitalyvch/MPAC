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
 * File:    mpac_disk_bm.c
 * Author:
 * Created on April 21, 2010
 */

#ifdef DBG_MPAC_DISK
#define DBG(x) x
#else
#define DBG(x)
#endif

#include"mpac.h"
#include"mpac_disk.h"

struct mpac_disk_config_t     disk_config;
struct mpac_disk_context_t  **disk_context;
mpac_thread_manager_t tm;
pthread_attr_t atr;

int main(int argc, char** argv)
{

	int i,j;
    double TPWA = 0.0;
    double TPRA = 0.0;
    double timew = 0.0;
    double * TPW;
    double *TPR;
    double T1,timer=0.0;
   
    if(mkdir("/tmp/MPACIO",0777)==-1)
     	        {
     	            system("rm -rf /tmp/MPACIO/text0");
     	  	     system("rmdir /tmp/MPACIO");
     	  	      mkdir("/tmp/MPACIO",0777);
     	        }
     	        else
     	        {
     	      	    printf("\ndirectory has been made");

     	         }
  mpac_disk_arg_handler(argc , argv, &disk_config);
  char * m1 = "Happy birthday to you in April ";
  printf("\n message will be written to the file : %s", m1);
  disk_context = (struct mpac_disk_context_t **)malloc(sizeof(struct mpac_disk_context_t));
  disk_context =( struct mpac_disk_context_t **) mpac_disk_configure(disk_config);
  TPW  = (double*)malloc(20 * sizeof(double));
  TPR  = (double*)malloc(20 * sizeof(double));
  /*for(i=0;i<3;i++)
  {
	  gtime[i] = 0.0;
  }*/

   /*printf("in main disk_context->requiredLength: %s\n",disk_context[0]->requiredLength);*/





        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                               disk_config.num_thrs,
                               &atr,
                               disk_config.aff,
                               mpac_disk_bm_wt,
                               (void**)disk_context);

        mpac_thread_manager_startj(&tm,                 /*Thread creation*/
                                  disk_config.num_thrs,
                                  &atr,
                                  disk_config.aff,
                                  mpac_disk_bm_rt,
                                  (void**)disk_context);

        T1 = disk_config.file_size/(1000*1000);/*1K=1000 for standard throughput*/
        printf("\n Throughput for each Thread in MB/sec:\n");
        printf("\n Threads     Write Throughput\n");
        for(j = 0;j < disk_config.num_thrs;j++ )
        {
        	 TPW[j] = T1/gtime[j];
             printf(" %d           %5lf\n",disk_context[j]->thr_id,TPW[j]);
             TPWA += TPW[j];

        }

        printf("\n Threads     Read Throughput\n");
          for(i=0;i<disk_config.num_thrs;i++)
          {
        	  TPR[i] = T1/gtime1[i];
        	  printf(" %d           %5lf\n",disk_context[i]->thr_id,TPR[i]);
              TPRA += TPR[i];
          }
          TPRA = TPRA/disk_config.num_thrs;
          TPWA = TPWA/disk_config.num_thrs;
          /*printf("time1 elasped %lf\n",time1);
          printf(" total time elasped %lf\n",time);
          time  = time /disk_config.num_thrs; // Average time of all threads*/
          printf("file size: %lld\n",disk_config.file_size);
          /*printf("Path %s \n",disk_config.path);*/

         /* printf("time elasped %lf\n",time);*/
          /*totalTP  = (disk_config.num_thrs * T1)/time;//totalTP is in MB/sec

          time1 = time1/disk_config.num_thrs;

          TP1 =  (disk_config.num_thrs * T1)/time1;*/

            mpac_disk_output(TPWA,TPRA);


    return MPAC_SUCCESS;

}



