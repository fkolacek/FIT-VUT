#-------------------------------------------------
#
# Project created by QtCreator 2013-11-22T11:49:51
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qircserver.cpp \
    qirclogger.cpp \
    qircinifile.cpp \
    qircexception.cpp \
    qircuserdb.cpp \
    qircuser.cpp \
    qircroom.cpp \
    qircmessage.cpp \
    qircconnection.cpp \
    qircbuffer.cpp

HEADERS += \
    qircserver.h \
    qirclogger.h \
    qircinifile.h \
    qircexception.h \
    qircuserdb.h \
    qircuser.h \
    qircroom.h \
    qircmessage.h \
    qircconnection.h \
    qircbuffer.h

OTHER_FILES += \
    config.ini \
    users.db
