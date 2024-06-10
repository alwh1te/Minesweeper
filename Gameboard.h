#ifndef MINESWEEPER_GAMEBOARD_H
#define MINESWEEPER_GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include "GameCell.h"

class GameBoard : public QWidget {
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    void setupBoard(int width, int height, int mines);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<QVector<GameCell*>> cells;
    int boardWidth;
    int boardHeight;
    int mineCount;
    bool firstClick;
    void placeMines(int firstX, int firstY);
    void revealCell(int x, int y);
    void revealEmptyCells(int x, int y);
    void gameOver(bool won);
    void checkForWin();
    void updateNumbers();

private slots:
    void handleCellClick(int x, int y);
    void handleCellRightClick(int x, int y);
};

#endif // MINESWEEPER_GAMEBOARD_H
