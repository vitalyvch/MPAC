#include "mpac.h"
#include "mpac_signal.h"

static int sigflag = 0;

void mpac_signal_setsigflag()
{
    sigflag = 1;
    sleep(1);
}

int mpac_signal_getsigflag()
{
    return sigflag;
}
int mpac_signal_sighandler(int sig, void (*func)(void))
{
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = (void (*)(int))(*func);
    if (sigemptyset(&act.sa_mask) == -1)
           return MPAC_FAILURE;
    if (sigaction(sig, &act, NULL) == -1)
        return MPAC_FAILURE;
    return MPAC_SUCCESS;
}

int mpac_signal_settimer(double duration)
{
    timer_t timer;
    struct itimerspec tval;
    if (timer_create(CLOCK_REALTIME, NULL, &timer) == -1)
      return MPAC_FAILURE;
  
    tval.it_interval.tv_sec = (long)duration;
    tval.it_interval.tv_nsec = (duration - tval.it_interval.tv_sec)*1000000000.0;
    if (tval.it_interval.tv_nsec >= 1000000000.0)
    {
        tval.it_interval.tv_sec++;
        tval.it_interval.tv_nsec -= 1000000000.0;
    }
    tval.it_value = tval.it_interval;
    return timer_settime(timer, 0, &tval, NULL);
}

int mpac_signal_setitimer(int duration)
{
   struct itimerval tval;
   tval.it_interval.tv_sec = duration;
   tval.it_interval.tv_usec = 0;
   tval.it_value = tval.it_interval;
   return (setitimer(ITIMER_REAL, &tval, NULL));
}
