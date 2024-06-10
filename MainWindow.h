#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include <QMainWindow>
#include "GameBoard.h"
#include "NewGameDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void gameFinished(bool won);  // Новый слот для обработки окончания игры

private:
    void createMenus();
    void createToolBar();
    GameBoard *gameBoard;
    QMenu *gameMenu;
    QAction *newGameAction;
};

#endif // MINESWEEPER_MAINWINDOW_H
