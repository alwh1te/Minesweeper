#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include <QMainWindow>
#include "Gameboard.h"
#include "NewGameDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();

private:
    void createMenus();
    void createToolBar();  // Новый метод
    GameBoard *gameBoard;
};

#endif // MINESWEEPER_MAINWINDOW_H
