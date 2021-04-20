#-------------------------------------------------
#
# Project created by QtCreator 2019-07-21T11:44:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crowd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    userinfo.cpp \
    registerwindow.cpp \
    loginwindow.cpp \
    message.cpp \
    personmainwindow.cpp \
    messagewindow.cpp \
    taskinfo.cpp \
    posttaskwindow.cpp \
    mytaskwindow.cpp \
    tranwindow.cpp \
    checkuserwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    userinfo.h \
    registerwindow.h \
    platform.h \
    loginwindow.h \
    message.h \
    sqlconnection.h \
    personmainwindow.h \
    messagewindow.h \
    taskinfo.h \
    posttaskwindow.h \
    mytaskwindow.h \
    tranwindow.h \
    checkuserwindow.h \
    server.h

FORMS    += mainwindow.ui \
    registerwindow.ui \
    loginwindow.ui \
    personmainwindow.ui \
    messagewindow.ui \
    posttaskwindow.ui \
    mytaskwindow.ui \
    tranwindow.ui \
    checkuserwindow.ui

CONFIG += c++11

RESOURCES += \
    resource.qrc
