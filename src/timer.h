#ifndef TIMER_H
#define TIMER_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <sys/time.h>
#include <stddef.h>
#include <stdbool.h>


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
  struct timeval start;
  struct timeval stop;
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
  timer->running       = false;
  timer->seconds       = 0;
  timer->start.tv_sec  = 0;
  timer->start.tv_usec = 0;
  timer->stop.tv_sec   = 0;
  timer->stop.tv_usec  = 0;
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
    gettimeofday(&(timer->start), NULL);
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
  gettimeofday(&(timer->stop), NULL);
  timer->seconds += (double)(timer->stop.tv_sec - timer->start.tv_sec);
  timer->seconds += 1e-6 * (timer->stop.tv_usec - timer->start.tv_usec);
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
