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

 File:   mpac_cpu.h
 Author: M. Hasan Jamal

 Created on November 10, 2008
*/

#ifndef _MPAC_CPU_H
#define _MPAC_CPU_H

#include"mpac.h"

double *gtime, gtime1, gtime2;

/* CPU benchmark configuration */
struct mpac_cpu_config_t
{
  int    		num_thrs;	// number of threads
  unsigned long		num_reps;	// number of repetitions
  unsigned int 		aff;		// Affinity Flag
  char 			bm_uc;		// benchmark use case
};

/* CPU benchmark data context */
struct mpac_cpu_context_t
{
  unsigned long			num_reps_per_thr;
  int       			thr_id;
  int				num_wrkrs;
} ;

struct mpac_cpu_context_t** mpac_cpu_configure(struct mpac_cpu_config_t cpu_config);

/* function prototypes for the main */
void mpac_cpu_usage (char * prog_name);
void mpac_cpu_print_config (struct mpac_cpu_config_t * cpu_config);
void mpac_cpu_default_config (struct mpac_cpu_config_t * cpu_config);
int  mpac_cpu_parse_cmdl (int argc, char ** argv, struct mpac_cpu_config_t * cpu_config);
void  mpac_cpu_output (double *tp, double *gt, int index);
int mpac_cpu_arg_handler (int argc, char** argv, struct mpac_cpu_config_t * cpu_config);

void*  mpac_cpu_bm_fl(void*);
void*  mpac_cpu_bm_int(void*);
void*  mpac_cpu_bm_lo(void*);
void*  mpac_cpu_bm_sps(void*);

enum { arr_size=0x1f };
extern long arr[arr_size+1];

#endif /* _MPAC_CPU_H */
