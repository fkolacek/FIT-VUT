#-------------------------------------------------
#
# Project created by QtCreator 2013-11-26T22:05:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        client_window.cpp \
    about.cpp \
    new_server.cpp \
    join_chan.cpp \
    new_account.cpp \
    change_account.cpp

HEADERS  += client_window.h \
    about.h \
    new_server.h \
    join_chan.h \
    new_account.h \
    change_account.h

FORMS    += client_window.ui \
    about.ui \
    new_server.ui \
    join_chan.ui \
    new_account.ui \
    change_account.ui

RESOURCES += \
    client.qrc
