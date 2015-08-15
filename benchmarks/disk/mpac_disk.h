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
 Author:

 Created on November 10, 2008
*/

#ifndef _MPAC_DISK_H
#define _MPAC_DISK_H
#define _FILE_OFFSET_BITS  64

#include"mpac.h"
double gtime[30];
double gtime1[30];

/* disk benchmark configuration */
struct mpac_disk_config_t
{
  int    		num_thrs;	// number of threads
  /*unsigned long		num_reps;	// number of repetitions*/
  long     block_size;  //data size to be transfered
  long long int     file_size ;   // size of the file
  char *path;           //Path where file is to be created
  unsigned int 		aff;		// Affinity Flag

};

/* IO benchmark data context */
struct mpac_disk_context_t
{

  long  requiredLength;
  long long int filesize;
  char *path;
  int       			thr_id;
  int				num_wrkrs;
} ;

struct mpac_disk_context_t** mpac_cpu_configure(struct mpac_disk_config_t disk_config);

/* function prototypes for the main */
void mpac_disk_usage (char * prog_name);
void mpac_disk_print_config (struct mpac_disk_config_t * disk_config);
void mpac_disk_default_config (struct mpac_disk_config_t * disk_config);
int  mpac_disk_parse_cmdl (int argc, char ** argv, struct mpac_disk_config_t * disk_config);
void  mpac_disk_output (double TP,double TP1);
int mpac_disk_arg_handler (int argc, char** argv, struct mpac_disk_config_t * disk_config);

void*  mpac_disk_bm_wt(void*);
void*  mpac_disk_bm_rt(void*);


#endif /* _MPAC_DISK_H */
