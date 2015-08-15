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
 * File:    mpac_cpu_app.c
 * Author:  M. Hasan Jamal
 * Created on October 23, 2009
 */

#ifdef DBG_MPAC_HELLO
#define DBG(x) x
#else
#define DBG(x)
#endif

#include"mpac.h"
#include"mpac_hello.h"

struct mpac_hello_config_t     hello_config;
struct mpac_hello_context_t  **hello_context;
mpac_thread_manager_t tm;
pthread_attr_t atr;

int main(int argc, char** argv)
{       
//	int i, j;
 
	mpac_hello_arg_handler(argc , argv, &hello_config);
	hello_context = mpac_hello_configure(hello_config);

        mpac_thread_manager_startj(&tm,                         /*Thread creation*/
                                   hello_config.num_thrs, 
                                   &atr,
                                   hello_config.aff,
                                   mpac_hello_app_hw,                              
                                   (void**)hello_context);

return MPAC_SUCCESS;
}

