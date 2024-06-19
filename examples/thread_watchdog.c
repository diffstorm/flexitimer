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

    This example implements a thread watchdog for five threads.
    Each thread must call the flexitimer_delay function before the timer expires, or the thread is considered stuck and is restarted.
*/

#include "flexitimer.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 5
#define WATCHDOG_TIMEOUT 5 // 5 seconds

pthread_t threads[NUM_THREADS];

void start_thread(int i);

void thread_work(int id)
{
    while(1)
    {
        uint32_t remaining_time;
        flexitimer_get_elapsed(id, &remaining_time);

        if(remaining_time < WATCHDOG_TIMEOUT)
        {
            if(SCHEDULER_OK == flexitimer_delay(id, WATCHDOG_TIMEOUT))
            {
                printf("Delayed thread %d.\n", id);
            }
        }

        sleep((rand() % (WATCHDOG_TIMEOUT * 2)) + 1); // Simulate work
    }
}

void watchdog_callback(timer_id_t i)
{
    printf("Watchdog timer expired! Restarting thread %d.\n", i);

    if(pthread_cancel(threads[i]) == 0)
    {
        printf("Thread canceled successfully.\n");
    }
    else
    {
        printf("Error canceling thread\n");
    }

    start_thread(i);
}

void start_thread(int i)
{
    pthread_create(&threads[i], NULL, (void *(*)(void *))thread_work, (void *)(intptr_t)i);
    flexitimer_start(i, TIMER_TYPE_SINGLESHOT, WATCHDOG_TIMEOUT, watchdog_callback);
    printf("Started thread %d.\n", i);
}

int main(void)
{
    srand(time(NULL));
    flexitimer_init();

    for(int i = 0; i < NUM_THREADS; i++)
    {
        start_thread(i);
    }

    int i = NUM_THREADS * 10;

    while(i--)
    {
        flexitimer_handler();
        sleep(1); // Sleep for 1 second
    }

    return 0;
}
