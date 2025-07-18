# FlexiTimer Scheduler Library [![Awesome](https://awesome.re/badge.svg)](https://github.com/diffstorm/flexitimer)

[![Build Status](https://github.com/diffstorm/flexitimer/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/diffstorm/flexitimer/actions)
[![License](https://img.shields.io/github/license/diffstorm/flexitimer)](https://github.com/diffstorm/flexitimer/blob/main/LICENSE)
[![Language](https://img.shields.io/github/languages/top/diffstorm/flexitimer)](https://github.com/diffstorm/flexitimer)
[![Code Coverage](https://codecov.io/gh/diffstorm/flexitimer/branch/main/graph/badge.svg)](https://codecov.io/gh/diffstorm/flexitimer)
![GitHub Stars](https://img.shields.io/github/stars/diffstorm/flexitimer?style=social)
![Platforms](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)

A fast and efficient software timer library designed to work seamlessly across any embedded system, operating system, or bare-metal environment. With full test coverage and MISRA C compliance, it ensures safety and reliability, making it ideal for real-time applications. The timer resolution is flexible and depends on the frequency of handler function calls, providing high precision for diverse use cases.

## Features

- **Lightweight & Fast**: Optimized for minimal resource usage.
- **Platform-Independent**: Compatible with embedded systems, operating systems, and bare-metal environments.
- **MISRA-Compliant**: Follows MISRA C guidelines for safety and reliability.
- **Flexible Timer Resolution**: The resolution of the software timers depends on the calling frequency of the handler function.
- **Industry-Proven**: Deployed in diverse industrial applications and rigorously unit-tested.

## Getting Started

### Prerequisites

- A C compiler (e.g., GCC)
- CMake (for building the project)

### Building the Library and Examples

1. **Clone the repository:**

    ```bash
    git clone https://github.com/diffstorm/flexitimer.git
    cd flexitimer
    ```

2. **Create a build directory and navigate to it:**

    ```bash
    mkdir build
    cd build
    ```

3. **Run CMake to configure the project:**

    ```bash
    cmake ..
    ```

4. **Build the project:**

    ```bash
    make
    ```

5. **Run the unit tests:**

    ```bash
    ./test/flexitimerTest
    ```

### Library Usage

Include the library header in your project:

```c
#define FLEXITIMER_MAX_TIMERS (50) // Optional
#include "flexitimer.h"
```

Define the callback function(s) by using the same function signature as `timer_callback_t`

```c
void callback(timer_id_t i)
{
    // code
}
```

Initialize the scheduler in your main function:

```c
flexitimer_init();
```

Start timers:

```c
flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 1000, ctrl_power_on); // 1 second
flexitimer_start(1, TIMER_TYPE_PERIODIC, 2000, read_sensors); // 2 seconds
flexitimer_start(2, TIMER_TYPE_SINGLESHOT, 5000, heating_on); // 5 seconds
flexitimer_start(3, TIMER_TYPE_PERIODIC, 500, check_faults); // Every 0.5 seconds
flexitimer_start(4, TIMER_TYPE_PERIODIC, 10, read_inputs); // Every 10 ms
```

Call the handler function periodically (e.g., in a timer interrupt or main loop):

```c
while (1) {
    flexitimer_handler();
    usleep(1000); // Sleep for 1 millisecond (example)
}
```

To ensure optimal performance, it is preferred that callback functions are non-blocking or consist of minimal, efficient code. This prevents any delays in the execution of the scheduler.

### Examples

The repository includes several example programs to demonstrate the library's usage:

- **Basic Example**: Shows basic usage with three timers.
- **Traffic Lights**: Simulates a traffic light system.
- **Process Watchdog**: Monitors multiple threads and restarts them if they become unresponsive.
- **Chicken Farm Ventilation System**: Manages the operation of ventilation fans in a chicken farm.
- **Industrial Device**: Periodically reads sensors and I/Os, deals with sensor errors.

#### Running Examples

After building the project, the executables for the examples will be located in the `build/examples` directory. You can run them from the terminal:

```bash
./examples/basic_example
./examples/traffic_light
./examples/process_watchdog
./examples/ventilation_system
./examples/industrial_device
```

## API Reference

### Initialization

```c
void flexitimer_init(void);
```
Initializes the scheduler by resetting all timers.

### Starting a Timer

```c
flexitimer_error_t flexitimer_start(timer_id_t id, timer_type_t type, timer_time_t timeout, timer_callback_t callback);
```
Starts a timer with the specified id, type, timeout, and callback.

Example Callback function

```c
void timer_callback_1(timer_id_t i)
{
    printf("Timer %d expired!\n", i);
}
...
flexitimer_start(0, TIMER_TYPE_SINGLESHOT, 5000, timer_callback_1);
```

### Handler Function

```c
void flexitimer_handler(void);
```
The scheduler handler that should be called periodically to manage timers.

### Postponing/Delaying a Timer

```c
flexitimer_error_t flexitimer_delay(timer_id_t id, timer_time_t delay);
```
Postpones / delays the timer with the specified id.

### Pausing a Timer

```c
flexitimer_error_t flexitimer_pause(timer_id_t id);
```
Pauses the timer with the specified id.

### Resuming a Timer

```c
flexitimer_error_t flexitimer_resume(timer_id_t id);
```
Resumes the paused timer with the specified id.

### Restarting a Timer

```c
flexitimer_error_t flexitimer_restart(timer_id_t id);
```
Restarts the timer with the specified id. Can now also re-activate expired single-shot timers if they have a valid callback.

### Cancelling a Timer

```c
flexitimer_error_t flexitimer_cancel(timer_id_t id);
```
Cancels the timer with the specified id.

### Getting Timer State

```c
flexitimer_error_t flexitimer_get_state(timer_id_t id, timer_state_t *state);
```
Gets the state of the timer with the specified id.

### Getting Timer Type

```c
flexitimer_error_t flexitimer_get_type(timer_id_t id, timer_type_t *type);
```
Gets the type of the timer with the specified id.

### Getting Original Timeout

```c
flexitimer_error_t flexitimer_get_time(timer_id_t id, timer_time_t *time);
```
Gets the original timeout value of the timer with the specified id.

### Getting Remaining Time

```c
flexitimer_error_t flexitimer_get_elapsed(timer_id_t id, timer_time_t *time);
```
Gets the remaining time of the timer with the specified id.

## Best Practices / Tips
- Configure `FLEXITIMER_MAX_TIMERS` via CMake: The maximum number of timers can be set during the CMake configuration step. This allows you to adjust the library's capacity without modifying source files.
```bash
cmake -DFLEXITIMER_MAX_TIMERS=50 ..
```
  You can also adjust `timer_id_t` and `timer_time_t` in `flexitimer.h` to match specific needs and save memory in resource-constrained environments.
- Ensure callback functions are non-blocking and consist of minimal, efficient code to prevent delays in the scheduler execution.
- Use an enum to list timer IDs in a single place for easier management and readability.
- Utilize getter functions to control the flow and monitor timer states effectively.
- Always check the return values of library functions to handle errors appropriately.
- Avoid using the same callback function for both periodic and single-shot timers to prevent unexpected behavior.
- Use mutexes or other synchronization mechanisms if timers interact with shared resources in a multi-threaded environment.
- Implement robust error handling and logging within callback functions to identify and troubleshoot issues quickly.
- Consider using the Proxy design pattern to test callbacks. By using a proxy, you can intercept calls to the real callback functions, allowing you to simulate different conditions, measure execution times, and verify that the scheduler behaves correctly without modifying the actual callback logic.

## :snowman: Author

Eray Öztürk ([@diffstorm](https://github.com/diffstorm))

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
