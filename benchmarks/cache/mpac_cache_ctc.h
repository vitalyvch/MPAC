/* 
 * File:   mpac_cache_ctc.h
 * Author: Ghulam Mustafa
 *
 * Created on January 20, 2009, 12:34 PM
 */

#ifndef _MPAC_CACHE_CTC_H
#define	_MPAC_CACHE_CTC_H

#include "mpac.h"

typedef struct 
{
    int             pc_pairs;
    int             pID;
    int             cID;
    int             pmode;
    int             cmode;
    long            dataset_sz;
    long            block_sz; //in KB
    long            stride_sz;
    long            lbound_sync;
    long            ubound_sync;
    int             cwarm;
    long            itrs_w;
    long            itrs_bm;
    int             measure_ss;
    int             exclusion;
    int             verbosity;

} mpac_cache_ctc_config_t;

//producer consumer context
typedef struct 
{
     mpac_cache_ctc_config_t*       ctc_conf;
    size_t*                          data_at;
    double*                            tspec;
    long*                             sep_cp;
    int                                  id;

} mpac_cache_ctc_context_t;

//CLI parsing routine
int mpac_cache_ctc_parse_cmdl      (int, char**, mpac_cache_ctc_config_t*);
//For Default Configuration
void mpac_cache_ctc_default_config  (mpac_cache_ctc_config_t*);
//Validate CL options
int mpac_cache_ctc_validate_CLopts  (mpac_cache_ctc_config_t*);
void mpac_cache_ctc_print_config    (mpac_cache_ctc_config_t*);
//Final output
void mpac_cache_ctc_output          (mpac_cache_ctc_context_t**,mpac_cache_ctc_context_t**,mpac_cache_ctc_config_t*);
// Usage help
void mpac_cache_ctc_usage           (char*);

int mpac_cache_ctc_arg_handler      (int , char** , mpac_cache_ctc_config_t*);
mpac_cache_ctc_context_t** mpac_cache_ctc_configure(mpac_cache_ctc_config_t*);
//CLI configuration parameters

#endif	/* _MPAC_CACHE_CTC_H */




