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
 * File:   mpac_cache_ctc.h
 * Author: Ghulam Mustafa
 *
 * Created on January 20, 2009, 12:34 PM
 */
#include "mpac_cache_ctc.h"

void mpac_cache_ctc_default_config (mpac_cache_ctc_config_t* c_conf)
{
    c_conf->pc_pairs        = 1;
    c_conf->pID             = 0;
    c_conf->cID             = 1;
    c_conf->pmode           = MPAC_WR;
    c_conf->cmode           = MPAC_RD;
    c_conf->dataset_sz      = 8*mpac_M;
    c_conf->block_sz        = 32*mpac_K;
    c_conf->stride_sz       = 16;
    c_conf->lbound_sync     = 1L;
    c_conf->ubound_sync     = 1024L;
    c_conf->cwarm           = YES;
    c_conf->itrs_w          = 2L;
    c_conf->itrs_bm         = 12L;
    c_conf->measure_ss      = YES;
    c_conf->exclusion       = NO;
    c_conf->verbosity       = NO;
  }


int mpac_cache_ctc_parse_cmdl(int argc, char** argv, mpac_cache_ctc_config_t* c_conf)
{	
 mpac_cache_ctc_default_config (c_conf);
int c;
while ((c = getopt(argc, argv, "l:P:C:p:c:d:b:i:I:k:K:s:wSxvh")) != -1)
{
    switch(c)
    {
        case 'l':
            c_conf->pc_pairs = atoi(optarg);
            break;
        case 'P':
            c_conf->pID = atoi(optarg);
            break;
        case 'C':
            c_conf->cID = atoi(optarg);
            break;
        case 'p':
            if (!strcmp(optarg, "rd"))      c_conf->pmode = MPAC_RD;
            else if (!strcmp(optarg, "wr")) c_conf->pmode = MPAC_WR;
            else if (!strcmp(optarg, "rw")) c_conf->pmode = MPAC_RW;
            break;
        case 'c':
            if (!strcmp(optarg, "rd"))      c_conf->cmode = MPAC_RD;
            else if (!strcmp(optarg, "wr")) c_conf->cmode = MPAC_WR;
            else if (!strcmp(optarg, "rw")) c_conf->cmode = MPAC_RW;
            break;
        case 'd':
            c_conf->dataset_sz = atol(optarg) * mpac_K;
            break;
        case 'b':
            c_conf->block_sz = atol(optarg) * mpac_K;
            break;
        case 's':
            c_conf->stride_sz = atoi(optarg);
            break;
        case 'k':
            c_conf->lbound_sync = atoi(optarg);
            break;
        case 'K':
            c_conf->ubound_sync = atoi(optarg);
            break;
        case 'w':
            c_conf->cwarm = NO;
            break;
        case 'i':
            c_conf->itrs_w = atoi(optarg);
            break;
        case 'I':
            c_conf->itrs_bm = atoi(optarg);
            break;
        case 'S':
            c_conf->measure_ss = NO;
            break;
      case 'x': 
          c_conf->exclusion = YES;
          break;
      case 'v': 
          c_conf->verbosity = YES;
          break;
      case 'h':
          mpac_cache_ctc_usage(argv[0]);
          exit(MPAC_SUCCESS);
      case '?':
          mpac_cache_ctc_usage(argv[0]);
          return MPAC_FAILURE;;
        default :
            mpac_cache_ctc_usage(argv[0]);
            return MPAC_FAILURE;;
    }
}
return MPAC_SUCCESS;
}

int mpac_cache_ctc_validate_CLopts(mpac_cache_ctc_config_t* c_conf)
{

    if (c_conf->pc_pairs < 1)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: P/C pair Should be at least one\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->pID < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->cID < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->cwarm < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->itrs_bm < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->itrs_w < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->lbound_sync < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->ubound_sync < 0)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Should be a non-negative value\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->block_sz < 8 * mpac_K)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Blocksize must be at least 8K\n");
        return (MPAC_FAILURE);
    }
    if (c_conf->stride_sz != 1 &&
        c_conf->stride_sz != 4 &&
        c_conf->stride_sz != 8 &&
        c_conf->stride_sz != 16  )
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Invalid stride size\n");
        exit (MPAC_FAILURE);
    }
    if (c_conf->dataset_sz < c_conf->block_sz)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: Blocksize cannot be larger than Datasize\n");
        return (MPAC_FAILURE);
    }
    if (c_conf->measure_ss && c_conf->itrs_bm <= MPAC_STDY_STATE)
    {
        fprintf(stderr, "mpac_cache_ctc_validate_CLopts: To measure steady state, benchmark iterations "\
                "must be greater than %ld \n",MPAC_STDY_STATE);
        return (MPAC_FAILURE);
    }
    return MPAC_SUCCESS;
}

void mpac_cache_ctc_print_config (mpac_cache_ctc_config_t* c_conf)
{
  fprintf(stdout,"mpac_cache_ctc_bm configuration\n");
  fprintf(stdout,"=============================== \n");
  fprintf(stdout,"P/C pairs                               = %d\n",c_conf->pc_pairs);
  fprintf(stdout,"Stride size                             = %ld K\n",c_conf->stride_sz);
  fprintf(stdout,"Size of data set                        = %ld K\n",c_conf->dataset_sz/mpac_K);
  fprintf(stdout,"Size of data block                      = %ld K\n",c_conf->block_sz/mpac_K);
  //fprintf(stdout,"Producer ruuning at core (CPU) No = %ld\t",c_conf->pID);
  //fprintf(stdout,"Consumer ruuning at core (CPU) No = %ld\t",c_conf->cID);
  fprintf(stdout,"Producer's data operation               = %s\n",
          (c_conf->pmode == 0) ? "rd" : (c_conf->pmode == 1) ? "wr" : "rw");
  fprintf(stdout,"Consumer's data operation               = %s\n",
          (c_conf->cmode == 0) ? "rd" : (c_conf->cmode == 1) ? "wr" : "rw");
  fprintf(stdout,"Lower bound of synchronization interval = %ld\n",c_conf->lbound_sync);
  fprintf(stdout,"Upper bound of synchronization interval = %ld\n",c_conf->ubound_sync);
  fprintf(stdout,"Warmup consumer = %s\n",c_conf->cwarm ? "Yes" : "No");
  fprintf(stdout,"Steady State performance measurement    = %s\n",c_conf->measure_ss ? "Yes" : "No");
  fprintf(stdout,"Mutual Exclusion                        = %s\n",c_conf->exclusion ? "Yes" : "No");
  fprintf(stdout,"No. of benchmark iterations             = %ld\n",c_conf->itrs_bm);
  fprintf(stdout,"No. of warmup iterations                = %ld\n",c_conf->itrs_w);
}

void mpac_cache_ctc_output(mpac_cache_ctc_context_t** p_cntx, mpac_cache_ctc_context_t** c_cntx,
                                                                mpac_cache_ctc_config_t* c_conf)
{
  double *ctime, *ptime;
  long *cdist, *pdist;
  long fitrs_bm = 0, eitrs_bm = c_conf->itrs_bm;
  double aptime = 0.0, actime = 0.0, apdist = 0.0, acdist = 0.0;
  long n, b, num_blks, indx, eindx, pd, cd;
  double ptimeval, ctimeval,pt = 0, ct = 0, time_overhead, apbandwidth, acbandwidth;
  double agptime = 0, agpbandwidth = 0, agpdist = 0, agctime = 0, agcbandwidth = 0, agcdist = 0;

  time_overhead = mpac_time_overhead_gtd(); /* In usecs */
  num_blks = c_conf->dataset_sz/c_conf->block_sz;
  int i;
    for (i = 0; i < c_conf->pc_pairs; i++)
    {
        ctime = c_cntx[i]->tspec;
        cdist = c_cntx[i]->sep_cp;
        ptime = p_cntx[i]->tspec;
        pdist = p_cntx[i]->sep_cp;

  if (c_conf->measure_ss == 1)
  {
      fitrs_bm = MPAC_STDY_STATE;
      eitrs_bm = c_conf->itrs_bm - MPAC_STDY_STATE;
  }

  for ( n = fitrs_bm; n < c_conf->itrs_bm; n++)
  {
      pt = 0;
      ct = 0;
      for ( b=0; b < num_blks; b++)
      {
          indx = n * num_blks + b;
          eindx = (n - fitrs_bm) * num_blks + b;

          ptimeval = ptime[indx];
          ctimeval = ctime[indx];
          pt += ptimeval * 1000000L - time_overhead;
          ct += ctimeval * 1000000L - time_overhead;
          pd = pdist[indx];
          cd = cdist[indx];
          apdist += (double) pd;
          acdist += (double) cd;
      }
      aptime += (double) pt;
      actime += (double) ct;
  }
  aptime /= (eitrs_bm * num_blks);
  actime /= (eitrs_bm * num_blks);
  apdist /= (eitrs_bm * num_blks);
  acdist /= (eitrs_bm * num_blks);  
  agptime += aptime;
  agctime += actime;
  agpdist += apdist;
  agcdist += acdist;
  apbandwidth = c_conf->block_sz/aptime;
  acbandwidth = c_conf->block_sz/actime;  
  agpbandwidth += apbandwidth;
  agcbandwidth += acbandwidth;
  
  if (c_conf->verbosity == 1)
  {
      fprintf(stdout, "P/C pair#, Avg Prod time(usecs), Avg Prod bandwidth(MB/s), Avg Prod Distance, "\
              "Avg Cons time(usecs), Avg Cons bandwidth(MB/s), Avg Cons Distance\n");
      fprintf(stdout, "%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",\
              i, aptime, apbandwidth, apdist,actime, acbandwidth, acdist);
  }

    } //pc_pairs for loop
  
  fprintf(stdout, "==========================\n");
  fprintf(stdout, "        Results\n");
  fprintf(stdout, "==========================\n\n");
  fprintf(stdout, "No. of P/C Pairs, Avg. Producers time(usecs), Avg. Producers Throughput(MB/s), Avg. Producers Distance, "\
              "Avg. Consumers time(usecs), Avg. Consumers Throughput(MB/s), Avg. Consumers Distance\n");
  fprintf(stdout, "%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n\n",\
           c_conf->pc_pairs, agptime/c_conf->pc_pairs, agpbandwidth/c_conf->pc_pairs, agpdist/c_conf->pc_pairs,
           agctime/c_conf->pc_pairs, agcbandwidth/c_conf->pc_pairs, agcdist/c_conf->pc_pairs);
}

void mpac_cache_ctc_usage(char *prog)
{
    char *parameters = "\
-l          N0. of Prod/Cons pairs: [ Positive integer No.]\n\
            (Default: 0)\n\
-p          Producer's data operation: [ rd | wr | rw ]\n\
            (Default: wr)\n\
-c          Consumer's data operation: [ rd | wr | rw ]\n\
            (Default: rd)\n\
-d          Dataset size in kilobytes: \n\
            (Default: 8192)\n\
-b          Bloack size in kilobytes\n\
            (Default: 32)\n\
-s          Stride size: [ 1 | 4 | 8 | 16 ] \n\
            (Default: 16)\n\
-k          Lower bound of synchronization interval: [positive integer No.]\n\
            (Default: 1)\n\
-K          Upper bound of synchronization interval: [positive integer No.]\n\
            (Default: 1024)\n\
-w 	    Mentioned to disable warming up the consumer\n\
-i          Number of warmup iterations\n\
            (Default: 2)\n\
-I          Number of benchmarking iterations\n\
            (Default: 12)\n\
-S 	    Mentioned to disable steady state performance measurement \n\
-x          Mentioned to enable Producer/Consumer access distinct data\n\
-v 	    Verbose output\n\
-h          For help\n"\
;
	printf("Usage: %s [options]...\nOptions:\n%s\n", prog, parameters);
/*
-P          Core/CPU No. to pin producer to: [ 0 | 1 | 2 ... depending on available No. of cores]\n\
            (Default: 0)\n\
-C          Core/CPU No. to pin consumer to: [ 0 | 1 | 2 ... depending on available No. of cores]\n\
            (Default: 1)\n\
*/
}

int mpac_cache_ctc_arg_handler(int argc, char** argv, mpac_cache_ctc_config_t* c_conf)
{
    int ret;
    if(argc < 2)
    {                 /*Default Case*/
      mpac_cache_ctc_usage(argv[0]);
      mpac_cache_ctc_default_config(c_conf);
      fprintf(stdout,"Using default configuration options.....\n");
    }
  else{
    if((ret = mpac_cache_ctc_parse_cmdl(argc, argv, c_conf))!= MPAC_SUCCESS)
      {
	  fprintf(stderr, "mpac_cache_ctc_arg_handler(): failed to parse commandline args \n");
	  mpac_cache_ctc_usage(argv[0]);
	  exit (MPAC_FAILURE);
	}
    }
    mpac_cache_ctc_print_config(c_conf);
    return MPAC_SUCCESS;
}

mpac_cache_ctc_context_t** mpac_cache_ctc_configure(mpac_cache_ctc_config_t* c_conf)
{
  int i;
  mpac_cache_ctc_context_t **cntx;
  int th_prs = c_conf->pc_pairs;
  long num_blks = c_conf->dataset_sz/c_conf->block_sz;
  producer_at = (volatile long**) malloc(th_prs * sizeof (long*));
  consumer_at = (volatile long**) malloc(th_prs * sizeof (long*));
  reset_at    = (volatile long**) malloc(th_prs * sizeof (long*));
  

  cntx   = (mpac_cache_ctc_context_t**)  malloc(th_prs*sizeof (mpac_cache_ctc_context_t*));

  if (cntx == NULL )
  {
      fprintf(stderr,"mpac_cache_ctc_configure(): Not enough memory \n " );
      return MPAC_NULL;
    }
  for (i = 0; i < th_prs; i++)
    {
      cntx[i]=(mpac_cache_ctc_context_t*) malloc(sizeof(mpac_cache_ctc_context_t));

      if (cntx[i] == NULL)
	{
	  fprintf(stderr,"mpac_cache_ctc_configure(): Not enough memory \n " );
	  return MPAC_NULL;
	}
      producer_at[i] = (long*) malloc(((CACHE_WIDTH)/sizeof(long))* sizeof(long));
      consumer_at[i] = (long*) malloc(((CACHE_WIDTH)/sizeof(long))* sizeof(long));
      reset_at[i]    = (long*) malloc(((CACHE_WIDTH)/sizeof(long))* sizeof(long));
      producer_at[i][0] = -1;
      consumer_at[i][0] = -1;
      reset_at   [i][0] = 0;

      cntx[i]->ctc_conf = c_conf;
      cntx[i]->data_at  = (size_t*)       malloc(c_conf->dataset_sz);
      cntx[i]->tspec    = (double*) malloc(c_conf->itrs_bm * num_blks * sizeof(double));
      cntx[i]->sep_cp   = (long *)        malloc(c_conf->itrs_bm * num_blks * sizeof(long));
      cntx[i]->id       = i;
    }
  return cntx;
}
