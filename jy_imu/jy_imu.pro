TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    serial_port/serialport.cpp \
    jy_imu/imu.cpp

HEADERS += \
    serial_port/serialport.h \
    jy_imu/imu.h

LIBS += -lpthread
