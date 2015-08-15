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
 * File:   mpac_mem_mtm_gt.c
 * Author: M. Hasan Jamal
 *
 * Created on May 28, 2008, 8:03 PM
 */

#include"mpac.h"
#include"mpac_mem.h"

/**mpac_mem_mtm_gt(): Thread function for global case*/
void* mpac_mem_mtm_gt(void  *arg)
{
	register unsigned long int i, j;
	double bytes;
	struct mpac_mem_context_t *mem_ctx = (struct mpac_mem_context_t*) arg;

	if(mem_ctx->data_t == 'd')
	{
		double *A, *B;
		A  = (double*)malloc(mem_ctx->arraysize * sizeof(double));
		B  = (double*)malloc(mem_ctx->arraysize * sizeof(double));
		bytes = 2 * mem_ctx->arraysize * sizeof(double);

		//Initialization
		srandom(getpid());
		for (j=0; j<mem_ctx->arraysize; j++)
		{
			A[j] = (double) random();
			B[j] = 0.0;
		}
        
		for (j=0; j<mem_ctx->arraysize; j++)
			B[j] = A[j]; //Warm Up

		mpac_thread_manager_barrier(&gtime1);
		for (i=0; i<mem_ctx->num_reps_per_thr; i++)
		 {	              	    
       	 	for (j=0; j<mem_ctx->arraysize; j++) 
                                B[j] = A[j];
	        }
		mpac_thread_manager_barrier(&gtime2);

		mem_ctx->threadsData = (mem_ctx->num_reps_per_thr * bytes * 8.0)/(1024.0*1024.0);
		free(A);
		free(B);
		return MPAC_SUCCESS;
        }

	else if(mem_ctx->data_t == 'i')
	{
		int *A, *B;
		A  = (int*)malloc(mem_ctx->arraysize * sizeof(int));
		B  = (int*)malloc(mem_ctx->arraysize * sizeof(int));
		bytes = 2 * mem_ctx->arraysize * sizeof(int);

		//Initialization
		srandom(getpid());
		for (j=0; j<mem_ctx->arraysize; j++)
		 {
			A[j] = (int) random();
			B[j] = 0.0;
		 }

		for (j=0; j<mem_ctx->arraysize; j++)
			B[j] = A[j]; //warmup

		mpac_thread_manager_barrier(&gtime1);
		for (i=0; i<mem_ctx->num_reps_per_thr; i++)
		 {
			for (j=0; j<mem_ctx->arraysize; j++) 
                                B[j] = A[j];
		 }
		mpac_thread_manager_barrier(&gtime2);

		mem_ctx->threadsData = (mem_ctx->num_reps_per_thr * bytes * 8.0)/(1024.0*1024.0);
		free(A);
		free(B);
		return MPAC_SUCCESS;
	}

	else if(mem_ctx->data_t == 'f')
	{
		float *A, *B;
		A  = (float*)malloc(mem_ctx->arraysize * sizeof(float));
		B  = (float*)malloc(mem_ctx->arraysize * sizeof(float));
		bytes = 2 * mem_ctx->arraysize * sizeof(float);

		//Initialization
		srandom(getpid());

		for (j=0; j<mem_ctx->arraysize; j++)
		 {
			A[j] = (float) random();
			B[j] = 0.0;
		 }

		for (j=0; j<mem_ctx->arraysize; j++)
			B[j] = A[j]; //warmup
	
		mpac_thread_manager_barrier(&gtime1);
		for (i=0; i<mem_ctx->num_reps_per_thr; i++)
		 {
			for (j=0; j<mem_ctx->arraysize; j++) 
                                B[j] = A[j];
		 }
		mpac_thread_manager_barrier(&gtime2);
	
		mem_ctx->threadsData = (mem_ctx->num_reps_per_thr * bytes * 8.0)/(1024.0*1024.0);
		free(A);
		free(B);
		return MPAC_SUCCESS;
	}

	else if(mem_ctx->data_t == 'l')
	{
		long *A, *B;
		A  = (long*)malloc(mem_ctx->arraysize * sizeof(long));
		B  = (long*)malloc(mem_ctx->arraysize * sizeof(long));
		bytes = 2 * mem_ctx->arraysize * sizeof(long);

		//Initialization
		srandom(getpid());
		for (j=0; j<mem_ctx->arraysize; j++)
		{
			A[j] = (long) random();
			B[j] = 0.0;
		}

		for (j=0; j<mem_ctx->arraysize; j++)
			B[j] = A[j]; //warmup
	
		mpac_thread_manager_barrier(&gtime1);
		for (i=0; i<mem_ctx->num_reps_per_thr; i++)
		 {
			for (j=0; j<mem_ctx->arraysize; j++) 
				B[j] = A[j];
		 }
		mpac_thread_manager_barrier(&gtime2);
	
		mem_ctx->threadsData = (mem_ctx->num_reps_per_thr * bytes * 8.0)/(1024.0*1024.0);
		free(A);
		free(B);
		return MPAC_SUCCESS;
	}

	else if(mem_ctx->data_t == 'c')
	{
		char *A, *B;
		A  = (char*)malloc(mem_ctx->arraysize * sizeof(char));
		B  = (char*)malloc(mem_ctx->arraysize * sizeof(char));
		bytes = 2 * mem_ctx->arraysize * sizeof(char);

		//Initialization
		srandom(getpid());

		for (j=0; j<mem_ctx->arraysize; j++)
		{
			A[j] = (char) random();
			B[j] = 0.0;
		}

		for (j=0; j<mem_ctx->arraysize; j++)
			B[j] = A[j]; //warmup

		mpac_thread_manager_barrier(&gtime1);
		for (i=0; i<mem_ctx->num_reps_per_thr; i++)
		{
			for (j=0; j<mem_ctx->arraysize; j++) 
				B[j] = A[j];
		}
		mpac_thread_manager_barrier(&gtime2);

		mem_ctx->threadsData = (mem_ctx->num_reps_per_thr * bytes * 8.0)/(1024.0*1024.0);
		free(A);
		free(B);
		return MPAC_SUCCESS;
	}
	else
	{
		printf("Data type not supported\n");
		return  MPAC_NULL;
	}
}
