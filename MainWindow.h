#pragma once

#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QDir>
#include "GameBoard.h"
#include "NewGameDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void gameFinished(bool won);

private:
    void createMenus();
    void createToolBar();
    GameBoard *gameBoard;
    QString saveFileName;
};

#endif // MINESWEEPER_MAINWINDOW_H
