/* 
 * File:   mpac_time.h
 * Author: Ghulam Mustafa
 *
 * Created on April 21, 2008, 4:34 PM
 */

#ifndef _MPAC_TIME_H
#define	_MPAC_TIME_H
#include"mpac.h"

/** mpac_time_wctime() calculates the CPU time (in seconds) taken by 
 * a function to execute */
double  mpac_time_wctime(void* (*)(void*),void* );   //DONE
/** mpac_time_frt_gtd() measures the running time of a function 
 * using gettimeofday() */
int mpac_time_frt_gtd(struct timeval *,void* (*)(void*),void*); 
/** mpac_time_frt_rtc() measures the running time of a function using
 * clock_gettime(CLOCK_REALTIME, - ) */
int mpac_time_frt_rtc(struct timespec *, void* (*)(void*),void*);
double mpac_time_ggettime();
double mpac_time_rgettime();
double mpac_time_loop_overhead();
//Hassan - double mpac_time_min_measurable();
double mpac_time_ggettime_usec();
double mpac_time_rgettime_nsec();
double mpac_time_overhead_rtc();
double mpac_time_overhead_gtd();
//Added by Hassan
double mpac_time_min_measurable_gtd(double, double, double);
double mpac_time_min_measurable_rtc(double, double, double);
#endif	/* _MPAC_TIME_H */

