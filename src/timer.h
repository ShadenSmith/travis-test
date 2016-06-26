#ifndef TIMER_H
#define TIMER_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <time.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __MACH__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

/******************************************************************************
 * STRUCTURES
 *****************************************************************************/

/**
* @brief Represents a wall-clock timer.
*/
typedef struct
{
  bool running;
  double seconds;
  double start;
  double stop;
} sp_timer_t;




/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/


/**
* @brief Reset all fields of a sp_timer_t.
*
* @param timer The timer to reset.
*/
static inline void timer_reset(sp_timer_t * const timer)
{
  timer->running = false;
  timer->seconds = 0;
  timer->start   = 0;
  timer->stop    = 0;
}

double monotonic_seconds()
{
#ifdef __MACH__
  static mach_timebase_info_data_t info;
  static double seconds_per_unit;
  if(seconds_per_unit == 0) {
    mach_timebase_info(&info);
    seconds_per_unit = (info.numer / info.denom) / 1e9;
  }
  return seconds_per_unit * mach_absolute_time();
#else
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

/**
* @brief Start a sp_timer_t. NOTE: this does not reset the timer.
*
* @param timer The timer to start.
*/
static inline void timer_start(sp_timer_t * const timer)
{
  if(!timer->running) {
    timer->running = true;
    timer->start = monotonic_seconds();
  }
}


/**
* @brief Stop a sp_timer_t and update its time.
*
* @param timer The timer to stop.
*/
static inline void timer_stop(sp_timer_t * const timer)
{
  timer->running = false;
  timer->stop = monotonic_seconds();
  timer->seconds += timer->stop - timer->start;
}


/**
* @brief Give a sp_timer_t a fresh start by resetting and starting it.
*
* @param timer The timer to refresh.
*/
static inline void timer_fstart(sp_timer_t * const timer)
{
  timer_reset(timer);
  timer_start(timer);
}

#endif
