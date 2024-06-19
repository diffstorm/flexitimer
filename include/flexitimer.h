/**
    @file flexitimer.h
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

#ifndef FLEXITIMER_H
#define FLEXITIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
    @brief Number of times
*/
#define FLEXITIMER_MAX_TIMERS (10)

/**
    @brief Id unit type
*/
typedef uint8_t timer_id_t;

/**
    @brief Time unit type
*/
typedef uint32_t timer_time_t;

/**
    @brief Timer callback function type.

    @param id The unique id of the timer.
*/
typedef void (*timer_callback_t)(timer_id_t id);

/**
    @brief Timer type enumeration.
*/
typedef enum
{
    TIMER_TYPE_SINGLESHOT,
    TIMER_TYPE_PERIODIC
} timer_type_t;

/**
    @brief Timer state enumeration.
*/
typedef enum
{
    TIMER_STATE_PASSIVE,
    TIMER_STATE_ACTIVE,
    TIMER_STATE_PAUSED
} timer_state_t;

/**
    @brief Error enumeration for scheduler functions.
*/
typedef enum
{
    FLEXITIMER_OK,
    FLEXITIMER_ERROR_INVALID_ID,
    FLEXITIMER_ERROR_INVALID_STATE,
    FLEXITIMER_ERROR_INVALID_ARG,
    FLEXITIMER_ERROR_ZERO_TIMEOUT
} flexitimer_error_t;

/**
    @brief Initializes the scheduler.
*/
void flexitimer_init(void);

/**
    @brief Starts a timer with the specified parameters.
    @param id Timer identifier.
    @param type Timer type (singleshot or periodic).
    @param timeout Timeout value in milliseconds.
    @param callback Callback function to be called when the timer expires.
    @return Error code.
*/
flexitimer_error_t flexitimer_start(timer_id_t id, timer_type_t type, timer_time_t timeout, timer_callback_t callback);

/**
    @brief Handler function to be called in a loop.
*/
void flexitimer_handler(void);

/**
    @brief Postpones / Delays the specified timer.
    @param id Timer identifier.
    @param delay Delay value to be added to the timeout.
    @return Error code.
*/
flexitimer_error_t flexitimer_delay(timer_id_t id, timer_time_t delay);

/**
    @brief Pauses the specified timer.
    @param id Timer identifier.
    @return Error code.
*/
flexitimer_error_t flexitimer_pause(timer_id_t id);

/**
    @brief Resumes the specified timer.
    @param id Timer identifier.
    @return Error code.
*/
flexitimer_error_t flexitimer_resume(timer_id_t id);

/**
    @brief Restarts the specified timer.
    @param id Timer identifier.
    @return Error code.
*/
flexitimer_error_t flexitimer_restart(timer_id_t id);

/**
    @brief Cancels the specified timer.
    @param id Timer identifier.
    @return Error code.
*/
flexitimer_error_t flexitimer_cancel(timer_id_t id);

/**
    @brief Gets the state of the specified timer.
    @param id Timer identifier.
    @param state Pointer to store the state.
    @return Error code.
*/
flexitimer_error_t flexitimer_get_state(timer_id_t id, timer_state_t *state);

/**
    @brief Gets the type of the specified timer.
    @param id Timer identifier.
    @param type Pointer to store the type.
    @return Error code.
*/
flexitimer_error_t flexitimer_get_type(timer_id_t id, timer_type_t *type);

/**
    @brief Gets the original timeout of the specified timer.
    @param id Timer identifier.
    @param time Pointer to store the original timeout.
    @return Error code.
*/
flexitimer_error_t flexitimer_get_time(timer_id_t id, timer_time_t *time);

/**
    @brief Gets the remaining time of the specified timer.
    @param id Timer identifier.
    @param time Pointer to store the remaining time.
    @return Error code.
*/
flexitimer_error_t flexitimer_get_elapsed(timer_id_t id, timer_time_t *time);

#ifdef __cplusplus
}
#endif

#endif // FLEXITIMER_H
