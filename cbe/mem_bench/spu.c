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
 *
 * File:                 spu.c
 * Author:               ahmed bilal numan
 * 
 * Created on December 10, 2010
 */
#include<stdio.h>
#include <spu_mfcio.h>         
#define MAX 16384
int main(unsigned long long spe_id, unsigned long long program_data_ea,unsigned long long env)
{
	char array[MAX] __attribute__((aligned(128)));
	int func,dma_count;
	unsigned int tag = 1,count,k,byte_size,chunk_size, transfered_size,dest_inc;
	unsigned int count1,add_inc;
	unsigned long int rep;
	char arr[MAX];
	unsigned long int array_size = 32768;
	unsigned long int data_size;
	spu_write_decrementer(0);
	rep = spu_read_in_mbox();
	data_size = spu_read_in_mbox();
	func = spu_read_in_mbox();
	byte_size = data_size;
	k = byte_size - MAX;
	chunk_size = byte_size;
	mfc_get(array, (unsigned int)program_data_ea, chunk_size, tag, 0, 0);
	mfc_write_tag_mask(1<<tag);                     
	mfc_read_tag_status_any();
	for(count = 0; count < rep;count++)
	for(count1 = 0 ; count1 < chunk_size ; count1++)
	{
		arr[count1%array_size] = array[count1];
	}
	return 0;
}
