//
// Created by Alexander on 09.06.2024.
//

#ifndef MINESWEEPER_GAMECELL_H
#define MINESWEEPER_GAMECELL_H

#include <QPushButton>


class GameCell : public QPushButton {
    Q_OBJECT

public:
    explicit GameCell(int x, int y, QWidget *parent = nullptr);
    void reveal();
    bool hasMine() const;
    void setMine(bool hasMine);
    bool isRevealed() const;
    void setNumber(int number);
    int getNumber() const;

signals:
    void cellClicked(int x, int y);
    void cellRightClicked(int x, int y);
    void cellMiddleClicked(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int posX;
    int posY;
    bool mine;
    bool revealed;
    int number;
};

#endif//MINESWEEPER_GAMECELL_H
