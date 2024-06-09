QT       += core gui widgets

CONFIG   += c++20

TARGET = Minesweeper
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gameboard.cpp \
    gamecell.cpp

HEADERS += \
    mainwindow.h \
    gameboard.h \
    gamecell.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += .
