/**
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

    This example manages a chicken farm ventilation system with four fans.
    Each fan runs for 15 minutes and then turns off for 1 hour, with a 20-minute offset between each fan.
*/

#include "flexitimer.h"
#include <stdio.h>
#include <unistd.h>

#define NUM_FANS    4
#define OFFSET      20 // minutes
#define ON_TIME     15 // minutes
#define OFF_TIME    60 // minutes

void fan_off(timer_id_t fan_id);

void fan_on(timer_id_t fan_id)
{
    printf("Fan %d ON\n", fan_id);
    flexitimer_start(fan_id, TIMER_TYPE_SINGLESHOT, ON_TIME, fan_off); // 15 minutes
}

void fan_off(timer_id_t fan_id)
{
    printf("Fan %d OFF\n", fan_id);
    flexitimer_start(fan_id, TIMER_TYPE_SINGLESHOT, OFF_TIME, fan_on); // 1 hour
}

int main(void)
{
    flexitimer_init();

    for(int i = 0; i < NUM_FANS; i++)
    {
        flexitimer_start(i, TIMER_TYPE_SINGLESHOT, i * OFFSET, fan_on);
    }

    int i = 220;

    while(i--)
    {
        flexitimer_handler();
        sleep(1); // Sleep for 1 minute
    }

    return 0;
}
