#include "Gamecell.h"
#include <QMouseEvent>

GameCell::GameCell(int x, int y, QWidget *parent)
    : QPushButton(parent), posX(x), posY(y), mine(false), revealed(false), number(0) {
    setFixedSize(40, 40);
}

void GameCell::reveal() {
    revealed = true;
    setEnabled(false);
    if (mine) {
        setText("M");
    } else if (number > 0) {
        setText(QString::number(number));
    } else {
        setText("");
    }
}

bool GameCell::hasMine() const {
    return mine;
}

void GameCell::setMine(bool hasMine) {
    mine = hasMine;
}

bool GameCell::isRevealed() const {
    return revealed;
}

void GameCell::setNumber(int num) {
    number = num;
}

int GameCell::getNumber() const {
    return number;
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
