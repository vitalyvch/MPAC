/* 
 * File:   mpac_signal.h
 * Author: ghulam
 *
 * Created on October 29, 2008, 3:32 PM
 */

#ifndef _MPAC_SIGNAL_H
#define	_MPAC_SIGNAL_H

#include "mpac.h"

void mpac_signal_setsigflag   ();
int mpac_signal_getsigflag    ();
int mpac_signal_sighandler    (int sig, void (*func)(void));
int mpac_signal_settimer      (double sec);
int mpac_signal_setitimer     (int duration);

#endif	/* _MPAC_SIGNAL_H */

