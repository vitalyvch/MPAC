/*
  mpac_thread_manager.h

*/

#ifndef MPAC_THREAD_MANAGER_H
#define MPAC_THREAD_MANAGER_H

#ifndef _REENTRANT
#define _REENTRANT
#endif


#include "mpac.h"

typedef struct
{
  int id;
  unsigned int affinity;      /*Affinity support flag*/
  void * (*work_routine)(void *);
  /**For consistency purposes, arg must be an array of premitive types 
   * or user defined types even if the same parameters are to be passed
   * to all threads. This reduces synchronization overhead, too.*/
  void *arg; 

} mpac_thread_manager_job_t;
/**mpac_thread_manager_t construct encapsulates the thread management-related 
 * information. A pointer to an object of this type is passed to every thread
 * management function as a handle.
 */
typedef struct
{
  int num_threads;
  int num_live_threads;
  pthread_t **worker_thr;
  pthread_attr_t *w_attr ;
  mpac_thread_manager_job_t** arg;
} mpac_thread_manager_t;


/**mpac_lock_t construct encapsulates locking mechanism for collaborative work. 
 * It should be used for long-term waiting, provided that the workers are cancellable. 
 * For short-term locking, simply use mutexes/semaphores. 
 */
typedef struct
{
  pthread_mutex_t lock;
  pthread_cond_t go;
  int nworker;
  int narrived;
} mpac_lock_t;

/**mpac_thread_manager_init() initializes mpac_thread_manager_t*/
int mpac_thread_manager_init  ( mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr,
                                unsigned int aff,
				void *(*work_routine)(void *),
				void ** arg );

/**mpac_thread_manager_fork() creates the given No. of threads and 
 * let them do their work in parallel*/
int mpac_thread_manager_fork(mpac_thread_manager_t * handle);

/**mpac_thread_manager_wait() encapsulates the join operation for multiple 
 * threads*/
int mpac_thread_manager_wait (mpac_thread_manager_t *handle);

/**mpac_thread_manager_startj() is a high level function that manages
 * the lifecycle of requested No. of threads*/
int mpac_thread_manager_startj(mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr, 
                                unsigned int aff,
				void *(*work_routine)(void *),
				void ** arg);

/**mpac_thread_manager_startd() is a high level function that creates requested 
 * No. of threads but does not wait for the joining i.e detach*/
int mpac_thread_manager_startd(mpac_thread_manager_t * handle,
				int num_workers,
                                pthread_attr_t* w_attr, 
                                unsigned int aff,
				void *(*work_routine)(void *),
				void ** arg);

/**mpac_thread_manager_end() encapsulates cancel operation for multiple threads*/
int mpac_thread_manager_end(mpac_thread_manager_t * handle);

/**mpac_thread_manager_isolate() encapsulates the detach operation for multiple
 * threads and let them release their resources themselves.
 * It takes ids of the n threads to be isolated*/
int mpac_thread_manager_isolate(mpac_thread_manager_t *handle);

/**mpac_thread_manager_sendsig() encapsulates the kill operation for multiple
 * threads and sends a particular signal to all threads*/
int mpac_thread_manager_sendsig(mpac_thread_manager_t *handle,int sig);

/**mpac_lock_init() initializes mpac_lock_t*/
void mpac_thread_manager_initlock(mpac_lock_t*,int );

/**mpac_lock_destroylock() destroys mpac_lock_t*/
void mpac_thread_manager_destroylock(mpac_lock_t*);
/**mpac_lock_barrier() is  high level function. It ensures that all threads start work at the same time
 * and sets the barrier release time in time attribute of mpac_lock_t struct*/
int mpac_thread_manager_free(mpac_thread_manager_t*);
int mpac_thread_manager_barrier(double*);
int mpac_thread_manager_No_of_cores();
int mpac_thread_manager_core_affinity(int);
void* mpac_thread_manager_affinity_setter(void*);

#endif
