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
#include<math.h>
#include<stdio.h>
#include <spu_mfcio.h>         
struct my                
{
	unsigned long long rep_per_th;
	char operation;
	char function;
	char input1[15];
    char input2[15];
} __attribute__((aligned(16))); 
typedef struct my program_data;
int main(unsigned long long spe_id, unsigned long long program_data_ea,unsigned long long env)
{
	program_data pd;
	int tag_id = 0;
	double  b_float,tmp;
	register unsigned long int a_int;
  	register long  long b_int = 5;
        char d_left = 1;
	char d_right = 255;
	unsigned char c_logic;
	char x[11];
	mfc_get(&pd, program_data_ea, sizeof(pd), tag_id, 0, 0); 
	mfc_write_tag_mask(1<<tag_id);                     
	mfc_read_tag_status_any();
//	printf("string 1 is %s\n",pd.input1);
//	printf("string 2 is %s\n",pd.input2);	
	if(pd.operation == 0)
	{ 
		if(pd.function == 'i')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				b_int = b_int + a_int;
 			}
		}
		if(pd.function == 'f')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{ 
				tmp = (a_int*3.14/4)/pd.rep_per_th;	
   				b_float += log(tmp);
			}
		}
		if(pd.function == 'l')
		{
			c_logic = d_left;
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				c_logic = c_logic << 1;
				if(c_logic == 0)
					c_logic = d_left;
			}
		}	
		
	}
	else if(pd.operation == 1)
	{
		if(pd.function == 'i')
		{ 
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				b_int = b_int - a_int;
 			}
		}
		if(pd.function == 'f')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				tmp = (a_int*3.14/4)/pd.rep_per_th;	
   				b_float += sin(tmp);
			}
		}
		if(pd.function == 'l')
		{
			c_logic = d_right;
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				c_logic = c_logic >> 1;
				if(c_logic == 0)
					c_logic = d_right;
			}
		}	
	}
	else if(pd.operation == 2)
	{ 
		if(pd.function == 'i')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				b_int = b_int * a_int;
 			}
		}
		if(pd.function == 'f')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				tmp = (a_int*3.14/4)/pd.rep_per_th;	
   				b_float += cos(tmp);
			}			
		}	
		if(pd.function == 'l')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				int j;
				for(j=0; j<11;j++)
				{
					if(pd.input1[j]== pd.input2[j])
					x[j] = 1;
				}
 			}
		}
	}
	else if(pd.operation == 3)
	{
		if(pd.function == 'f')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{
				tmp = (a_int*3.14/4)/pd.rep_per_th;	
   				b_float += tan(tmp);
			}			
		}		
		
	}
	else if(pd.operation == 4)
	{
		if(pd.function == 'f')
		{
			for (a_int=0; a_int<pd.rep_per_th; a_int++)
			{					
   				b_float += exp(a_int);
			}			
		}		
		
	}

	mfc_put(&pd, program_data_ea, sizeof(program_data), tag_id, 0, 0);
	mfc_write_tag_mask(1<<tag_id);                   
	mfc_read_tag_status_any();
	return 0;
}


