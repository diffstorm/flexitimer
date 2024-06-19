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

    This example simulates a traffic light system.
    The lights switch between green, yellow, and red in a coordinated manner.
*/

#include "flexitimer.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void light(bool r, bool y, bool g)
{
    printf("Red :\t%s\n", r ? "on" : "off");
    printf("Yellow :\t%s\n", y ? "on" : "off");
    printf("Green :\t%s\n", g ? "on" : "off");
    printf("--------------\n");
}

void yellow_light(timer_id_t i);
void green_light(timer_id_t i);

void red_light(timer_id_t i)
{
    light(true, false, false);
    flexitimer_start(1, TIMER_TYPE_SINGLESHOT, 8, yellow_light);
}

void yellow_light(timer_id_t i)
{
    light(true, true, false);
    flexitimer_start(2, TIMER_TYPE_SINGLESHOT, 2, green_light);
}

void green_light(timer_id_t i)
{
    light(false, false, true);
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 15, red_light);
}

int main(void)
{
    flexitimer_init();
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 0, green_light); // immediate
    int i = 100;

    while(i--)
    {
        flexitimer_handler();
        sleep(1); // 1 second
    }

    return 0;
}

