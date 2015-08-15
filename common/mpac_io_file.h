/*
  mpac_io_file.h

*/

#ifndef MPAC_IO_FILE_H
#define MPAC_IO_FILE_H

#include "mpac.h"

#define MPAC_IO_FILE_BUF_SZ 1024

int mpac_io_file_length(char * p_file_name);
int mpac_io_file_read (char * p_file_name, 
		       unsigned char * buffer, 
		       int buf_size);

int mpac_io_file_readln(int,char*,int); 

#endif
