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

  Name:                mpac_thr_test.c
  Author:              Abdul Waheed, Ghulam Mustafa
  Date:                April 18, 2008
  Description:         Test for thread manager                        
*/

#include "mpac.h"
#include"mpac_thread_manager.h" 
void* thread(void*);
typedef struct {
    int id;
    int ret;
} mpac_thread_manager_arg;

//pthread_mutex_t lck;
mpac_thread_manager_t tst;
mpac_thread_manager_arg** ar;

int main (int argc, char ** argv){
  //  pthread_mutex_init(&lck, NULL);
    pthread_attr_t atr;
    int wr=10;
    ar=(mpac_thread_manager_arg**) malloc(wr*sizeof(mpac_thread_manager_arg));
    int i;
    for (i = 0; i < wr; i++) {
        ar[i] =(mpac_thread_manager_arg*) malloc(sizeof (mpac_thread_manager_arg));
        ar[i]->id = i;
        ar[i]->ret = i*5;
    }
printf("Thread No.\tThread ID\tTime\n");
mpac_thread_manager_startj(&tst,wr,&atr,0,(void*(*)(void*)) thread,(void**) ar);
for (i = 0; i < wr; i++)
    printf("\n%ld\t",ar[i]->ret);

mpac_thread_manager_startd(&tst,wr,&atr,0,(void*(*)(void*)) thread,(void**) ar);
for (i = 0; i < wr; i++)
    printf("\n%ld\t",ar[i]->ret);
for (i = 0; i < wr; i++) free(ar[i]);
free(ar);
  printf ("\nNo of Cores:%i\n", mpac_thread_manager_No_of_cores());
return MPAC_SUCCESS;
}

void* thread(void * ptr)
{
    time_t t1;
    t1=time(NULL);
    int tid;
    //pthread_mutex_lock(&lck);
    mpac_thread_manager_arg *a =(mpac_thread_manager_arg*) ptr;
    tid = (int) a->id;
    //pthread_mutex_unlock(&lck);
    printf("%li\t%d\t%li\t%s\n",tid, a->ret, pthread_self(), ctime(&t1));
  a->ret = a->id*7;
  return NULL; 
}
