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
    explicit MainWindow(bool debug, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void gameFinished(bool won);
    void toggleDebugMode(bool checked);

private:
    void createMenus();
    void createToolBar();
    bool debugMode;
    GameBoard *gameBoard;
    QString saveFileName;
    QAction *debugAction;
};

#endif // MINESWEEPER_MAINWINDOW_H
