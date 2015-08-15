/* 
 * File:   mpac_resolution.h
 * Author: Ghulam Mustafa
 *
 * Created on April 21, 2008, 4:35 PM
 */

#ifndef _MPAC_RESOLUTION_H
#define	_MPAC_RESOLUTION_H

/// mpac_resval Structure holds the data related to resolution
struct mpac_resval
{
  double adiff;         ///Average Difference 
  int itr;             ///No. of iterations required
}; 
 
/** mpac_resolution_gtd() tests the resolution of gettimeofday().
Sets the 'adiff' in microseconds and  'it' to the No. of calls
required to get 'nd' differences.  Returns 0 if successful and -1 if failed.*/

int mpac_resolution_gtd(struct mpac_resval*,int);

/**mpac_resolution_rtc() tests the resolution of realtime clock
 * i.e. clock_gettime(CLOCK_REALTIME, - ).
 * Sets the 'adiff' in nanoseconds and  'it' to the No.
 * of calls made to get 'nd' differences. 
 * Returns 0 if successful and -1 if failed.*/

int mpac_resolution_rtc(struct mpac_resval*, int);

/** mpac_resolution_rtc_com() displays the systemwide realtime clock resolution */

double mpac_resolution_swrtc(void);
//Hassan-double mpac_resolution_clock_res();
//Added by Hassan
double mpac_resolution_clock_gtd();
double mpac_resolution_clock_rtc();

#endif	/* _MPAC_RESOLUTION_H */

