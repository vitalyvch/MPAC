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
struct my
{
	unsigned long long rep_per_th;
	char operation;
	char function;
	char input1[15];
	char input2[15];
} __attribute__((aligned(16))) ;
typedef struct my program_data;
typedef struct
{
	spe_context_ptr_t SPE_ID;
	program_data * pd1;
	int  total_threads;
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
double time_dif[5];
double mops;
char in1[11];
char in2[11];
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
	ptr = (void *)datap-> pd1;
	mpac_thread_manager_barrier(&my_time1);
	spe_context_run(datap->SPE_ID, &entry, 0, ptr, NULL, NULL);
	mpac_thread_manager_barrier(&my_time2);
	pthread_exit(NULL);	
}

extern spe_program_handle_t spu_cpu;
int main(int argc, char * argv[])
{
	int count,count1=0,no_thread,rep,c,opt;
	char func; 
	void * ptr; 
	spe_context_ptr_t * A ;   
	thread_man tm;
	void * align_mem;
	program_data * pd;  
	pthr_structure * arg;
	no_thread = 1;
	rep = 1000;
//	printf("Hello\n");
	if(argc < 3)
		printf("Default values are used\nn=1 r=1000 u=i\n");
	else if(argc < 5 )
		printf("Default value for r and i are used\n r=1000 u=i\n");	
//------------------------------------------------------------------------------
//			Input Options
//------------------------------------------------------------------------------
	printf("Input option list\n");
	printf("./output -n [number of threads] -r [repitition/thread] -u[f,i,l]\n");
	while((c = getopt(argc,argv,"n:r:u:")) != -1)
	{
		switch(c)
		{
			case 'n':
				no_thread = atoi(optarg);				
				break;
			case 'r':
				rep = atoi(optarg);
				break;
			case 'u':
				func = * optarg;		
				break;
			case '?':
				printf("default settings\n ");
				no_thread = 1;
				rep = 1000;				
		}
		
	}	
//	printf("Hello");
	if(func == 'l')
	{
		printf("Enter first string for comparison\n");
		scanf("%10c",in1);
		printf("Enter second string for comparison\n");
		scanf("%10c",in2);
                
	}
//-------------------------------------------------------------------------------
//                        Initialization
//-------------------------------------------------------------------------------                   	
	tm.thr_num = no_thread;
	tm.live_thr = 0;
	pthread_mutex_init(&my_lock.lock,NULL);
	pthread_cond_init(&my_lock.go,NULL);
	my_lock.narriv = 0;
	my_lock.nwork = no_thread;
	if(func != 'i' && func != 'f' && func != 'l')
	{
		printf("Invalid or No input for function. Default option(integer) will be used\n");
		func = 'i';
	}   
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
	if (posix_memalign(&align_mem, 16,(sizeof(program_data)*tm.thr_num ))) 
	{
		printf("Failed posix_memalign() of tgt_buf");
		return(-1);
	}
	pd = (program_data *)align_mem;
	
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
		pd[count].rep_per_th = rep;
		pd[count].function = func;
		strcpy(pd[count].input1,in1);
		strcpy(pd[count].input2,in2);
	}
	if(func == 'f')
		thr_counter = 5;
	else
		thr_counter = 3;	
//-------------------------------------------------------------------------------
//                          Forking
//-------------------------------------------------------------------------------
for(count1=0;count1<thr_counter;count1 ++){	
	opt = count1;
	for(count =0;count < tm.thr_num;count++)
	{		
		pd[count].operation = opt;
		arg[count].pd1 = &pd[count] ;	
		arg[count].total_threads = tm.thr_num;  
        	A[count] = spe_context_create (0, NULL);
		arg[count].SPE_ID = A[count];
       		spe_program_load (A[count], &spu_cpu);
		ptr = (void *)&arg[count];       		     
                if (pthread_create(tm.worker_th[count],NULL,ppu_pthread_function,ptr))
		{
			printf("mpac_thread_manager_start: failed to create thread: %d \n", count);
			return 0;
		}
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
	time_dif[count1] = my_time2-my_time1;	
}       
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
//-----------------------------------------------------------------------------
//		    Output
//-----------------------------------------------------------------------------			        	 
	if(func == 'i')
	{
        	printf("----------------------------------------------------------------\n");
		printf("Time of execution for addition is %f\n",(1000000*time_dif[0]/rep));
		mops = (no_thread * rep)/(time_dif[0]*1000000);
		printf("MOPS for addition are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for subtraction is %f\n",(1000000*time_dif[1]/rep));
		mops = (no_thread * rep)/(time_dif[1]*1000000);
		printf("MOPS for subtraction are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for nmultiplication is %f\n",(1000000*time_dif[2]/rep));
		mops = (no_thread * rep)/(time_dif[2]*1000000);
		printf("MOPS for multiplication are %f\n",mops);
		printf("----------------------------------------------------------------\n");
	}
	if(func == 'f')
	{
        	printf("----------------------------------------------------------------\n");
		printf("Time of execution for Natural Log is %f\n",(1000000*time_dif[0]/rep));
		mops = (no_thread * rep)/(time_dif[0]*1000000);
		printf("MOPS for Natural Log are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for sine is %f\n",(1000000*time_dif[1]/rep));
		mops = (no_thread * rep)/(time_dif[1]*1000000);
		printf("MOPS for sine are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for cosine is %f\n",(1000000*time_dif[2]/rep));
		mops = (no_thread * rep)/(time_dif[2]*1000000);
		printf("MOPS for cosine are %f\n",mops);
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for tangent is %f\n",(1000000*time_dif[3]/rep));
		mops = (no_thread * rep)/(time_dif[3]*1000000);
		printf("MOPS for tangent are %f\n",mops);
		printf("----------------------------------------------------------------\n");
		printf("Time of execution for exponent is %f\n",(1000000*time_dif[4]/rep));
		mops = (no_thread * rep)/(time_dif[4]*1000000);
		printf("MOPS for exponent are %f\n",mops);
		printf("----------------------------------------------------------------\n");
	
	}
	if(func == 'l')
	{
        	printf("----------------------------------------------------------------\n");
		printf("Time of execution for Left shift is %f\n",(1000000*time_dif[0]/rep));
		mops = (no_thread * rep)/(time_dif[0]*1000000);
		printf("MOPS for Left shift are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for Right shift is %f\n",(1000000*time_dif[1]/rep));
		mops = (no_thread * rep)/(time_dif[1]*1000000);
		printf("MOPS for Right shift are %f\n",mops);	
		printf("----------------------------------------------------------------\n");	
		printf("Time of execution for Comparison is %f\n",(1000000*time_dif[2]/rep));
		mops = (no_thread * rep)/(time_dif[2]*1000000);
		printf("MOPS for Comparison are %f\n",mops);
		printf("----------------------------------------------------------------\n");
	}

        return 0;
}


