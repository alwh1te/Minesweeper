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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newGame();
    void gameFinished(bool won);

    void saveGame();
    void loadGame();

private:
    void createMenus();
    void createToolBar();
    GameBoard *gameBoard;
    void autoLoadGame();
};

#endif // MINESWEEPER_MAINWINDOW_H
