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
 * Name:                 mpac_disk.c
 * Author:
 * Date:                 November 10, 2008
 * Description:          sample mpac disk microbenchmark
*/
#include "mpac.h"
#include "mpac_disk.h"

void mpac_disk_usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS] \n", prog_name);
  printf("\nOPTIONS: \n");
  printf("-n          Number of threads \n");
  printf("            Default value : 1 \n");
  printf("-s          Block size in bytes\n");
  printf("            Default value : 512\n");
  printf("-p          Path\n");
  printf("            Default path : /tmp/MPACIO\n");
  printf("-f          File size in bytes\n");
  printf("            Default value : 2147483648 B \n");
  printf("-a          Core Affinity: [1 | 0] i.e. yes or no \n");


}

void mpac_disk_print_config (struct mpac_disk_config_t *disk_config){
  fprintf(stdout,"mpac_disk configuration: \n");
  fprintf(stdout,"======================= \n");
  fprintf(stdout," Number of threads   : %d \n", disk_config->num_thrs);
  fprintf(stdout," Block size          : %ld \n", disk_config->block_size);
  fprintf(stdout," File size           :%lld \n", disk_config->file_size);
  fprintf(stdout," Path of File        : %s\n",disk_config->path);
  fprintf(stdout," Core Affinity       : %i \n", disk_config->aff);

}

void mpac_disk_default_config (struct mpac_disk_config_t *disk_config){
  disk_config->num_thrs         = 1;
  disk_config->block_size       = 512;// in KB
  disk_config->file_size        = 2147483648;  //2*1024*2024*1024 = 2GB
  disk_config->path             = "/tmp/MPACIO";
  disk_config->aff              = 0;


  }

/* 
 * Parses commandline args as well as initializes them as needed.
 * It first calls the default configuration function to make sure
 * that all config parameters have valid values.
 */
int mpac_disk_parse_cmdl(int argc, char **argv, struct mpac_disk_config_t *disk_config)
{
  int c;



  /* start with the default values */
  mpac_disk_default_config(disk_config);
  while ((c = getopt(argc, argv, "n:s:f:p:a:h")) != -1){
      switch (c){
	case 'n':
	  if(atoi(optarg) < 1 || atoi(optarg) > 32767){
	      fprintf(stderr, "mpac_disk_parse_cmdl: number of thread: %d "
		      "is not usable \n", disk_config->num_thrs);
	      exit(MPAC_FAILURE);
	    }
          disk_config->num_thrs = atoi(optarg);
	  break;          

	case 's':
	            if(atol(optarg) < 1 || atol(optarg) > LONG_MAX ){
		      fprintf(stderr, "In mpac_disk_parse_cmdl(): Block size %lu is not usable \n",
			      disk_config->block_size);
		      exit(MPAC_FAILURE);
		    }
	            disk_config->block_size = strtol(optarg,NULL,10);
	            break;
	case 'f':
		            if(atoll(optarg) < 1 || atoll(optarg) >= LLONG_MAX ){
			      fprintf(stderr, "In mpac_mem_parse_cmdl(): Size of data %llu is not usable \n",
				      disk_config->file_size);
			      exit(MPAC_FAILURE);
			    }
		            disk_config->file_size= strtoll(optarg,NULL,0);
		            break;
	case 'p':


		              disk_config->path = optarg;

		              break;
	case 'a':
	  if(atoi(optarg) != 0 && atoi(optarg) != 1){
	      fprintf(stderr, "mpac_cpu_parse_cmdl: Thread Affinity Flag: %i "
		      "is not usable \n", disk_config->aff);
	      exit(MPAC_FAILURE);
	    }
          disk_config->aff = atoi(optarg);
	  break;


	case 'h':
	  mpac_disk_usage(argv[0]);
	  exit(0);
	case '?':
          mpac_disk_usage(argv[0]);
	  return MPAC_FAILURE;
	default:
          mpac_disk_usage(argv[0]);
	  return MPAC_FAILURE;
	}
    }
  return MPAC_SUCCESS;
}

void mpac_disk_output(double TP,double TP1){



  printf("mpac_disk finished successfully \n\n");
  printf("=================\n");
  printf("     Results     \n");
  printf("=================\n");



  printf("Write Throughput : %lf MB/sec\n",TP);
  printf("Read Throughput  : %lf MB/sec\n",TP1);


}

int mpac_disk_arg_handler(int argc, char** argv, struct mpac_disk_config_t * disk_config){
    int ret;
    if(argc < 2){                 /*Default Case*/      
      mpac_disk_usage(argv[0]);
      mpac_disk_default_config(disk_config);
      fprintf(stdout,"Using default configuration options.....\n");

    }
  else{      
    if((ret = mpac_disk_parse_cmdl(argc, argv, disk_config))!= MPAC_SUCCESS){
	  fprintf(stderr, "mpac_cpu: failed to parse commandline args \n");
	  mpac_disk_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}

    }

   mpac_disk_print_config((struct mpac_disk_config_t *)disk_config);
    return MPAC_SUCCESS;
}

struct mpac_disk_context_t**
mpac_disk_configure(struct mpac_disk_config_t disk_config)
{
  int i;
    struct mpac_disk_context_t** disk_context;

  disk_context=(struct mpac_disk_context_t**)
    malloc(disk_config.num_thrs * sizeof(struct mpac_disk_context_t*));


  if (disk_context == NULL)
    {
      fprintf(stderr,"mpac_disk_configure(): Not enough memory \n " );
      return MPAC_NULL;                    
    }


  for (i = 0; i < disk_config.num_thrs; i++)
    {
      disk_context[i]=(struct mpac_disk_context_t*)
	malloc(sizeof(struct mpac_disk_context_t));
      if (disk_context[i] == NULL)
	{
	  fprintf(stderr,"mpac_disk_bm: Not enough memory \n " );
	  return MPAC_NULL;                    
	}


      disk_context[i]->requiredLength = disk_config.block_size;
      disk_context[i]->filesize = disk_config.file_size;
      disk_context[i]->path = disk_config.path;
      disk_context[i]->num_wrkrs = disk_config.num_thrs;
      disk_context[i]->thr_id    = i;

    }


    /*for(j=0;j<disk_config.num_thrs; j++)
    {
    	printf("\npath1 entering for loop : %s\n",path1);
    	sprintf(s1, "%d", j);
        printf("\ns1: %s\n",s1);
        strcat(path1,s1);
        printf("\npath1 : %s\n",path1);
        strcpy(path,path1);
        disk_context[j]->path = path;
        printf("disk_context[j]->path: %s\n",disk_context[j]->path);
    }*/
  return disk_context;
}




