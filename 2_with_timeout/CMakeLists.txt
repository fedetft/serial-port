
cmake_minimum_required(VERSION 3.1)
project(TEST)

## Target
set(CMAKE_CXX_STANDARD 11)
set(TEST_SRCS main.cpp TimeoutSerial.cpp)
add_executable(timeout ${TEST_SRCS})

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(timeout ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(timeout ${CMAKE_THREAD_LIBS_INIT})
