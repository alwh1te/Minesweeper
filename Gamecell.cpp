#include "Gamecell.h"
#include <QMouseEvent>

GameCell::GameCell(int x, int y, QWidget *parent) : QPushButton(parent), posX(x), posY(y), mine(false), number(0), revealed(false) {
    setFixedSize(30, 30); // Размер ячейки для лучшего визуального отображения
}

void GameCell::setMine(bool hasMine) {
    mine = hasMine;
}

bool GameCell::hasMine() const {
    return mine;
}

void GameCell::setNumber(int num) {
    number = num;
    if (number > 0) {
        setText(QString::number(number));
    }
}

int GameCell::getNumber() const {
    return number;
}

void GameCell::reveal() {
    revealed = true;
    setEnabled(false); // Делаем ячейку неактивной
    if (mine) {
        setText("M"); // Показываем мину
    } else if (number > 0) {
        setText(QString::number(number));
    } else {
        setText("");
    }
}

bool GameCell::isRevealed() const {
    return revealed;
}

void GameCell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit cellClicked(posX, posY);
    } else if (event->button() == Qt::RightButton) {
        emit cellRightClicked(posX, posY);
    } else if (event->button() == Qt::MiddleButton) {
        emit cellMiddleClicked(posX, posY);
    }
    QPushButton::mousePressEvent(event);
}
