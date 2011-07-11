A note about compiling issues
=============================

The name of the boost libraries seems to change from release to release.
This is not a problem if you use CMake, but with QMake it is an issue.
 
To fix it open the file SerialGUI.pro and look at the LIBS variable:

LIBS += -L/usr/local/lib \
    -lboost_system \
    -lboost_thread \
    -lboost_date_time

Some version of the boost libraries have the -mt suffix. If this is the case
on your platform, modify the LIBS variable to:

LIBS += -L/usr/local/lib \
    -lboost_system-mt \
    -lboost_thread-mt \
    -lboost_date_time-mt


A note about windows users
==========================

To use boost.asio on windows you need to add this line to SerialGUI.pro:

DEFINES += _WIN32_WINNT=0x0501

and modify the LIBS variable to:

LIBS += -LC:/the path where you have the boost libraries  \
    -lboost_system \
    -lboost_thread \
    -lboost_date_time \
    -lwsock32 -lws2_32
