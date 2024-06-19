#
# Flexitimer library examples cmake
# Copyright (c) 2010 Eray Ozturk <erayozturk1@gmail.com>
#

# Specify the CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(FlexitimerExamples)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/../include)

# Add the executable for each example
add_executable(basic_example basic_example.c)
add_executable(traffic_light traffic_light.c)
add_executable(thread_watchdog thread_watchdog.c)
add_executable(ventilation_system ventilation_system.c)
add_executable(industrial_device industrial_device.c)

# Link the scheduler library to each example
target_link_libraries(basic_example flexitimer)
target_link_libraries(traffic_light flexitimer)
target_link_libraries(thread_watchdog flexitimer)
target_link_libraries(ventilation_system flexitimer)
target_link_libraries(industrial_device flexitimer)
