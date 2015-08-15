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
 * mpac_disk_bm_rt.c
 *
 *  Created on: Apr 23, 2010
 *      Author: root
 */
#include "mpac.h"
#include "mpac_disk.h"
#include "mpac_time.h"

void*  mpac_disk_bm_rt(void *arg)
{

	FILE *fptr;
	char s1[1];
	char path[50];
	double gtime3, gtime4;
	struct mpac_disk_context_t *disk_context = (struct mpac_disk_context_t *) arg;
	long Len;
	Len = disk_context->requiredLength;
	char buffer[Len+1];


	        sprintf(s1, "%d", disk_context->thr_id);
	        strcpy(path,disk_context->path);

	        strcat(path,"/text");
	        strcat(path,s1);

	       if((fptr = fopen(path,"r|b"))== NULL)
	       {
		       printf("File could not be opened\n");
		       exit (MPAC_FAILURE);

	       }


	mpac_thread_manager_barrier(NULL);
	 gtime3 =  mpac_time_ggettime();
	while(!feof(fptr))
	{
		fread(buffer,sizeof(char),disk_context->requiredLength,fptr);
	}
	gtime4 =  mpac_time_ggettime();
	gtime1[disk_context->thr_id] = gtime4 - gtime3;
	printf("\nRead Time for %d thread : %lf\n",disk_context->thr_id,gtime1[disk_context->thr_id]);
	fclose(fptr);
	return MPAC_SUCCESS;


}
