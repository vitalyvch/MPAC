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
 * File:                 mpac_disk_bm_wt.c
 * Author:
 * 
 * Created on April 23, 2010, 11:30 AM
 */

#include "mpac.h"
#include "mpac_disk.h"
#include "mpac_time.h"


/*
 * This function will create a file of size specified by user with
 * user specified block size and measure the time to create this file
 */

void*  mpac_disk_bm_wt(void *arg)
{
	struct mpac_disk_context_t *disk_context = (struct mpac_disk_context_t *) arg;
    char * m1 = "Happy birthday to you in April ";
    double gtime1,gtime2;
    int s;
    int L = strlen(disk_context->path);
    char path [L+10];
    char s1[3];
    /*clock_t start,end;
    double CPU_Time_used;*/
    long long int n = 0;
    long long int fsize = disk_context->filesize / disk_context->requiredLength;
    /*int rem = disk_context->filesize % disk_context->requiredLength;*/
    FILE *cfptr;
	int length = strlen(m1);
    const long  requiredLength = disk_context->requiredLength;
	int copies = disk_context->requiredLength/length;
	long remainder = (disk_context->requiredLength) % length;
    char array[requiredLength];
	int i,l;
	int j = 0;
	long long int k;
   	for(i = 0; i < copies; i++){
		strncpy(&array[i*length], m1, length);

	}

	for(j = 0; j < remainder; j++){
		array[j+(length*copies)] = '*';

	}

    s = strlen(array);
    /*printf("\n\n fsize : %lld and block size : %ld\n",fsize,requiredLength);

    printf("disk_context->path: %s\n",disk_context->path);*/

    	    strcpy(path,disk_context->path);
    	     strcat(path,"/text");
    	     sprintf(s1, "%d", disk_context->thr_id);
    	     strcat(path,s1);
    	     printf("\npath(with file name) : %s\n",path);
    	     cfptr = fopen(path,"w|b");




	 mpac_thread_manager_barrier(NULL);
      printf("Now working of %d thread...\n",disk_context->thr_id);

	    gtime1 =  mpac_time_ggettime();// current time in sec

    	for(k=0;k<(fsize);k++)
    	{


    		if( fwrite(array,sizeof(char),requiredLength,cfptr) == requiredLength)
    		{
    			 n = n+ requiredLength;
    		}

    	}



         if(disk_context->filesize-n > 0)
         printf("Not enough space\n"); 
        /*for(l=0;l< disk_context->filesize-n;l++)
        {
        	n = n+ fwrite(array,sizeof(char),1,cfptr);

        }*/
        /*printf("\nValue of n : %lld\n",n);*/
    	gtime2 =  mpac_time_ggettime();//  time in sec

        gtime[disk_context->thr_id] = gtime2 - gtime1;

        printf("\nWrite time for %d thread : %lf\n",disk_context->thr_id,gtime[disk_context->thr_id]);
        fclose(cfptr);


 }


