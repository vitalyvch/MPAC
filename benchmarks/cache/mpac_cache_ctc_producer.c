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
 *****************************************************************************/
#include "mpac.h"
#include "mpac_cache.h"
#include "mpac_cache_ctc.h"

#define NO_FC 1

void* mpac_cache_ctc_producer(void *arg)
{
    mpac_cache_ctc_context_t attr = *(mpac_cache_ctc_context_t*)arg;
    size_t *data_at = attr.data_at;
    mpac_cache_ctc_config_t* opts = attr.ctc_conf;
    int pID = opts->pID;
    int op = opts->pmode;
    long dataset_sz = opts->dataset_sz;
    long block_sz = opts->block_sz;
    long ubound = opts->ubound_sync;
    int xclusion = opts->exclusion;
    int cwarm = opts->cwarm;
    long itrs_w = opts->itrs_w;
    long itrs_b = opts->itrs_bm;
    long strd = opts->stride_sz;
    register size_t sum = (size_t) 0;
    long b, n;
    double st,et,td;
    int i;

    long num_blks = dataset_sz/block_sz;
    long WperB = block_sz/sizeof(size_t);
    mpac_thread_manager_core_affinity(pID);
    memset((char *) data_at, 0, dataset_sz);

    for (n=0; n < itrs_w; n++)
    {
        register size_t *fw = data_at;
        for ( b=0; b < num_blks; b++)
        {
            if (xclusion == 1 || cwarm == 1)
                while ((producer_at[attr.id][0] - consumer_at[attr.id][0]) > ubound)
                    NO_OP();

            register size_t *lw = fw + WperB - (strd * MPAC_UR_LOADS);
            if(NO_FC)
                {
                    switch (strd)
                    {
                    case 1:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 32; i++)
                                sum += fw[i];
                            fw += 32;
                        }
                        break;
                    case 4:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 128; i=i+4)
                                sum += fw[i];
                            fw += 128;
                        }
                        break;
                    case 8:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 256; i=i+8)
                                sum += fw[i];
                            fw += 256;
                        }
                        break;
                    case 16:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 512; i=i+16)
                                sum += fw[i];
                            fw += 512;
                        }
                        break;
                    }
                }
                else
                {
                    switch (strd)
                    {
                        case 1:
                            sum = mpac_cache_load_stride1(fw, lw);
                            break;
                        case 4:
                            sum = mpac_cache_load_stride4(fw, lw);
                            break;
                        case 8:
                            sum = mpac_cache_load_stride8(fw, lw);
                            break;
                        case 16:
                            sum = mpac_cache_load_stride16(fw, lw);
                            break;
                    }

                }
/*
            if (stride == 1)
                    sum = mpac_cache_load_stride1(fw, lw);
                else if (stride == 4)
                    sum = mpac_cache_load_stride4(fw, lw);
                else if (stride == 8)
                    sum = mpac_cache_load_stride8(fw, lw);
                else if (stride == 16)
                    {
                    if(NO_FC)
                    {
                        while (fw <= lw)
                        {
                            for (i = 0; i < 512; i=i+16)
                                sum += fw[i];
                            fw += 512;
                        }
                    }
                    else sum = mpac_cache_load_stride16(fw, lw);
                }
*/
                        if (xclusion == 1 || cwarm == 1)
                            producer_at[attr.id][0]++;
        }
        if (xclusion == 1 || cwarm == 1)
        {
            while (reset_at[attr.id][0] == 0) NO_OP();
            reset_at[attr.id][0] = 0;
        }
    }
    mpac_cache_ctc_usesum(sum);
    double *timeVals = attr.tspec;
    long *distances = attr.sep_cp;
    for ( n=0; n < itrs_b; n++)
    {
        register size_t *fw = data_at;
        for (b=0; b < num_blks; b++)
        {
            long index = n * num_blks + b;
            long distance = (producer_at[attr.id][0] - consumer_at[attr.id][0]);
            while ((producer_at[attr.id][0] - consumer_at[attr.id][0]) > ubound)
                NO_OP();

            st = mpac_time_ggettime();

            if (op == MPAC_RD)
            {
                register size_t *lw = fw + WperB - (strd * MPAC_UR_LOADS);
                if(NO_FC)
                {
                    switch (strd)
                    {
                    case 1:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 32; i++)
                                sum += fw[i];
                            fw += 32;
                        }
                        break;
                    case 4:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 128; i=i+4)
                                sum += fw[i];
                            fw += 128;
                        }
                        break;
                    case 8:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 256; i=i+8)
                                sum += fw[i];
                            fw += 256;
                        }
                        break;
                    case 16:
                        while (fw <= lw)
                        {
                            for (i = 0; i < 512; i=i+16)
                                sum += fw[i];
                            fw += 512;
                        }
                        break;
                    }
                }
                else
                {
                    switch (strd)
                    {
                        case 1:
                            sum = mpac_cache_load_stride1(fw, lw);
                            break;
                        case 4:
                            sum = mpac_cache_load_stride4(fw, lw);
                            break;
                        case 8:
                            sum = mpac_cache_load_stride8(fw, lw);
                            break;
                        case 16:
                            sum = mpac_cache_load_stride16(fw, lw);
                            break;
                    }

                }
            }
            else if (op == MPAC_WR)
            {
                register size_t *lw = fw + WperB - (strd * MPAC_UR_STORES);
                if(NO_FC)
                {
                    switch (strd)
                    {
                        case 1:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 32; i++)
                                    fw[i] = (size_t) 2;
                                fw += 32;
                            }
                            break;
                        case 4:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 128; i=i+4)
                                    fw[i] = (size_t) 2;
                                fw += 128;
                            }
                            break;
                        case 8:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 256; i=i+8)
                                    fw[i] = (size_t) 2;
                                fw += 256;
                            }
                            break;
                        case 16:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 512; i=i+16)
                                    fw[i] = (size_t) 2;
                                fw += 512;
                            }
                            break;
                    }
                }
                else
                {
                    switch (strd)
                    {
                        case 1:
                            mpac_cache_store_stride1(fw, lw);
                            break;
                        case 4:
                            mpac_cache_store_stride4(fw, lw);
                            break;
                        case 8:
                            mpac_cache_store_stride8(fw, lw);
                            break;
                        case 16:
                            mpac_cache_store_stride16(fw, lw);
                            break;
                    }

                }
            }
            else if (op == MPAC_RW) // If we've been asked to read followed by a write
            {
                register size_t *lw = fw + WperB - (strd * MPAC_UR_STORES);
                if(NO_FC)
                {
                    switch (strd)
                    {
                        case 1:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 32; i++)
                                    sum += fw[i];
                                for (i = 0; i < 32; i++)
                                    fw[i] = (size_t) 2;
                                fw += 32;
                            }
                            break;
                        case 4:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 128; i=i+4)
                                    sum += fw[i];
                                for (i = 0; i < 128; i=i+4)
                                    fw[i] = (size_t) 2;
                                fw += 128;
                            }
                            break;
                        case 8:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 256; i=i+8)
                                    sum += fw[i];
                                for (i = 0; i < 256; i=i+8)
                                    fw[i] = (size_t) 2;
                                fw += 256;
                            }
                            break;
                        case 16:
                            while (fw <= lw)
                            {
                                for (i = 0; i < 512; i=i+16)
                                    sum += fw[i];
                                for (i = 0; i < 512; i=i+16)
                                    fw[i] = (size_t) 2;
                                fw += 512;
                            }
                            break;
                    }
                }
                else
                {
                    switch (strd)
                    {
                        case 1:
                            sum = mpac_cache_ldst_stride1(fw, lw);
                            break;
                        case 4:
                            sum = mpac_cache_ldst_stride4(fw, lw);
                            break;
                        case 8:
                            sum = mpac_cache_ldst_stride8(fw, lw);
                            break;
                        case 16:
                            sum = mpac_cache_ldst_stride16(fw, lw);
                            break;
                    }
                }
            }
            et = mpac_time_ggettime();
            td = et - st;
            timeVals[index] = td;
            distances[index] = distance;
            producer_at[attr.id][0]++;
        }
        while (reset_at[attr.id][0] == 0)
            NO_OP();
        reset_at[attr.id][0] = 0;
    }
    mpac_cache_ctc_usesum(sum);
    pthread_exit(NULL);
}
