cmake_minimum_required(VERSION 3.27)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Minesweeper main.cpp
        MainWindow.h
        Gameboard.h
        Gamecell.h
        Gamecell.cpp
        Gameboard.cpp
        MainWindow.cpp
        NewGameDialog.cpp
        NewGameDialog.h
)
target_link_libraries(Minesweeper
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

