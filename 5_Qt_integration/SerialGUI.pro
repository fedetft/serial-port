# -------------------------------------------------
# Project created by QtCreator 2010-04-14T20:57:49
# -------------------------------------------------
TARGET = SerialGUI
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    QAsyncSerial.cpp \
    AsyncSerial.cpp
HEADERS += mainwindow.h \
    QAsyncSerial.h \
    AsyncSerial.h
FORMS += mainwindow.ui
LIBS += -L/usr/local/lib \
    -lboost_system \
    -lboost_thread \
    -lboost_date_time
