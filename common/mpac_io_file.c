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
  Name:           mpac_io_file.c
  Author:         Abdul Waheed, Ghulam Mustafa
  Date:           April 13, 2008
  Desription:     File I/O related services

*/

#include "mpac.h"

int mpac_io_file_readln(int fd, char *buffer, int buffsize) 
{
   int ret,nread = 0;
   
   while (nread < buffsize - 1) 
     {
      ret = read(fd, buffer + nread, 1);
      if ((ret == -1) && (errno == EINTR))  continue;
      if ((ret == 0) && (nread == 0))       return MPAC_SUCCESS;
      if (ret == 0)                         break;
      if (ret == -1)                        return MPAC_FAILURE;
      nread++;
      if (buffer[nread-1] == '\n') 
        {
         buffer[nread] = '\0';
         return nread;
      }  
   }   
   errno = EINVAL;
   return -1;
}

/**
 *  This function returns the size of a file in bytes.
 */
int mpac_io_file_length(char * p_file_name)
{
  struct stat info;
  int ret=0;
  
  if(p_file_name == NULL)
    {
      fprintf(stderr, "mpac_io_file_length: null file name \n");
      return MPAC_FAILURE;
    }

  ret = stat(p_file_name, &info);
  if(ret != 0)
    {
      fprintf(stderr, 
	      "mpac_io_file_length: stat failed for file: %s\n",
	      p_file_name);
      return MPAC_FAILURE;
    }
  
  return (info.st_size);
}

/**
 *  This function takes a file name, a pointer to a byte buffer, and
 *  length of the file in bytes as input arguments. It then opens
 *  and reads the entire file, such that the number of bytes read
 *  and copied in the buffer are equal to the passed lenght. 
 *  Function returns the actual number of bytes read in the buffer,
 *  which caller can use later on.
 */
int mpac_io_file_read(char * p_file_name, 
		      unsigned char * buffer, 
		      int len)
{
  int i, bytes;
  int fd;
  unsigned char * buffer_pos;

  if (p_file_name == NULL) 
    {
      fprintf(stderr, "mpac_io_file_read: null file name \n");
      return MPAC_FAILURE; 
    }
  if(buffer == NULL)
    {
      fprintf(stderr, "mpac_io_file_read: null buffer passed as arg \n");
      return MPAC_FAILURE;
    }
  if(len <= 0)
    {
      fprintf(stderr, 
	      "mpac_io_file_read: bad buffer len: %d\n", len);
      return MPAC_FAILURE;
    }

    if ((fd = open(p_file_name, O_RDONLY)) < 0)
      {
	fprintf(stderr, 
		"mpac_io_file_read: open failed for file: %s \n",
		p_file_name);
	return MPAC_FAILURE;
      }
    
    buffer_pos = buffer;
    i = 0;
    do{
        bytes = read(fd, buffer_pos, MPAC_IO_FILE_BUF_SZ);
        if (bytes <= 0)  break;
        buffer_pos += bytes;
        i += bytes;
      } while (i <= len);
    close(fd);

    return i;
}
