/**
    @file flexitimer.c
    @brief FlexiTimer Scheduler Library

    FlexiTimer is a fast and efficient software timer library designed to work seamlessly across
    any embedded system, operating system, or bare-metal environment.
    With MISRA C compliance, it ensures safety and reliability, making it ideal for real-time applications.
    The timer resolution is flexible and depends on the frequency of the handler function calls,
    providing high precision for various use cases.

    @date 2010-02-18
    @version 1.0
    @author Eray Ozturk | erayozturk1@gmail.com
    @url github.com/diffstorm
    @license MIT License
*/

#include "flexitimer.h"
#include <stdio.h> // for NULL

/**
    @brief Timer structure.
*/
typedef struct
{
    timer_time_t timeout;
    timer_time_t remaining;
    timer_type_t type;
    timer_state_t state;
    timer_callback_t callback;
} timer_t;

static timer_t timers[FLEXITIMER_MAX_TIMERS];

/* Initializes the scheduler */
void flexitimer_init(void)
{
    for(timer_id_t i = 0; i < FLEXITIMER_MAX_TIMERS; i++)
    {
        flexitimer_cancel(i);
    }
}

/* Starts a timer with the specified parameters */
flexitimer_error_t flexitimer_start(timer_id_t id, timer_type_t type, timer_time_t timeout, timer_callback_t callback)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(type == TIMER_TYPE_PERIODIC && timeout == 0)
    {
        return FLEXITIMER_ERROR_ZERO_TIMEOUT;
    }

    timers[id].timeout = timeout;
    timers[id].remaining = timeout;
    timers[id].type = type;
    timers[id].state = TIMER_STATE_ACTIVE;
    timers[id].callback = callback;
    return FLEXITIMER_OK;
}

/* Handler function to be called in a loop */
void flexitimer_handler(void)
{
    for(timer_id_t i = 0; i < FLEXITIMER_MAX_TIMERS; i++)
    {
        if(timers[i].state == TIMER_STATE_ACTIVE)
        {
            if(timers[i].remaining > 0)
            {
                timers[i].remaining--;
            }

            if(timers[i].remaining == 0)
            {
                if(timers[i].type == TIMER_TYPE_PERIODIC)
                {
                    timers[i].remaining = timers[i].timeout;
                }
                else
                {
                    timers[i].state = TIMER_STATE_PASSIVE;
                }

                if(timers[i].callback)
                {
                    timers[i].callback(i);
                }
            }
        }
    }
}

/* Delays the specified timer */
flexitimer_error_t flexitimer_delay(timer_id_t id, timer_time_t delay)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(timers[id].state != TIMER_STATE_PASSIVE)
    {
        timers[id].remaining += delay;
        return FLEXITIMER_OK;
    }

    return FLEXITIMER_ERROR_INVALID_STATE;
}

/* Pauses the specified timer */
flexitimer_error_t flexitimer_pause(timer_id_t id)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(timers[id].state == TIMER_STATE_ACTIVE)
    {
        timers[id].state = TIMER_STATE_PAUSED;
        return FLEXITIMER_OK;
    }

    return FLEXITIMER_ERROR_INVALID_STATE;
}

/* Resumes the specified timer */
flexitimer_error_t flexitimer_resume(timer_id_t id)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(timers[id].state == TIMER_STATE_PAUSED)
    {
        timers[id].state = TIMER_STATE_ACTIVE;
        return FLEXITIMER_OK;
    }

    return FLEXITIMER_ERROR_INVALID_STATE;
}

/* Restarts the specified timer */
flexitimer_error_t flexitimer_restart(timer_id_t id)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(timers[id].state != TIMER_STATE_PASSIVE)
    {
        timers[id].remaining = timers[id].timeout;
        timers[id].state = TIMER_STATE_ACTIVE;
        return FLEXITIMER_OK;
    }

    return FLEXITIMER_ERROR_INVALID_STATE;
}

/* Cancels the specified timer */
flexitimer_error_t flexitimer_cancel(timer_id_t id)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    timers[id].state = TIMER_STATE_PASSIVE;
    timers[id].remaining = 0;
    timers[id].callback = NULL;
    return FLEXITIMER_OK;
}

/* Gets the state of the specified timer */
flexitimer_error_t flexitimer_get_state(timer_id_t id, timer_state_t *state)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(state == NULL)
    {
        return FLEXITIMER_ERROR_INVALID_ARG;
    }

    *state = timers[id].state;
    return FLEXITIMER_OK;
}

/* Gets the type of the specified timer */
flexitimer_error_t flexitimer_get_type(timer_id_t id, timer_type_t *type)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(type == NULL)
    {
        return FLEXITIMER_ERROR_INVALID_ARG;
    }

    *type = timers[id].type;
    return FLEXITIMER_OK;
}

/* Gets the original timeout of the specified timer */
flexitimer_error_t flexitimer_get_time(timer_id_t id, timer_time_t *time)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(time == NULL)
    {
        return FLEXITIMER_ERROR_INVALID_ARG;
    }

    *time = timers[id].timeout;
    return FLEXITIMER_OK;
}

/* Gets the remaining time of the specified timer */
flexitimer_error_t flexitimer_get_elapsed(timer_id_t id, timer_time_t *time)
{
    if(id >= FLEXITIMER_MAX_TIMERS)
    {
        return FLEXITIMER_ERROR_INVALID_ID;
    }

    if(time == NULL)
    {
        return FLEXITIMER_ERROR_INVALID_ARG;
    }

    *time = timers[id].remaining;
    return FLEXITIMER_OK;
}
