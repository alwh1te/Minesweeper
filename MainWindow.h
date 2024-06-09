//
// Created by Alexander on 09.06.2024.
//

#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include <QMainWindow>
#include "Gameboard.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();

private:
    GameBoard *gameBoard;
    void createMenus();
};

#endif//MINESWEEPER_MAINWINDOW_H
