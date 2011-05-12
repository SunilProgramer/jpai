# -------------------------------------------------
# Project created by QtCreator 2011-05-02T21:33:37
# -------------------------------------------------
QT += core \
    gui \
    opengl \
    sql
TARGET = proximity
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    drawer.cpp \
    aihandler.cpp \
    executor.cpp \
    directorymanager.cpp \
    settingsmanager.cpp \
    map.cpp \
    hex.cpp \
    utils.cpp \
    playercolor.cpp \
    initmanager.cpp \
    databasemanager.cpp \
    particlesystem.cpp \
    directorywatcher.cpp \
    mapdrawer.cpp \
    matchdialog.cpp \
    aiselect.cpp \
    gamehandler.cpp
HEADERS += mainwindow.h \
    definition.h \
    game.h \
    drawer.h \
    aihandler.h \
    executor.h \
    directorymanager.h \
    settingsmanager.h \
    hex.h \
    utils.h \
    playercolor.h \
    initmanager.h \
    databasemanager.h \
    particlesystem.h \
    directorywatcher.h \
    map.h \
    mapdrawer.h \
    matchdialog.h \
    aiselect.h \
    gamehandler.h
FORMS += mainwindow.ui \
    matchdialog.ui \
    aiselect.ui
