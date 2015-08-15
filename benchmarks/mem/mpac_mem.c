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
  Name:                 mpac_mem.c
  Author:               M. Hasan Jamal
  Date:                 May 12, 2008
  Description:          sample mpac memory microbenchmark

*/

#include "mpac.h"
#include "mpac_mem.h"

void mpac_mem_usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  printf("-n          Number of threads \n");
  printf("-s          Size of data array \n");
  printf("-r          Number of overall repetitions \n");
  printf("-t          Type of test: [f | d | i | l | c] \n\t\ti.e. float, double, int, long, char\n");
  printf("-a          Core Affinity: [1 | 0] i.e. yes or no \n");
  printf("-u          Use case: [g | l] i.e. (global timing) or (local timing)\n");

}

void mpac_mem_print_config (struct mpac_mem_config_t * mem_config){
  fprintf(stdout,"mpac_mem configuration: \n");
  fprintf(stdout,"======================= \n");
  fprintf(stdout," Number of threads      : %d \n", mem_config->num_thrs);
  fprintf(stdout," Size of data array     : %ld \n", mem_config->data_size);
  fprintf(stdout," Number of repetitions  : %lu \n", mem_config->num_reps);
  fprintf(stdout," Type of Test           : %c \n", mem_config->data_t);
  fprintf(stdout," Core Affinity          : %i \n", mem_config->aff);
  fprintf(stdout," Benchmark use case     : %c \n", mem_config->bm_uc);
}

void mpac_mem_default_config (struct mpac_mem_config_t * mem_config){
  mem_config->num_thrs         = 1;
  mem_config->data_size        = 1000000;
  mem_config->num_reps         = 10;
  mem_config->data_t           = 'd' ;
  mem_config->aff              = 0;
  mem_config->bm_uc            = 'l';
  }

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */
int mpac_mem_parse_cmdl(int argc, char **argv, struct mpac_mem_config_t *mem_config)
{
  int c;
  /* start with the default values */
  mpac_mem_default_config(mem_config);
  while ((c = getopt(argc, argv, "n:s:r:t:a:u:h")) != -1){
      switch (c){
	case 'n':
	  if(atoi(optarg) < 1 || atoi(optarg) > 32767){
	      fprintf(stderr, "mpac_mem_parse_cmdl: number of thread: %d "
		      "is not usable \n", mem_config->num_thrs);
	      exit(MPAC_FAILURE);
	    }
          mem_config->num_thrs = atoi(optarg);
	  break;          
	case 's':
            if(atoi(optarg) < 1 || atoi(optarg) > 100000000 ){
	      fprintf(stderr, "In mpac_mem_parse_cmdl(): Size of data %lu is not usable \n", 
		      mem_config->data_size);
	      exit(MPAC_FAILURE);
	    }
            mem_config->data_size = atoi(optarg);
	  break;
	case 'r':
	  if( atoi(optarg) < 1 || atoi(optarg) > 100000000 ){
	      fprintf(stderr, "mpac_mem_parse_cmdl: number of repetitions: %lu "
		      "is not usable \n", 
		      mem_config->num_reps);
	      exit(MPAC_FAILURE);
	    }
          mem_config->num_reps = atoi(optarg);
	  break;

	case 't':
            mem_config->data_t = (char) *optarg;
          if( mem_config->data_t    != 'i'  &&
              mem_config->data_t    != 'd'  &&
              mem_config->data_t    != 'f'  &&
              mem_config->data_t    != 'c'  &&
              mem_config->data_t    != 'l'  ){
	      fprintf(stderr, "mpac_mem_parse_cmdl: Type of test: %c "
		      "is not usable \n", mem_config->data_t);
	      exit(MPAC_FAILURE);
	    }	 
	  break;

	case 'a':
	  if(atoi(optarg) != 0 && atoi(optarg) != 1){
	      fprintf(stderr, "mpac_mem_parse_cmdl: thread affinity flag: %i "
		      "is not usable \n", mem_config->aff);
	      exit(MPAC_FAILURE);
	    }
          mem_config->aff = atoi(optarg);
	  break;
	  
	case 'u':	  
	  mem_config->bm_uc = *optarg;
          if (mem_config->bm_uc != 'g' && mem_config->bm_uc != 'l') {
                fprintf(stderr, "mpac_mem_parse_cmdl: Use Case: %c "
		      "is not usable \n", mem_config->bm_uc);
	      exit(MPAC_FAILURE);
                }          
          break;	  
	case 'h':
	  mpac_mem_usage(argv[0]);
	  exit(0);
	case '?':
          mpac_mem_usage(argv[0]);    
	  return MPAC_FAILURE;
	default:
          mpac_mem_usage(argv[0]);  
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}
void mpac_mem_output(double tp, double latency, double el_time){
  printf("mpac_mem finished successfully \n\n");
  printf("=================\n");
  printf("     Results     \n");
  printf("=================\n");
  printf("Throughput(Mbps), Latency per repetition(usec), Elapsed time(usec)\n");
  printf("%.2lf, %.2lf, %.2lf\n",tp, latency,el_time);
}

int mpac_mem_arg_handler(int argc, char** argv, struct mpac_mem_config_t* mem_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      mpac_mem_usage(argv[0]);
      mpac_mem_default_config(mem_config);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{      
    if((ret = mpac_mem_parse_cmdl(argc, argv, mem_config))!= MPAC_SUCCESS){
	  fprintf(stderr, "mpac_mem: failed to parse commandline args \n");
	  mpac_mem_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
mpac_mem_print_config((struct mpac_mem_config_t *)mem_config);
    return MPAC_SUCCESS;
}

struct mpac_mem_context_t** 
mpac_mem_configure(struct mpac_mem_config_t mem_config)
{
  int i;
  struct mpac_mem_context_t** mem_context;
                
  mem_context=(struct mpac_mem_context_t**)
    malloc(mem_config.num_thrs*sizeof (struct mpac_mem_context_t*));

  if (mem_context == NULL)
    {
      fprintf(stderr,"mpac_mem_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }

  for (i = 0; i < mem_config.num_thrs; i++)
    {
      mem_context[i]=(struct mpac_mem_context_t*)
	malloc(sizeof(struct mpac_mem_context_t));
      if (mem_context[i] == NULL)
	{
	  fprintf(stderr,"mpac_mem_bm: Not enough memory \n " );
	  return MPAC_NULL;                    
	}

	    mem_context[i]->num_reps_per_thr = mem_config.num_reps;
	    mem_context[i]->thr_id = i;    
	    mem_context[i]->arraysize = mem_config.data_size;
	    mem_context[i]->data_t = mem_config.data_t;
	    mem_context[i]->num_wrkrs = mem_config.num_thrs;
	    mem_context[i]->roundtime = 
                    malloc(mem_context[i]->num_reps_per_thr *sizeof(double));
		if (mem_context[i]->roundtime == NULL)
		  {
    			fprintf(stderr,"mpac_mem: Not enough memory \n " );
    			exit(MPAC_FAILURE);                    
  		  }
	      mem_context[i]->threadsData = 0.0;
    } 
         
  return mem_context;
}

