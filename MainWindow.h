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
    void gameFinished(bool won);

private:
    void createMenus();
    void createToolBar();
    GameBoard *gameBoard;
    QString saveFileName; // Добавлено для хранения имени файла сохранения
};

#endif // MINESWEEPER_MAINWINDOW_H
