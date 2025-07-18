#
# Flexitimer library cmake
# Copyright (c) 2010 Eray Ozturk <erayozturk1@gmail.com>
#
cmake_minimum_required(VERSION 3.10)
project(FlexiTimerLibrary LANGUAGES C CXX)

option(FLEXITIMER_MAX_TIMERS_OPTION "Set the maximum number of timers" ON)
if(FLEXITIMER_MAX_TIMERS_OPTION)
    set(FLEXITIMER_MAX_TIMERS 10 CACHE STRING "Maximum number of timers")
    add_definitions(-DFLEXITIMER_MAX_TIMERS=${FLEXITIMER_MAX_TIMERS})
endif()

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the library
add_library(flexitimer STATIC src/flexitimer.c)

# Add the examples subdirectory
add_subdirectory(examples)

# Add the unit tests
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(test)
