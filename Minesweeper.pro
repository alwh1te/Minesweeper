QT += core gui widgets

CONFIG   += c++20

TARGET = Minesweeper
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gameboard.cpp \
    gamecell.cpp \
    newgamedialog.cpp

HEADERS += \
    mainwindow.h \
    gameboard.h \
    gamecell.h \
    newgamedialog.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += translations/minesweeper_en.ts \
               translations/minesweeper_ru.ts

INCLUDEPATH += .
