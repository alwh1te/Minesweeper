#include "GameCell.h"
#include <QFont>

GameCell::GameCell(int x, int y, QWidget *parent)
    : QPushButton(parent), posX(x), posY(y), mine(false), number(0), revealed(false), flagged(false) {
    setFixedSize(30, 30);
//    setStyleSheet("background-color: blue");
    mineIcon = QIcon(":/icons/mine.png");
    flagIcon = QIcon(":/icons/flag.png");
    setIconSize(QSize(30, 30));
    setFont(QFont("Arial", 14, QFont::Bold));
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
    setStyleSheet("background-color: grey");
    if (mine) {
        setIcon(mineIcon);
    } else if (number > 0) {
        setText(QString::number(number));
    }
}

bool GameCell::isRevealed() const {
    return revealed;
}

void GameCell::toggleFlag() {
    if (revealed) return;
    flagged = !flagged;
    if (flagged) {
        setIcon(flagIcon);
    } else {
        setIcon(QIcon());
        setText("");
    }
}

bool GameCell::isFlagged() const {
    return flagged;
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
