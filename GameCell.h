#pragma once

#ifndef MINESWEEPER_GAMECELL_H
#define MINESWEEPER_GAMECELL_H

#include <QMouseEvent>
#include <QPushButton>
#include <QIcon>

class GameCell : public QPushButton {
    Q_OBJECT

public:
    GameCell(int x, int y, QWidget *parent = nullptr);
    void setMine(bool hasMine);
    bool hasMine() const;
    void setNumber(int number);
    int getNumber() const;
    void reveal();
    bool isRevealed() const;
    void toggleFlag();
    bool isFlagged() const;

signals:
    void cellClicked(int x, int y);
    void cellRightClicked(int x, int y);
    void cellMiddleClicked(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int posX, posY;
    bool mine;
    int number;
    bool revealed;
    bool flagged;
    QIcon mineIcon;
    QIcon flagIcon;
    QIcon emptyIcon;
    QIcon cellIcon; // Добавляем иконку не нажатой ячейки
    void paintEvent(QPaintEvent *event);
    QColor getColorForNumber(int number);
};

#endif // MINESWEEPER_GAMECELL_H
