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
 * Name:                 mpac_cpu.c
 * Author:               M. Hasan Jamal
 * Date:                 November 10, 2008
 * Description:          sample mpac cpu microbenchmark
*/

#include "mpac.h"
#include "mpac_cpu.h"

void mpac_cpu_usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  printf("-n          Number of threads \n");
  printf("-r          Number of Iterations \n");
  printf("-a          Core Affinity: [1 | 0] i.e. yes or no \n");
  printf("-u          Use case: [f | l | i] i.e. (floating Point/logical/integer Operations)\n");

}

void mpac_cpu_print_config (struct mpac_cpu_config_t *cpu_config){
  fprintf(stdout,"mpac_cpu configuration: \n");
  fprintf(stdout,"======================= \n");
  fprintf(stdout," Number of threads      : %d \n", cpu_config->num_thrs);
  fprintf(stdout," Number of iterations   : %lu \n", cpu_config->num_reps);
  fprintf(stdout," Core Affinity          : %i \n", cpu_config->aff);
  fprintf(stdout," Benchmark use case     : %c \n", cpu_config->bm_uc);
}

void mpac_cpu_default_config (struct mpac_cpu_config_t *cpu_config){
  cpu_config->num_thrs         = 1;
  cpu_config->num_reps         = 1000000;
  cpu_config->aff              = 0;
  cpu_config->bm_uc            = 'l';
  }

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */
int mpac_cpu_parse_cmdl(int argc, char **argv, struct mpac_cpu_config_t *cpu_config)
{
  int c;
  /* start with the default values */
  mpac_cpu_default_config(cpu_config);
  while ((c = getopt(argc, argv, "n:r:a:u:h")) != -1){
      switch (c){
	case 'n':
	  if(atoi(optarg) < 1 || atoi(optarg) > 65535){
	      fprintf(stderr, "mpac_cpu_parse_cmdl: number of thread: %d "
		      "is not usable \n", cpu_config->num_thrs);
	      exit(MPAC_FAILURE);
	    }
          cpu_config->num_thrs = atoi(optarg);
	  break;          
	case 'r':
	  if( atoi(optarg) < 1 || atoi(optarg) > ULONG_MAX){
	      fprintf(stderr, "mpac_cpu_parse_cmdl: number of itertions: %lu "
		      "is not usable \n", 
		      cpu_config->num_reps);
	      exit(MPAC_FAILURE);
	    }
          cpu_config->num_reps = strtoul(optarg,NULL,0);
	  break;

	case 'a':
	  if(atoi(optarg) != 0 && atoi(optarg) != 1){
	      fprintf(stderr, "mpac_cpu_parse_cmdl: Thread Affinity Flag: %i "
		      "is not usable \n", cpu_config->aff);
	      exit(MPAC_FAILURE);
	    }
          cpu_config->aff = atoi(optarg);
	  break;

	case 'u':	  
	  cpu_config->bm_uc = *optarg;
          if (cpu_config->bm_uc != 'f' && cpu_config->bm_uc != 'l' && cpu_config->bm_uc != 'i') {
                fprintf(stderr, "mpac_cpu_parse_cmdl: Use Case: %c "
		      "is not usable \n", cpu_config->bm_uc);
	      exit(MPAC_FAILURE);
                }          
          break;	  
  
	case 'h':
	  mpac_cpu_usage(argv[0]);
	  exit(0);
	case '?':
          mpac_cpu_usage(argv[0]);    
	  return MPAC_FAILURE;
	default:
          mpac_cpu_usage(argv[0]);  
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}

void mpac_cpu_output(double *tp, double *time, int index){

  int i;

  printf("mpac_cpu finished successfully \n\n");
  printf("=================\n");
  printf("     Results     \n");
  printf("=================\n");

	if (index == 1)
	{
	  printf("[log(mops),log(usec)] [sin(mops),sin(usec)] [cos(mops),cos(usec)] [tan(mops),tan(usec)] [exp(mops),exp(usec)]\n");
	  for (i=0; i<5; i++)
	    printf("[%.2lf, %lf] ",tp[i]/1000000.0, time[i] * 1000000.0);
	}

	else if (index == 2)
	{
	  printf("[summation(mops),summation(usec)] [subtraction(mops),subtraction(usec)] [product(mops),product(usec)]\n");
	  for (i=0; i<3; i++)
	    printf("[%.2lf, %lf] ",tp[i]/1000000.0, time[i] * 1000000.0);
	}	

	else if (index == 3)
	{
	  printf("[LeftShift(mops),LeftShift(usec)] [RightShift(mops),RightShift(usec)] [comparision(mops),comparision(usec)]\n");
	  for (i=0; i<3; i++)
	    printf("[%.2lf, %lf] ",tp[i]/1000000.0, time[i] * 1000000.0);
	}
    printf("\n");
}

int mpac_cpu_arg_handler(int argc, char** argv, struct mpac_cpu_config_t * cpu_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      mpac_cpu_usage(argv[0]);
      mpac_cpu_default_config(cpu_config);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{      
    if((ret = mpac_cpu_parse_cmdl(argc, argv, cpu_config))!= MPAC_SUCCESS){
	  fprintf(stderr, "mpac_cpu: failed to parse commandline args \n");
	  mpac_cpu_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
mpac_cpu_print_config((struct mpac_cpu_config_t *)cpu_config);
    return MPAC_SUCCESS;
}

struct mpac_cpu_context_t** 
mpac_cpu_configure(struct mpac_cpu_config_t cpu_config)
{
  int i;
  struct mpac_cpu_context_t** cpu_context;

  cpu_context=(struct mpac_cpu_context_t**)
    malloc(cpu_config.num_thrs*sizeof (struct mpac_cpu_context_t*));


  if (cpu_context == NULL)
    {
      fprintf(stderr,"mpac_cpu_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }
  for (i = 0; i < cpu_config.num_thrs; i++)
    {
      cpu_context[i]=(struct mpac_cpu_context_t*)
	malloc(sizeof(struct mpac_cpu_context_t));
      if (cpu_context[i] == NULL)
	{
	  fprintf(stderr,"mpac_cpu_bm: Not enough memory \n " );
	  return MPAC_NULL;                    
	}

      cpu_context[i]->num_reps_per_thr    = cpu_config.num_reps;    
      cpu_context[i]->num_wrkrs = cpu_config.num_thrs;
      cpu_context[i]->thr_id    = i;    
    }
  return cpu_context;
}
