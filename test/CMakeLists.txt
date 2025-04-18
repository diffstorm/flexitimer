set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(flexitimerTest flexitimerTest.cpp)
target_link_libraries(
    flexitimerTest 
    PRIVATE 
    flexitimer 
    GTest::GTest 
    GTest::Main
)

include(GoogleTest)
gtest_discover_tests(flexitimerTest)