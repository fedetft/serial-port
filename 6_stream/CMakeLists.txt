
cmake_minimum_required(VERSION 3.1)
project(TEST)

## Target
set(CMAKE_CXX_STANDARD 11)
set(TEST_SRCS main.cpp serialstream.cpp)
add_executable(stream ${TEST_SRCS})

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(stream ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(stream ${CMAKE_THREAD_LIBS_INIT})
