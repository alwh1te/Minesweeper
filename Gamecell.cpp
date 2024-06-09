#include "Gamecell.h"
#include <QMouseEvent>

GameCell::GameCell(int x, int y, QWidget *parent)
    : QPushButton(parent), posX(x), posY(y), mine(false), number(0), revealed(false), flagged(false) {
    setFixedSize(30, 30);
}

void GameCell::setMine(bool hasMine) {
    mine = hasMine;
}

bool GameCell::hasMine() const {
    return mine;
}

void GameCell::setNumber(int num) {
    number = num;
}

int GameCell::getNumber() const {
    return number;
}

void GameCell::reveal() {
    if (revealed) return;
    revealed = true;
    if (mine) {
        setText("X");
        setStyleSheet("background-color: red");
    } else if (number > 0) {
        setText(QString::number(number));
        setStyleSheet("background-color: grey");
    }
    setEnabled(false);
}

bool GameCell::isRevealed() const {
    return revealed;
}

void GameCell::toggleFlag() {
    if (revealed) return;
    flagged = !flagged;
    if (flagged) {
        setText("F");
        setStyleSheet("background-color: lightcoral");
    } else {
        setText("");
        setStyleSheet("");
    }
}

void GameCell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit cellClicked(posX, posY);
    } else if (event->button() == Qt::RightButton) {
        emit cellRightClicked(posX, posY);
    } else if (event->button() == Qt::MiddleButton) {
        emit cellMiddleClicked(posX, posY);
    }
}
