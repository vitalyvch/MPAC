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
 * Name:                 mpac_sort.c
 * Author:               M. Hasan Jamal
 * Date:                 October 1, 2009
 * Description:          multithreaded sorting
*/

#include "mpac.h"
#include "mpac_sort.h"

void mpac_sort_usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  printf("-n          Number of threads \n");
  printf("-s          Size of Array \n");
  printf("-m          Upper Limit of Data generated\n");
  printf("-l          Lower Limit of Data generated \n");
  printf("-a          Core Affinity: [1 | 0] i.e. yes or no \n");
  printf("-u          Use case: [b|q] i.e. (bucket/quick sorting) \n");

}

void mpac_sort_print_config (struct mpac_sort_config_t *sort_config)
{
  fprintf(stdout,"mpac_sort configuration: \n");
  fprintf(stdout,"======================= \n");
  fprintf(stdout," Number of threads	: %d \n", sort_config->num_thrs);
  fprintf(stdout," Size of Array		: %lu \n", sort_config->array_size);
  fprintf(stdout," Upper Limit of Data	: %d \n", sort_config->upper_limit);
  fprintf(stdout," Lower Limit of Data	: %d \n", sort_config->lower_limit);
  fprintf(stdout," Core Affinity		: %i \n", sort_config->aff);
  fprintf(stdout," Application use case	: %c \n", sort_config->app_uc);

}

void mpac_sort_default_config (struct mpac_sort_config_t *sort_config)
{
  sort_config->num_thrs		= 1;
  sort_config->array_size	= 1000000;
  sort_config->upper_limit	= 100;
  sort_config->lower_limit	= 0;
  sort_config->aff		= 0;
  sort_config->app_uc		= 'b';

}

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */
int mpac_sort_parse_cmdl(int argc, char **argv, struct mpac_sort_config_t *sort_config)
{
  int c;
  /* start with the default values */
  mpac_sort_default_config(sort_config);
  while ((c = getopt(argc, argv, "n:s:m:l:a:u:h")) != -1){
      switch (c){

	case 'n':
	  if(atoi(optarg) < 1 || atoi(optarg) > 65535){
	      fprintf(stderr, "mpac_sort_parse_cmdl: number of thread: %d "
		      "is not usable \n", sort_config->num_thrs);
	      exit(MPAC_FAILURE);
	    }
          sort_config->num_thrs = atoi(optarg);
	  break;          

	case 's':
	  if( atoi(optarg) < 1 || atoi(optarg) > 1000000000 ){
	      fprintf(stderr, "mpac_sort_parse_cmdl: Array size: %lu "
		      "is not usable \n", 
		      sort_config->array_size);
	      exit(MPAC_FAILURE);
	    }
          sort_config->array_size = atoi(optarg);

	  if (sort_config->array_size % sort_config->num_thrs != 0){
		printf("\nError!! Array Size should be a multiple of Number of Threads\n");
	 	exit(MPAC_FAILURE);
	     }
	  break;

	case 'm':
	  if( atoi(optarg) < 1 || atoi(optarg) > 32767){
	      fprintf(stderr, "mpac_sort_parse_cmdl: Upper Limit is not usable \n");
	      exit(MPAC_FAILURE);
	    }
          sort_config->upper_limit = atoi(optarg);

	  if (sort_config->lower_limit >= sort_config->upper_limit){
		printf("\nError!! Invalid upper and lower limits of random data\n");
	 	exit(MPAC_FAILURE);
	     }
	  break;

	case 'l':
	  if( atoi(optarg) < 0 || atoi(optarg) >= sort_config->upper_limit){
	      fprintf(stderr, "mpac_sort_parse_cmdl: Lower Limit is not usable \n");
	      exit(MPAC_FAILURE);
	    }
          sort_config->lower_limit = atoi(optarg);

	  if (sort_config->lower_limit >= sort_config->upper_limit){
		printf("\nError!! Invalid upper and lower limits of random data\n");
	 	exit(MPAC_FAILURE);
	     }
	  break;

	case 'a':
	  if(atoi(optarg) != 0 && atoi(optarg) != 1){
	      fprintf(stderr, "mpac_sort_parse_cmdl: Thread Affinity Flag: %i "
		      "is not usable \n", sort_config->aff);
	      exit(MPAC_FAILURE);
	    }
          sort_config->aff = atoi(optarg);
	  break;

	case 'u':	  
	  sort_config->app_uc = *optarg;
          if (sort_config->app_uc != 'b' && sort_config->app_uc != 'q') {
                fprintf(stderr, "mpac_sort_parse_cmdl: Use Case: %c "
		      "is not usable \n", sort_config->app_uc);
	      exit(MPAC_FAILURE);
                }          
          break;	  
  
	case 'h':
	  mpac_sort_usage(argv[0]);
	  exit(0);

	case '?':
          mpac_sort_usage(argv[0]);    
	  return MPAC_FAILURE;

	default:
          mpac_sort_usage(argv[0]);  
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}

void mpac_sort_output(double el_time){
  printf("\nmpac_sort finished successfully \n");
  printf("=================\n");
  printf("     Results     \n");
  printf("=================\n");
  printf("Elapsed time(usec)\n");
  printf("%.2lf\n",el_time);

}

int mpac_sort_arg_handler(int argc, char** argv, struct mpac_sort_config_t * sort_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      mpac_sort_usage(argv[0]);
      mpac_sort_default_config(sort_config);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{      
    if((ret = mpac_sort_parse_cmdl(argc, argv, sort_config))!= MPAC_SUCCESS){
	  fprintf(stderr, "mpac_sort: failed to parse commandline args \n");
	  mpac_sort_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
mpac_sort_print_config((struct mpac_sort_config_t *)sort_config);
    return MPAC_SUCCESS;
}

struct mpac_sort_context_t** 
mpac_sort_configure(struct mpac_sort_config_t sort_config)
{
  int i;
  struct mpac_sort_context_t** sort_context;

  sort_context=(struct mpac_sort_context_t**)
    malloc(sort_config.num_thrs*sizeof (struct mpac_sort_context_t*));

  if (sort_context == NULL)
    {
      fprintf(stderr,"mpac_sort_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }

  for (i = 0; i < sort_config.num_thrs; i++)
    {
      sort_context[i]=(struct mpac_sort_context_t*)malloc(sizeof(struct mpac_sort_context_t));
  		if (sort_context[i] == NULL)
		  {
			 fprintf(stderr,"mpac_sort_app: Not enough memory \n " );
			 return MPAC_NULL;
		  }
      sort_context[i]-> num_wrkrs = sort_config.num_thrs;
      sort_context[i]-> thr_id = i;
      sort_context[i]-> flag = 0;
      sort_context[i]-> bucket_size = sort_config.array_size/sort_config.num_thrs;
      sort_context[i]-> array_per_thr = malloc(sort_context[i]-> bucket_size * sizeof(int));
		if (sort_context[i]->array_per_thr == NULL)
		  {
    			fprintf(stderr,"mpac_sort: Not enough memory \n " );
    			exit(MPAC_FAILURE);                    
  		  }
    }

  return sort_context;
}

void mpac_quicksort( int a[], int l, int r)
{
   int j;
   if( l < r ) 
   {
       j = mpac_partition( a, l, r);
       mpac_quicksort( a, l, j-1);
       mpac_quicksort( a, j+1, r);
   }
}

int mpac_partition( int a[], int l, int r)
{
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;
		
   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}

