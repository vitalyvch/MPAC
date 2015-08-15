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
 *
 * File:                 ppu.c
 * Author:               ahmed bilal numan
 * 
 * Created on December 10, 2010
 */
#include<string.h>
#include <stdio.h>
#include <pthread.h>
#include <libspe2.h>
#include <stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<time.h>

#define MAX 16384
typedef struct
{
	spe_context_ptr_t SPE_ID;
	int  total_threads;
	char * data;
} pthr_structure;
typedef struct
{
	int id;	
}thread_man_job_t;
typedef struct
{
	int thr_num;
	int live_thr;
	pthread_t ** worker_th;
	thread_man_job_t ** arg;	
}thread_man;
typedef struct
{
	pthread_mutex_t lock;
	pthread_cond_t go;
	int narriv;
	int nwork;
}lock_t; 
lock_t my_lock;
double my_time1,my_time2;
double time_dif = 0;
double mops;
int thr_counter;
double mpac_time_ggettime()
{ 
	struct timeval tp;
	if(gettimeofday(&tp, 0)) 
	{
		printf("mpac_time_ggettime: gettimeofday failed \n");
		exit(1);
	}  
	return ( (double) tp.tv_sec + (double) tp.tv_usec/ 1000000.0); 
}
void mpac_thread_manager_barrier(double *btime)
{
	pthread_mutex_lock(&my_lock.lock);  
	 
	if (my_lock.nwork <=  0) 
	{                       
		pthread_mutex_unlock(&my_lock.lock);
		printf("No. of threads less than or equal to zero\n");
		exit(1);
	}
	my_lock.narriv++;
	if (my_lock.nwork == my_lock.narriv)
	{
		my_lock.narriv = 0;       
		if (btime != NULL ) *btime = mpac_time_ggettime();
		pthread_cond_broadcast(&my_lock.go);
	}
	else
		pthread_cond_wait(&my_lock.go, &my_lock.lock); 
   
	pthread_mutex_unlock(&my_lock.lock);
}
void * ppu_pthread_function(void * arg)
{	
	void * ptr;	
	pthr_structure * datap = (pthr_structure *)arg;
	unsigned int entry = SPE_DEFAULT_ENTRY;
	ptr = (void *)datap-> data;
	mpac_thread_manager_barrier(&my_time1);
	spe_context_run(datap->SPE_ID, &entry, 0, ptr, NULL, NULL);
	mpac_thread_manager_barrier(&my_time2);
	pthread_exit(NULL);	
}

extern spe_program_handle_t spu_mem;
int main(int argc, char * argv[])
{
    	int count,no_thread,c;
    	unsigned int rep;
	unsigned int data_size,rand_count, chunk_size=1024,byte_size;
	unsigned int k,func='i'; 
	void * ptr;
	spe_context_ptr_t * A ;   
	thread_man tm;
	void * align_mem;  
	pthr_structure * arg;
	no_thread = 1;
	rep = 1000;
//------------------------------------------------------------------------------
//			Input Options
//------------------------------------------------------------------------------
	printf("Input option list\n");
	printf("./output -n [number of threads] -r [repitition/thread] -s \n");
	while((c = getopt(argc,argv,"n:r:s:")) != -1)
	{
		switch(c)
		{
			case 'n':
				no_thread = atoi(optarg);				
				break;
			case 'r':
				rep = atoi(optarg);
				break;
/*			case 'u':
				func = * optarg;		
				break;*/
			case 's':
				chunk_size = atoi(optarg);
				break;
			case '?':
				printf("default settings\n ");
				no_thread = 1;
				rep = 1000;
		}
		
	}	
//-------------------------------------------------------------------------------
//                        Initialization
//-------------------------------------------------------------------------------                   	
//	printf("PPU call Data_size %lu",data_size);
//	data_size = 1024;
	tm.thr_num = no_thread;
	tm.live_thr = 0;
	pthread_mutex_init(&my_lock.lock,NULL);
	pthread_cond_init(&my_lock.go,NULL);
	my_lock.narriv = 0;
	my_lock.nwork = no_thread;
	byte_size = chunk_size;
/*	if(func != 'i' && func != 'f' && func != 'l' && func != 'c')
	{
		printf("Invalid or No input for function. Default option(integer) will be used\n");
		func = 'i';
	}*/   
do
{
	if(chunk_size > MAX)
		data_size = MAX;
	else
		data_size = chunk_size;
	
	 if((tm.worker_th = (pthread_t **)malloc(no_thread*sizeof(pthread_t *))) == NULL)
	{
		printf("Malloc Error tm.worker_th\n");
		exit(1);
	}
	if((tm.arg = (thread_man_job_t **)malloc(no_thread*sizeof(thread_man_job_t*))) == NULL)
	{
		printf("Malloc Error tm.arg\n");
		exit(1);      
	}
	A = (spe_context_ptr_t *) malloc(no_thread*sizeof(spe_context_ptr_t)) ;
	arg = (pthr_structure *)malloc(tm.thr_num * sizeof(pthr_structure));  
	for ( count = 0;  count < no_thread;  count++) 
	{
		tm.worker_th[count] = (pthread_t*)malloc(sizeof (pthread_t));
		if(tm.worker_th[count] == NULL)
		{
			printf("Error in malloc tm.worker_th %d\n",count);
			exit(1);
		}
		tm.arg[count] = (thread_man_job_t*)malloc(sizeof (thread_man_job_t));
		if(tm.arg[count] == NULL)
		{
			printf("Error in malloc tm.arg %d\n",count);
			exit(1);
		}     
		tm.arg[count]->id  = count;
	}
//-------------------------------------------------------------------------------
//                          Forking
//-------------------------------------------------------------------------------
	for(count =0;count < tm.thr_num;count++)
	{
//		if(func == 'i')
//			{
/*				if (posix_memalign(&align_mem,128,data_size)) 
				{
					printf("Failed posix_memalign() of tgt_buf");
					return(-1);
				}*/
		arg[count].data = (char *)malloc(data_size);
		if(arg[count].data == NULL)
		{
			printf("Error in memory allocation\n");
			return -1;
		}
				
		for(rand_count = 0 ; rand_count < data_size; rand_count ++)
			arg[count].data[rand_count] = (char)rand();
		arg[count].total_threads = tm.thr_num;  
        	A[count] = spe_context_create (0, NULL);
		arg[count].SPE_ID = A[count];
       		spe_program_load (A[count], &spu_mem);
		ptr = (void *)&arg[count];       		     
        	if (pthread_create(tm.worker_th[count],NULL,ppu_pthread_function,ptr))
		{
			printf("mpac_thread_manager_start: failed to create thread: %d \n", count);
			return 0;
		}
        	spe_in_mbox_write(A[count], &rep,1,SPE_MBOX_ALL_BLOCKING);
        	spe_in_mbox_write(A[count], &data_size,1,SPE_MBOX_ALL_BLOCKING);
        	spe_in_mbox_write(A[count], &func,1,SPE_MBOX_ALL_BLOCKING);
        	tm.live_thr ++;
	}
//-------------------------------------------------------------------------------
//			  Thread termination
//-------------------------------------------------------------------------------
	for(count = 0 ; count < tm.thr_num; count++)
	{
		if(tm.live_thr > 0)
		{
			if(pthread_join(*tm.worker_th[count],NULL) )
			{
				printf("Error in Joining thread number %d\n",count);
				return 0;
			}
		}
		tm.live_thr --;
		spe_context_destroy(A[count]);
	}	
	time_dif += (my_time2-my_time1);	
       
//-------------------------------------------------------------------------------
//		      Freeing the Threads
//-------------------------------------------------------------------------------
	for (count = 0; count < tm.thr_num; count++)
	{
		free(tm.worker_th[count]);
		free(tm.arg[count]);      
	}
	free(tm.worker_th);
	free(tm.arg);
chunk_size-=data_size;

} while(chunk_size > 0);
//-----------------------------------------------------------------------------
//		    Output
//-----------------------------------------------------------------------------	
        printf("----------------------------------------------------------------\n");
		printf("Time of execution for data is %f MBps\n",(sizeof(int)* byte_size*rep*no_thread)/(time_dif*1024*1024));
		printf("----------------------------------------------------------------\n");	
		

        return 0;
}


