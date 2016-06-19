QT += core
QT -= gui

QT += websockets

CONFIG += c++11

LIBS += -lpthread -ldl
INCLUDEPATH += "wiringPi"
INCLUDEPATH += "src"

TARGET = deamonRaspi
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketserver.cpp \
    wiringPi/drcSerial.c \
    wiringPi/max5322.c \
    wiringPi/max31855.c \
    wiringPi/mcp23s08.c \
    wiringPi/mcp23s17.c \
    wiringPi/mcp3002.c \
    wiringPi/mcp3004.c \
    wiringPi/mcp3422.c \
    wiringPi/mcp4802.c \
    wiringPi/mcp23008.c \
    wiringPi/mcp23016.c \
    wiringPi/mcp23017.c \
    wiringPi/pcf8574.c \
    wiringPi/pcf8591.c \
    wiringPi/piHiPri.c \
    wiringPi/piThread.c \
    wiringPi/sn3218.c \
    wiringPi/softPwm.c \
    wiringPi/softServo.c \
    wiringPi/softTone.c \
    wiringPi/sr595.c \
    wiringPi/wiringPi.c \
    wiringPi/wiringPiI2C.c \
    wiringPi/wiringPiSPI.c \
    wiringPi/wiringSerial.c \
    wiringPi/wiringShift.c \
    wiringPi/wpiExtensions.c \
    src/devices.c \
    src/main_f.c \
    src/packet.c \
    src/raspi_ext.c \
    src/serial.c \
    src/server.c \
    src/sqlite3.c \
    src/SQLiteAPI.c

HEADERS += \
    websocketserver.h \
    wiringPi/drcSerial.h \
    wiringPi/max5322.h \
    wiringPi/max31855.h \
    wiringPi/mcp23s08.h \
    wiringPi/mcp23s17.h \
    wiringPi/mcp23x08.h \
    wiringPi/mcp23x0817.h \
    wiringPi/mcp3002.h \
    wiringPi/mcp3004.h \
    wiringPi/mcp3422.h \
    wiringPi/mcp4802.h \
    wiringPi/mcp23008.h \
    wiringPi/mcp23016.h \
    wiringPi/mcp23016reg.h \
    wiringPi/mcp23017.h \
    wiringPi/pcf8574.h \
    wiringPi/pcf8591.h \
    wiringPi/sn3218.h \
    wiringPi/softPwm.h \
    wiringPi/softServo.h \
    wiringPi/softTone.h \
    wiringPi/sr595.h \
    wiringPi/wiringPi.h \
    wiringPi/wiringPiI2C.h \
    wiringPi/wiringPiSPI.h \
    wiringPi/wiringSerial.h \
    wiringPi/wiringShift.h \
    wiringPi/wpiExtensions.h \
    src/devices.h \
    src/packet.h \
    src/raspi_ext.h \
    src/serial.h \
    src/server.h \
    src/sqlite3.h \
    src/sqlite3ext.h \
    src/SQLiteAPI.h

DISTFILES += \
    wiringPi/COPYING.LESSER \
    wiringPi/Makefile


DEFINES += SIMULATE_DATA=1
