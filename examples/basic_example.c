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

    This example demonstrates the basic usage of the FlexiTimer scheduler library with three timers.
    One timer sets another timer in its callback.
*/

#include "flexitimer.h"
#include <stdio.h>
#include <unistd.h>

void timer_callback_2(timer_id_t i);
void timer_callback_3(timer_id_t i);

void timer_callback_1(timer_id_t i)
{
    printf("Timer %d expired! Setting Timer 1.\n", i);
    flexitimer_start(1, TIMER_TYPE_SINGLESHOT, 3, timer_callback_2); // 3 seconds
}

void timer_callback_2(timer_id_t i)
{
    printf("Timer %d expired! Setting Timer 2.\n", i);
    flexitimer_start(2, TIMER_TYPE_SINGLESHOT, 2, timer_callback_3); // 2 seconds
}

void timer_callback_3(timer_id_t i)
{
    printf("Timer %d expired!\n", i);
}

void timer_callback_4(timer_id_t i)
{
    printf("Periodic timer %d expired!\n", i);
}

int main(void)
{
    flexitimer_init();
    flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 5, timer_callback_1); // 5 seconds
    flexitimer_start(2, TIMER_TYPE_SINGLESHOT, 0, timer_callback_3); // immediate
    flexitimer_start(3, TIMER_TYPE_PERIODIC,   1, timer_callback_4); // every second
    int i = 10;

    while(i--)
    {
        flexitimer_handler();
        sleep(1); // Sleep for 1 second
    }

    flexitimer_cancel(3);
    printf("Periodic timer is cancelled!\r\n");
    return 0;
}
