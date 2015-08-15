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
 * Name:                 mpac_hello.c
 * Author:               M. Hasan Jamal
 * Date:                 October 23, 2009
 * Description:          Hello World Application
*/

#include "mpac.h"
#include "mpac_hello.h"

void mpac_hello_usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  printf("-n          Number of threads \n");
  printf("-a          Core Affinity: [1 | 0] i.e. yes or no \n");
}

void mpac_hello_print_config (struct mpac_hello_config_t *hello_config){
  fprintf(stdout,"mpac_hello configuration: \n");
  fprintf(stdout,"======================= \n");
  fprintf(stdout," Number of threads      : %d \n", hello_config->num_thrs);
  fprintf(stdout," Core Affinity	\t: %i \n\n", hello_config->aff);
}

void mpac_hello_default_config (struct mpac_hello_config_t *hello_config)
{
  hello_config->num_thrs         = 1;
  hello_config->aff              = 0;
}

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */
int mpac_hello_parse_cmdl(int argc, char **argv, struct mpac_hello_config_t *hello_config)
{
  int c;
  /* start with the default values */
  mpac_hello_default_config(hello_config);
  while ((c = getopt(argc, argv, "n:a:h")) != -1){
      switch (c){
	case 'n':
	  if(atoi(optarg) < 1 || atoi(optarg) > 65535){
	      fprintf(stderr, "mpac_hello_parse_cmdl: number of thread: %d "
		      "is not usable \n", hello_config->num_thrs);
	      exit(MPAC_FAILURE);
	    }
          hello_config->num_thrs = atoi(optarg);
	  break;          

	case 'a':
	  if(atoi(optarg) != 0 && atoi(optarg) != 1){
	      fprintf(stderr, "mpac_hello_parse_cmdl: Thread Affinity Flag: %i "
		      "is not usable \n", hello_config->aff);
	      exit(MPAC_FAILURE);
	    }
          hello_config->aff = atoi(optarg);
	  break;
  
	case 'h':
	  mpac_hello_usage(argv[0]);
	  exit(0);
	case '?':
          mpac_hello_usage(argv[0]);    
	  return MPAC_FAILURE;
	default:
          mpac_hello_usage(argv[0]);  
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}

int mpac_hello_arg_handler(int argc, char** argv, struct mpac_hello_config_t * hello_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      mpac_hello_usage(argv[0]);
      mpac_hello_default_config(hello_config);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{      
    if((ret = mpac_hello_parse_cmdl(argc, argv, hello_config))!= MPAC_SUCCESS){
	  fprintf(stderr, "mpac_hello: failed to parse commandline args \n");
	  mpac_hello_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
mpac_hello_print_config((struct mpac_hello_config_t *)hello_config);
    return MPAC_SUCCESS;
}

struct mpac_hello_context_t** 
mpac_hello_configure(struct mpac_hello_config_t hello_config)
{
  int i;
  struct mpac_hello_context_t** hello_context;

  hello_context=(struct mpac_hello_context_t**)
    malloc(hello_config.num_thrs*sizeof (struct mpac_hello_context_t*));
  if (hello_context == NULL)
    {
      fprintf(stderr,"mpac_hello_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }

  for (i = 0; i < hello_config.num_thrs; i++)
    {
      hello_context[i]=(struct mpac_hello_context_t*)
	malloc(sizeof(struct mpac_hello_context_t));
      if (hello_context[i] == NULL)
	{
	  fprintf(stderr,"mpac_hello_app: Not enough memory \n " );
	  return MPAC_NULL;                    
	}

      hello_context[i]->num_wrkrs = hello_config.num_thrs;
      hello_context[i]->thr_id    = i;    
    }
  return hello_context;
}
