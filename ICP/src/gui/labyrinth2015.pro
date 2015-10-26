#-------------------------------------------------
#
# Project created by QtCreator 2015-05-06T20:24:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app


SOURCES += main.cpp qmain.cpp qdesk.cpp qstone.cpp qdialogconnect.cpp \
            ../cli/utils.cpp ../cli/exception.cpp ../cli/cli.cpp ../cli/game.cpp ../cli/client.cpp \
    qcli.cpp \
    qcustombutton.cpp \
    qdialoghelp.cpp \
    qdialogloading.cpp \
    qdialoginvite.cpp \
    qdebuglog.cpp \
    qdialognewgame.cpp \
    qcustomwrapper.cpp \
    qdialoggames.cpp


HEADERS  += qmain.h qdesk.h qstone.h qdialogconnect.h \
            ../cli/utils.h ../cli/exception.h ../cli/cli.h ../cli/game.h ../cli/client.h \
    qcli.h \
    qcustombutton.h \
    qdialoghelp.h \
    qdialogloading.h \
    qdialoginvite.h \
    qdebuglog.h \
    qdialognewgame.h \
    qcustomwrapper.h \
    qdialoggames.h

FORMS    += qmain.ui qdesk.ui qdialogconnect.ui \
    qdialoghelp.ui \
    qdialogloading.ui \
    qdialoginvite.ui \
    qdebuglog.ui \
    qdialognewgame.ui \
    qdialoggames.ui

RESOURCES += \
    images.qrc
