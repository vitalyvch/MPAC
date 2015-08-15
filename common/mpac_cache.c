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

#include "mpac_cache.h"

size_t mpac_cache_load_stride1(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
  {      
      for (i = 0; i < 32; i++)
          sum += fw[i];
      fw += 32;
  }
    return sum;
}

size_t mpac_cache_load_stride4(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {        
        for (i = 0; i < 128; i=i+4)
            sum += fw[i];
        fw += 128;
  }
    return sum;
}

size_t mpac_cache_load_stride8(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {        
        for (i = 0; i < 256; i=i+8)
            sum += fw[i];
        fw += 256;
  }
    return sum;
}

size_t mpac_cache_load_stride16(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {        
        for (i = 0; i < 512; i=i+16)
            sum += fw[i];
        fw += 512;
  }
    return sum;
}

void mpac_cache_store_stride1(size_t* fw,size_t* lw)
{
    int i;
    while (fw <= lw)
    {
        for (i = 0; i < 32; i++)
            fw[i] = (size_t) 2;
        fw += 32;
    }
}

void mpac_cache_store_stride4(size_t* fw,size_t* lw)
{
    int i;
    while (fw <= lw)
    {
        for (i = 0; i < 128; i=i+4)
            fw[i] = (size_t) 2;
        fw += 128;
    }
}

void mpac_cache_store_stride8(size_t* fw,size_t* lw)
{
    int i;
    while (fw <= lw)
    {
        for (i = 0; i < 256; i=i+8)
            fw[i] = (size_t) 2;
        fw += 256;
    }
}

void mpac_cache_store_stride16(size_t* fw,size_t* lw)
{
    int i;
    while (fw <= lw)
    {
        for (i = 0; i < 512; i=i+16)
            fw[i] = (size_t) 2;
        fw += 512;
    }
}

size_t mpac_cache_ldst_stride1(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {
        for (i = 0; i < 32; i++)
            sum += fw[i];
        for (i = 0; i < 32; i++)
            fw[i] = (size_t) 2;
        fw += 32;
    }
    return sum;
}

size_t mpac_cache_ldst_stride4(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {
        for (i = 0; i < 128; i=i+4)
            sum += fw[i];
        for (i = 0; i < 128; i=i+4)
            fw[i] = (size_t) 2;
        fw += 128;
    }
    return sum ;
}

size_t mpac_cache_ldst_stride8(size_t* fw,size_t* lw)
{
    int i;
    register size_t sum = 0;
    while (fw <= lw)
    {
        for (i = 0; i < 256; i=i+8)
            sum += fw[i];
        for (i = 0; i < 256; i=i+8)
            fw[i] = (size_t) 2;
        fw += 256;
    }
    return sum;
}

size_t mpac_cache_ldst_stride16(size_t* fw,size_t* lw)
{
    int i;
   register size_t sum = 0;
    while (fw <= lw)
    {
        for (i = 0; i < 512; i=i+16)
            sum += fw[i];
        for (i = 0; i < 512; i=i+16)
            fw[i] = (size_t) 2;
        fw += 512;
    }
   return sum;
}

size_t result = (size_t) 0;
void mpac_cache_ctc_usesum(size_t sum)
{
	result = sum;
}

