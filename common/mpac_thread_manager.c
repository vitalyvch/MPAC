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
 
  Name:           mpac_thread_manager.c
  Author:         Abdul Waheed, Ghulam Mustafa
  Date:           April 13, 2008 
  Desription:     Thread manager for fork-and-join execution of 
                  non-interacting workers.
*/

#include "mpac.h"
#include "mpac_thread_manager.h"

static mpac_lock_t brrlck;
static int num_cores;
int mpac_thread_manager_startj(mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr,
                               unsigned int af,
				void *(*work_routine)(void *),
				void ** arg)
{
    mpac_thread_manager_init  ( handle, num_workers, w_attr,af, work_routine, arg);
    mpac_thread_manager_fork(handle);
    mpac_thread_manager_wait (handle);    
    mpac_thread_manager_free(handle);
    return MPAC_SUCCESS;
}

int mpac_thread_manager_startd(mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr,
                               unsigned int af,
				void *(*work_routine)(void *),
				void ** arg )
{
  mpac_thread_manager_init(handle, num_workers, w_attr,af, work_routine, arg);
  mpac_thread_manager_fork(handle);
  mpac_thread_manager_isolate(handle);
  mpac_thread_manager_free(handle);
  return MPAC_SUCCESS;
}

int mpac_thread_manager_free(mpac_thread_manager_t* handle)
{
  int i;
  for (i = 0; i < handle->num_threads; i++)
  {
      free(handle->worker_thr[i]);
      free(handle->arg[i]);      
  }
  free(handle->worker_thr);
  free(handle->arg);
  return MPAC_SUCCESS;
}


/**
 *  mpac_thread_manager_init() takes the number of workers, worker routine, 
 *  worker attribute structure (or NULL for default attributes)
 *  and pointer to an args structure. It will copy the thread
 *  setup related information in the handle, which will be used
 *  in subsequent calls to the thread manager.
 */
int mpac_thread_manager_init  ( mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr ,
                                unsigned int aff,
				void *(*work_routine)(void *),                             
				void ** arg )
{
  if(handle == NULL)
    {
      fprintf(stderr, "mpac_thread_manager_init: null handle passed\n");
      return MPAC_FAILURE;
    }

  if(num_workers < 1){
    fprintf(stderr, "mpac_thread_manager_init: number of of thread "
	    "should be at least one; passed: %d \n", num_workers);
    return MPAC_FAILURE;
  }
  num_cores                    = mpac_thread_manager_No_of_cores();
  handle->num_threads          = num_workers;
  handle->num_live_threads     = 0;  
  handle->w_attr               = w_attr;
  if (handle->w_attr != NULL) 
    {
      pthread_attr_init(handle->w_attr);
      pthread_attr_setscope(handle->w_attr, PTHREAD_SCOPE_SYSTEM);
    }
    
  mpac_thread_manager_initlock(&brrlck,handle->num_threads);
    
  if((handle->worker_thr = 
      (pthread_t **)malloc(num_workers*sizeof(pthread_t))) == NULL)
    {
      fprintf(stderr, 
	      "mpac_thread_manager_init: malloc failed for worker_thr \n");
      return MPAC_FAILURE;
    }
  if((handle->arg = 
      (mpac_thread_manager_job_t **)malloc(num_workers*sizeof(mpac_thread_manager_job_t*))) == NULL)
    {
      fprintf(stderr, 
	      "mpac_thread_manager_init: malloc failed for worker_thr \n");
      return MPAC_FAILURE;
    }
  int  i;
  for ( i = 0;  i < num_workers;  i++) 
  {
      handle->worker_thr[i] = (pthread_t*)malloc(sizeof (pthread_t));
      handle->arg[i] = (mpac_thread_manager_job_t*)
                        malloc(sizeof (mpac_thread_manager_job_t));
      handle->arg[i]->affinity         = aff;
      handle->arg[i]->work_routine     = work_routine;
      if (arg == NULL) handle->arg[i]->arg = NULL;
      else         handle->arg[i]->arg = arg[i];
      handle->arg[i]->id               = i;
  }
  
  return MPAC_SUCCESS;
}

/**
 *  mpac_thread_manager_fork() starts the created threads.
 * It will create the requested number of threads with each 
 * thread receiving pointer to args structure.
 */
int mpac_thread_manager_fork(mpac_thread_manager_t * handle)
{
  int i;
  if (handle == NULL){
    fprintf(stderr, "mpac_thread_manager_start: null handle \n");
    return MPAC_FAILURE;
  }
  
  
      for (i=0; i<handle->num_threads; i++)
      {         
          if (pthread_create (handle->worker_thr[i], handle->w_attr,
                  (void *(*)(void*)) mpac_thread_manager_affinity_setter,
                  handle->arg[i]))
      {
          perror("mpac_thread_manager_start: thr creation error: ");
	  fprintf(stderr, "mpac_thread_manager_start: failed to "
		  "create thread: %d \n", i);
	  return MPAC_FAILURE;
      }

      handle->num_live_threads++;
      }         
  return MPAC_SUCCESS;
}

/**
 *  mpac_thread_manager_end() forces the worker threads specified by the handle
 *  to be killed and cleans up the thread manager. There should be
 *  no need to call this function except for the case when worker
 *  threads are blocked and a monitor thread could take this action.
 *  It encapsulates cancel operation for multiple threads
 */
int mpac_thread_manager_end( mpac_thread_manager_t * handle )
{
  int i;

  if (handle == NULL)
    {
      fprintf(stderr, "mpac_thread_manager_end: null handle \n");
      return MPAC_FAILURE;
    }
  for (i = 0; i < handle->num_threads; i++) 
    {
      if (handle->num_live_threads > 0) {
	if (pthread_cancel(*handle->worker_thr[i]))
	  {
	    fprintf(stderr, 
		    "mpac_thread_manager_end: Cancellation failed \n");
	    return MPAC_FAILURE;
	  }
        handle->num_live_threads--;    
      }
    }
  return MPAC_SUCCESS;
}


/**mpac_thread_manager_wait() encapsulates the join operation for multiple threads*/
int mpac_thread_manager_wait(mpac_thread_manager_t *handle)
{  
  int i;
  if (handle == NULL)
    {
      fprintf(stderr, "mpac_thread_manager_wait: null handle \n");
      return MPAC_FAILURE;
    }

  /* wait for all threads to join */
  for (i=0; i<handle->num_threads; i++)
    {
      if (handle->num_live_threads > 0) {
	if(pthread_join(*handle->worker_thr[i], NULL)) 
	  {
	    fprintf(stderr, "mpac_thread_manager_wait: Join failed \n");
	    return MPAC_FAILURE;
	  }
	handle->num_live_threads--;    
      }
    }
  return MPAC_SUCCESS;
}

/**mpac_thread_manager_isolate() encapsulates the detach operation for multiple
 * threads and let them release their resources themselves.
 * It takes ids of the n threads to be isolated*/

int mpac_thread_manager_isolate(mpac_thread_manager_t *handle)
{
  int i;
  if (handle == NULL)
    {
      fprintf(stderr, "mpac_thread_manager_isolate: null handle \n");
      return MPAC_FAILURE;
    }
  for (i = 0; i < handle->num_threads; i++) 
    {
      if (handle->num_live_threads > 0) {
	if (pthread_detach(*handle->worker_thr[i])) 
	  {
	    fprintf(stderr, "mpac_thread_manager_isolate: Detach failed \n");
	    return MPAC_FAILURE;
	  }
        handle->num_live_threads--;    
      }
    }
  return MPAC_SUCCESS;
}

/**mpac_thread_manager_sendsig() encapsulates the kill operation for multiple
 * threads and sends a particular signal to all threads*/

int mpac_thread_manager_sendsig(mpac_thread_manager_t *handle, int sig)
{
  int i;
  if (handle == NULL){
    fprintf(stderr, "mpac_thread_manager_sendsig: null handle \n");
    return MPAC_FAILURE;
  }
    
  for (i = 0; i < handle->num_threads; i++) 
    {
      if (handle->num_live_threads > 0) 
	{
	  if (pthread_kill(*handle->worker_thr[i],sig)) 
	    {
	      fprintf(stderr, "mpac_thread_manager_sendsig: Kill failed \n");
	      return MPAC_FAILURE;
            }
	  
	  handle->num_live_threads--;    
        }
    }
  return MPAC_SUCCESS;
}


/** mpac_thread_manager_initlock() should be called only once*/
void mpac_thread_manager_initlock(mpac_lock_t *ml,int nwrkr)
{
  pthread_mutex_init(&ml->lock,NULL);
  pthread_cond_init(&ml->go,NULL);
  ml->narrived = 0;
  ml->nworker = nwrkr;    
}

void mpac_thread_manager_destroylock(mpac_lock_t *ml)
{
  pthread_mutex_destroy(&ml->lock);
  pthread_cond_destroy(&ml->go);
  ml->nworker = 0;
}

int mpac_thread_manager_barrier(double *btime)
{    
  pthread_mutex_lock(&brrlck.lock);
  
  /* Ensure barrier initialization */
  if (brrlck.nworker <=  0) 
    {                       
      pthread_mutex_unlock(&brrlck.lock);
      return MPAC_FAILURE; 
    }
  brrlck.narrived++;
  if (brrlck.nworker == brrlck.narrived)
    {
      brrlck.narrived = 0;       
      if (btime != NULL ) *btime = mpac_time_ggettime();
      pthread_cond_broadcast(&brrlck.go);
    }
  else
    pthread_cond_wait(&brrlck.go, &brrlck.lock); 
   
  pthread_mutex_unlock(&brrlck.lock);
  return MPAC_SUCCESS;
}

int mpac_thread_manager_No_of_cores()
{
  char buffer[500];
  int count = 0, fd;
  fd = open("/proc/cpuinfo",O_RDONLY);
  while(mpac_io_file_readln(fd,buffer, sizeof(buffer)) != 0)
    if (strstr(buffer,"processor") != NULL && strstr(buffer,"model") == NULL)
      count++;
  close(fd);
  return count;
}

int mpac_thread_manager_core_affinity(int cpu)
{
  //unsigned long  mask = pow(2, cpu % mpac_thread_manager_No_of_cores() );
    int num_cores = mpac_thread_manager_No_of_cores();
        cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu % num_cores, &mask);
  pid_t tid;
  tid = syscall(SYS_gettid);

  if (sched_setaffinity(tid, sizeof(mask), &mask) <0)
    {
      perror("Unable to pin the thread");
      return MPAC_FAILURE;
    }                  
  return MPAC_SUCCESS;  
}

void* mpac_thread_manager_affinity_setter(void* as)
{
    mpac_thread_manager_job_t* job = (mpac_thread_manager_job_t*) as;
    if(job->affinity == 1)
        mpac_thread_manager_core_affinity(job->id);
    (*job->work_routine)(job->arg);
    return NULL;
}

