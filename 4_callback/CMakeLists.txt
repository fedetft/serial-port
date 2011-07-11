
cmake_minimum_required(VERSION 2.6)
project(TEST)

## Target
set(TEST_SRCS main.cpp AsyncSerial.cpp)
add_executable(simple_screen ${TEST_SRCS})

## Link libraries
set(BOOST_LIBS thread date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(simple_screen ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(simple_screen ${CMAKE_THREAD_LIBS_INIT})
