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

 File:   mpac_sort.h
 Author: M. Hasan Jamal

 Created on October 1, 2009
*/

#ifndef _MPAC_SORT_H
#define _MPAC_SORT_H

#include"mpac.h"

/* sort Application configuration */
struct mpac_sort_config_t
{
  int    		num_thrs;	// number of threads
  unsigned long int	array_size;	// size of array
  unsigned int 		aff;		// Affinity Flag
  char 			app_uc;		// Application use case
  int			upper_limit;
  int			lower_limit;
};

/* sort Application data context */
struct mpac_sort_context_t
{
  int		*array_per_thr;
  int       	thr_id;
  int		num_wrkrs;
  int		bucket_size;
  int 		flag;
} ;

struct mpac_sort_context_t** mpac_sort_configure(struct mpac_sort_config_t sort_config);

/* function prototypes for the main */
void mpac_sort_usage (char * prog_name);
void mpac_sort_print_config (struct mpac_sort_config_t * sort_config);
void mpac_sort_default_config (struct mpac_sort_config_t * sort_config);
int  mpac_sort_parse_cmdl (int argc, char ** argv, struct mpac_sort_config_t * sort_config);
void  mpac_sort_output (double el_time);
int mpac_sort_arg_handler (int argc, char** argv, struct mpac_sort_config_t * sort_config);

void*  mpac_sort_app_bs(void*);
void   mpac_quicksort( int[], int, int);
int    mpac_partition( int[], int, int);

#endif /* _MPAC_SORT_H */
