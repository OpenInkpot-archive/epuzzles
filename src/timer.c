#include <Ecore.h>
#include <time.h>
#include <sys/time.h>
#include "frontend.h"

void get_random_seed(void **randseed, int *randseedsize)
{
    struct timeval *tvp = snew(struct timeval);
    gettimeofday(tvp, NULL);
    *randseed = (void *)tvp;
    *randseedsize = sizeof(struct timeval);
}

static int timer_func(void *data)
{
    frontend *fe = (frontend *)data;

    if (fe->timer_active) {
	struct timeval now;
	float elapsed;
	gettimeofday(&now, NULL);
	elapsed = ((now.tv_usec - fe->last_time.tv_usec) * 0.000001F +
		   (now.tv_sec - fe->last_time.tv_sec));
        midend_timer(fe->me, elapsed);	/* may clear timer_active */
	fe->last_time = now;
    }

    return fe->timer_active;
}

void deactivate_timer(frontend *fe)
{
    if (!fe)
	return;			       /* can happen due to --generate */
    if (fe->timer_active)
        ecore_timer_del(fe->timer_id);
    fe->timer_active = FALSE;
}

void activate_timer(frontend *fe)
{
    if (!fe)
	return;			       /* can happen due to --generate */
    if (!fe->timer_active) {
        fe->timer_id = ecore_timer_add(20, timer_func, (void *) fe);
	gettimeofday(&fe->last_time, NULL);
    }
    fe->timer_active = TRUE;
}

