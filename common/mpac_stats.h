/* 
 * File:   mpac_stat.h
 * Author: Ghulam Mustafa
 *
 * Created on April 15, 2008, 11:27 AM
 */

#ifndef _MPAC_STATS_H
#define	_MPAC_STATS_H

#include "mpac.h"

/* Mode, mean median of real Data */
double mpac_stats_mode    (double *,int);
double mpac_stats_mean    (double *,int);
double mpac_stats_median  (double *,int);
double mpac_stats_min     (double *,int);
double mpac_stats_max     (double *,int);
double mpac_stats_var     (double *,int);
double mpac_stats_std     (double *,int);
int mpac_stats_conf_intr  (double *,double *,int,double);
int mpac_stats_cmp        (const void *,const void *);

#endif	/* _MPAC_STAT_H */

