
cmake_minimum_required(VERSION 3.1)
project(TEST)

## Target
set(CMAKE_CXX_STANDARD 11)
set(TEST_SRCS main.cpp)
add_executable(simple ${TEST_SRCS})

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(simple ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(simple ${CMAKE_THREAD_LIBS_INIT})
