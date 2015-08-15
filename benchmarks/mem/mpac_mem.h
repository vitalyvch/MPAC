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
 
  Name:                mpac_mem.h
  Author:              M. Hasan Jamal
  Date:                May 12, 2008, 11:53 AM

*/

#ifndef _MPAC_MEM_H
#define _MPAC_MEM_H

#include"mpac.h"

double gtime1, gtime2;

/* Memory benchmark configuration */
struct mpac_mem_config_t
{
  int			num_thrs;	// number of threads  
  long			data_size;	// Data size to be transfered  
  unsigned long int	num_reps;	// number of repetitions
  char			data_t;	// Type of data to be transfered 
					    // e.g. c(char), d(double), f(float) etc.
  unsigned int	aff;		// Affinity flag [Required(1)/notRequired(0)]
  char			bm_uc;		// benchmark use case [g(Global) / l(Local) timing]
};         

/* Memory benchmark data context */
struct mpac_mem_context_t
{
  unsigned long int	num_reps_per_thr;
  int			thr_id;
  char			data_t;  
  long			arraysize;
  int			num_wrkrs;
  double		*roundtime;       // To record local thread time
  double		threadsData; 
};  

struct mpac_mem_context_t** mpac_mem_configure(struct mpac_mem_config_t mem_config);

/* function prototypes for the main */
void mpac_mem_usage	(char *prog_name);
void mpac_mem_print_config	(struct mpac_mem_config_t *mem_config);
void mpac_mem_default_config (struct mpac_mem_config_t *mem_config);
int  mpac_mem_parse_cmdl (int argc, char **argv, struct mpac_mem_config_t *mem_config);
void mpac_mem_output	(double,double, double);
int mpac_mem_arg_handler (int argc, char** argv, struct mpac_mem_config_t* mem_config);

void*  mpac_mem_mtm_lt(void*);
void*  mpac_mem_mtm_gt(void*);

#endif	/* _MPAC_MEM_H */

