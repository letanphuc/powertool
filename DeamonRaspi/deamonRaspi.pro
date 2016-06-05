QT += core
QT -= gui

QT += websockets

CONFIG += c++11

LIBS += -lpthread -ldl

TARGET = PhucTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketserver.cpp \
    server.c \
    sqlite3.c \
    SQLiteAPI.c

HEADERS += \
    server.h \
    sqlite3.h \
    sqlite3ext.h \
    SQLiteAPI.h \
    websocketserver.h
