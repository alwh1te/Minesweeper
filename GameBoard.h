#ifndef MINESWEEPER_GAMEBOARD_H
#define MINESWEEPER_GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include "GameCell.h"

class GameBoard : public QWidget {
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    ~GameBoard();  // Добавим деструктор для освобождения ресурсов
    void setupBoard(int width, int height, int mines);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<QVector<GameCell*>> cells;
    int boardWidth;
    int boardHeight;
    int mineCount;
    int flaggedMines;
    bool firstClick;
    void placeMines(int firstX, int firstY);
    void revealCell(int x, int y);
    void revealEmptyCells(int x, int y);
    void checkForWin();
    void updateNumbers();
    void revealAllMines();
    void gameOver(bool won);

signals:
    void gameOverSignal(bool won);

private slots:
    void handleCellClick(int x, int y);
    void handleCellRightClick(int x, int y);
    void handleCellMiddleClick(int x, int y);
};

#endif // MINESWEEPER_GAMEBOARD_H
