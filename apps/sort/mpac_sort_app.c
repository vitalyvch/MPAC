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
 * File:    mpac_sort_app.c
 * Author:  M. Hasan Jamal
 * Created on October 1, 2009
 */

#ifdef DBG_MPAC_SORT
#define DBG(x) x
#else
#define DBG(x)
#endif

#include"mpac.h"
#include"mpac_sort.h"

struct mpac_sort_config_t     sort_config;
struct mpac_sort_context_t  **sort_context;
mpac_thread_manager_t tm;
pthread_attr_t atr;

int main(int argc, char** argv)
{       
  unsigned long int i, j, k;
  int *data_array,  min = INT_MAX, max = 0, diff = 0, rem = 0, b_value = 0;
  double s_time = 0, e_time = 0, t_time = 0;

  mpac_sort_arg_handler(argc , argv, &sort_config);
  sort_context = mpac_sort_configure(sort_config);

  mpac_thread_manager_init  (&tm, sort_config.num_thrs, &atr,sort_config.aff, mpac_sort_app_bs, (void**)sort_context);
  mpac_thread_manager_fork(&tm);

  data_array  = (int*)malloc(sort_config.array_size * sizeof(int));

  srandom(getpid());
  for (i = 0; i<sort_config.array_size; i++)
  {
	data_array[i] = sort_config.lower_limit + (int) random()% (sort_config.upper_limit - sort_config.lower_limit);
//        printf("%d ", data_array[i]);
  }
//  printf("\n");

  //Quick sort case - Actual work
  if(sort_config.app_uc == 'q')
  {
	s_time = mpac_time_ggettime();

	for (i = 0; i < sort_config.num_thrs; i++)
	  for (j = 0; j < sort_context[i]-> bucket_size; j++)
		sort_context[i]->array_per_thr[j] = data_array[j+(i*(sort_context[i]-> bucket_size))];

	for (i = 0; i < sort_config.num_thrs; i++)
		sort_context[i]->flag = 1;

	mpac_thread_manager_wait (&tm);    
	mpac_thread_manager_free(&tm);

	for (i = 0; i < sort_config.num_thrs; i++)
	  for (j = 0; j < sort_context[i]->bucket_size; j++)
		data_array[j+(i*(sort_context[i]-> bucket_size))] = sort_context[i]->array_per_thr[j];

//	for (i = 0; i<sort_config.array_size; i++)
//		printf("%d ", data_array[i]);
//	printf("\n");

	mpac_quicksort(data_array, 0, sort_config.array_size-1);

	e_time = mpac_time_ggettime();

//	for (i = 0; i<sort_config.array_size; i++)
//		printf("%d ", data_array[i]);
//	printf("\n");
    
	double t_time = (e_time - s_time) * 1000000;  	//Elapsed global time in sec

	mpac_sort_output(t_time);			// print the results
  }

  //Bucket sort case - Actual work
  else if (sort_config.app_uc == 'b')
  {
	s_time = mpac_time_ggettime();

	for (i = 0; i<sort_config.array_size; i++)
	{
		max = mpac_max(max, data_array[i]);
		min = mpac_min(min, data_array[i]);
	}
	diff = max - min + 1;

	if (diff % sort_config.num_thrs != 0)
	{
		rem = diff % sort_config.num_thrs;
		diff = diff - rem + sort_config.num_thrs;
	}

	b_value = diff/sort_config.num_thrs;

	for (i = 0; i < sort_config.num_thrs; i++)
		sort_context[i]->bucket_size = 0;

	for (i = 0; i < sort_config.num_thrs; i++)
	{
	  for (j = 0; j<sort_config.array_size; j++)
	  {
		if (data_array[j] >= (min + (b_value*i)) && data_array[j] < (min + (b_value*(i+1))))
		sort_context[i]->bucket_size = sort_context[i]->bucket_size + 1;
	  }
	}

	for (i = 0; i < sort_config.num_thrs; i++)
		free(sort_context[i]->array_per_thr);

	for (i = 0; i < sort_config.num_thrs; i++)
	{
		sort_context[i]-> array_per_thr = (int*)malloc(sort_context[i]-> bucket_size * sizeof(int));
		if (sort_context[i]->array_per_thr == NULL)
		{
    			fprintf(stderr,"mpac_sort: Not enough memory \n " );
    			exit(MPAC_FAILURE);                    
		}
	}

	for (i = 0; i < sort_config.num_thrs; i++)
	{
		j = 0; k = 0;
		while (j < sort_config.array_size)
		{	
			if (data_array[j] >= (min + (b_value*i)) && data_array[j] < (min + (b_value*(i+1))))
			{
				sort_context[i]->array_per_thr[k] = data_array[j];
				k++;
			}
			j++;
		}
	}

	for (i = 0; i < sort_config.num_thrs; i++)
		sort_context[i]->flag = 1;

	mpac_thread_manager_wait (&tm);    
	mpac_thread_manager_free(&tm);

	k = 0;
	for (i = 0; i < sort_config.num_thrs; i++)
	  for (j = 0; j < sort_context[i]->bucket_size; j++)
	  {
		data_array[k] = sort_context[i]->array_per_thr[j];
		k++;
	  }

	e_time = mpac_time_ggettime();

//	for (i = 0; i<sort_config.array_size; i++)
//		printf("%d ", data_array[i]);
//	printf("\n");
    
	t_time = (e_time - s_time) * 1000000;  	//Elapsed global time in sec

	mpac_sort_output(t_time);		// print the results
  }

  else 
  {
  	fprintf(stderr,"Invalid Use Case \n ");
  	exit(MPAC_FAILURE);                    
  }

  /* clean up */
  for (i = 0; i < sort_config.num_thrs; i++)
  {
	free(sort_context[i]->array_per_thr);
	free(sort_context[i]);
  }

  free(sort_context);
  free(data_array);

  return MPAC_SUCCESS;
}

void* mpac_sort_app_bs(void *arg)
{
  struct mpac_sort_context_t *sort_context = (struct mpac_sort_context_t *) arg;
//  register unsigned long int i;

  while (sort_context->flag == 0);

  mpac_thread_manager_barrier(NULL);
	mpac_quicksort(sort_context->array_per_thr, 0, sort_context->bucket_size - 1);
  mpac_thread_manager_barrier(NULL);

//  for (i = 0;i < sort_context->bucket_size; i++) 
//	printf("%d ", sort_context->array_per_thr[i]);
//  printf("\t I am thread %d\n", sort_context->thr_id);

  return MPAC_SUCCESS;
}

