/* 
 * File:   mpac_cache.h
 * Author: Ghulam Mustafa
 *
 * Created on January 19, 2009, 2:23 PM
 */

#ifndef _MPAC_CACHE_H
#define	_MPAC_CACHE_H

#include "mpac.h"

void*  mpac_cache_ctc_producer   (void*);
void*  mpac_cache_ctc_consumer   (void*);
size_t mpac_cache_load_stride1   (size_t* ,size_t*);
size_t mpac_cache_load_stride4   (size_t* ,size_t*);
size_t mpac_cache_load_stride8   (size_t* ,size_t*);
size_t mpac_cache_load_stride16  (size_t* ,size_t*);
void   mpac_cache_store_stride1  (size_t* ,size_t*);
void   mpac_cache_store_stride4  (size_t* ,size_t*);
void   mpac_cache_store_stride8  (size_t* ,size_t*);
void   mpac_cache_store_stride16 (size_t* ,size_t*);
size_t mpac_cache_ldst_stride1   (size_t* ,size_t*);
size_t mpac_cache_ldst_stride4   (size_t* ,size_t*);
size_t mpac_cache_ldst_stride8   (size_t* ,size_t*);
size_t mpac_cache_ldst_stride16  (size_t* ,size_t*);

#define MPAC_TIMESPEC struct timespec
#define MPAC_TIMEVAL struct timeval

#define CACHE_WIDTH 64
#define MPAC_UR_LOADS 32
#define MPAC_UR_STORES 32
#define MPAC_STDY_STATE 2L

volatile long** consumer_at;
volatile long** producer_at;
volatile long** reset_at;

void mpac_cache_ctc_usesum(size_t);

#endif	/* _MPAC_CACHE_H */

