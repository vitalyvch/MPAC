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
 * File:   mpac_cache_ctc_bm.c
 * Author: Ghulam Mustafa
 *
 * Created on January 20, 2009, 12:38 PM
 */

#include "mpac.h"
#include "mpac_cache_ctc.h"
#include "mpac_cache.h"

void mpac_cache_ctc_cleanup();
mpac_thread_manager_t p_tm;
mpac_thread_manager_t c_tm;
mpac_cache_ctc_config_t* opts;
mpac_cache_ctc_context_t** p_cntx;
mpac_cache_ctc_context_t** c_cntx;
pthread_attr_t p_attr,c_attr;

int main(int argc, char** argv)
{
    opts = (mpac_cache_ctc_config_t*) malloc(sizeof(mpac_cache_ctc_config_t));
    mpac_cache_ctc_arg_handler(argc,argv, opts);
    if (mpac_cache_ctc_validate_CLopts(opts)) exit(MPAC_FAILURE);
    p_cntx = mpac_cache_ctc_configure(opts);
    c_cntx = mpac_cache_ctc_configure(opts);

    if (!opts->exclusion)
    {
        int i;
        for (i = 0; i < opts->pc_pairs; i++)
        {
            free(c_cntx[i]->data_at);
            c_cntx[i]->data_at = p_cntx[i]->data_at;
        }
  }
mpac_thread_manager_init  ( &p_tm,opts->pc_pairs,&p_attr,0,
                   mpac_cache_ctc_producer,(void**)p_cntx );
mpac_thread_manager_fork(&p_tm);
mpac_thread_manager_init  ( &c_tm,opts->pc_pairs,&c_attr,0,
                   mpac_cache_ctc_consumer,(void**)c_cntx );
mpac_thread_manager_fork(&c_tm);
mpac_thread_manager_wait (&p_tm);
mpac_thread_manager_wait (&c_tm);
mpac_thread_manager_free(&p_tm);
mpac_thread_manager_free(&c_tm);
mpac_cache_ctc_output(p_cntx, c_cntx, opts);
mpac_cache_ctc_cleanup();
return MPAC_SUCCESS;
}

void mpac_cache_ctc_cleanup()
{
    int i;
    for (i = 0; i < opts->pc_pairs; i++)
    {
        free(p_cntx[i]->data_at);
        if (opts->exclusion) free(c_cntx[i]->data_at);
        free(p_cntx[i]->sep_cp);
        free(c_cntx[i]->sep_cp);
        free(p_cntx[i]->tspec);
        free(c_cntx[i]->tspec);
        free(p_cntx[i]);
        free(c_cntx[i]);
        free((void*)producer_at[i]);
        free((void*)consumer_at[i]);
        free((void*)reset_at[i]);
   }
    free(p_cntx);
    free(c_cntx);
    free(opts);
    free(producer_at);
    free(consumer_at);
    free(reset_at);
}
