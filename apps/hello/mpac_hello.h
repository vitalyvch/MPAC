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

 File:   mpac_hello.h
 Author: M. Hasan Jamal

 Created on October 23, 2009
*/

#ifndef _MPAC_HELLO_H
#define _MPAC_HELLO_H

#include"mpac.h"

/* Hello World configuration */
struct mpac_hello_config_t
{
  int    		num_thrs;	// number of threads
  unsigned int 		aff;		// Affinity Flag
};

/* Hello World data context */
struct mpac_hello_context_t
{
  int		thr_id;
  int		num_wrkrs;
} ;

struct mpac_hello_context_t** mpac_hello_configure(struct mpac_hello_config_t hello_config);

/* function prototypes for the main */
void mpac_hello_usage (char * prog_name);
void mpac_hello_print_config (struct mpac_hello_config_t * hello_config);
void mpac_hello_default_config (struct mpac_hello_config_t * hello_config);
int  mpac_hello_parse_cmdl (int argc, char ** argv, struct mpac_hello_config_t * hello_config);
int  mpac_hello_arg_handler (int argc, char** argv, struct mpac_hello_config_t * hello_config);

void*  mpac_hello_app_hw(void*);

#endif /* _MPAC_HELLO_H */
