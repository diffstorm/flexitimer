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

    This example manages an industrial device that reads sensors and I/Os periodically, also deals with sensor errors.
*/

#include "flexitimer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SENSORS     6
#define ID_IO           NUM_SENSORS
#define ID_PWRSWITCH    (NUM_SENSORS + 1)
int sensor_error = 0;
int sensor_id = 0;

void read_sensor(timer_id_t id)
{
    printf("Reading sensor %d\n", id);
    int error = rand() % NUM_SENSORS; // Simulate random error on sensor system

    if(sensor_error == 0 && error == 1)
    {
        printf("Error on sensor %d, reading stopped\n", id);
        sensor_error = 1;
        sensor_id = id;
        flexitimer_pause(id);
    }
}

void read_io(timer_id_t id)
{
    printf("Reading IOs\n");
}

void sensor_power_settle(timer_id_t id)
{
    printf("Sensor %d power settled\n", sensor_id);
    flexitimer_resume(sensor_id);
    sensor_error = 0;
    printf("Sensor reading resumes for sensor %d\n", sensor_id);
}

void sensor_power_on(timer_id_t id)
{
    printf("Sensor %d powered on\n", sensor_id);
    flexitimer_start(ID_PWRSWITCH, TIMER_TYPE_SINGLESHOT, 3, sensor_power_settle);
}

void sensor_power_off(timer_id_t id)
{
    printf("Sensor %d powered off\n", sensor_id);
    flexitimer_start(ID_PWRSWITCH, TIMER_TYPE_SINGLESHOT, 10, sensor_power_on);
}

void error_handler()
{
    timer_state_t state;
    flexitimer_get_state(ID_PWRSWITCH, &state);

    if(sensor_error && state == TIMER_STATE_PASSIVE)
    {
        flexitimer_start(ID_PWRSWITCH, TIMER_TYPE_SINGLESHOT, 1, sensor_power_off);
    }
}

int main(void)
{
    srand(time(NULL));
    flexitimer_init();
    int i;

    for(i = 0; i < NUM_SENSORS; i++)
    {
        flexitimer_start(i, TIMER_TYPE_PERIODIC, 5 + i * 2, read_sensor);
    }

    flexitimer_start(ID_IO, TIMER_TYPE_PERIODIC, 3, read_io);
    i = 1000;

    while(i--)
    {
        flexitimer_handler();
        error_handler();
        usleep(100000); // Sleep for 100 millisecond
    }

    return 0;
}
