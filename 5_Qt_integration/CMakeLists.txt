
project(SerialGUI)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

set(SerialGUI_SRCS main.cpp mainwindow.cpp AsyncSerial.cpp QAsyncSerial.cpp)
set(SerialGUI_HEADERS mainwindow.h AsyncSerial.h QAsyncSerial.h)
add_executable(SerialGUI ${SerialGUI_SRCS})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(SerialGUI Qt5::Widgets)

set(BOOST_LIBS system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(SerialGUI ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(SerialGUI ${CMAKE_THREAD_LIBS_INIT})
