#include "Gamecell.h"
#include <QMouseEvent>

GameCell::GameCell(int x, int y, QWidget *parent)
    : QPushButton(parent), posX(x), posY(y), mine(false), number(0), revealed(false), flagged(false) {
    setFixedSize(30, 30); // Начальный размер ячейки
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
    if (revealed) return;
    revealed = true;
    if (mine) {
        setText("X");
        setStyleSheet("background-color: red"); // Красим клетку в красный цвет при раскрытии мины
    } else if (number > 0) {
        setText(QString::number(number));
        setStyleSheet("background-color: grey"); // Красим клетку в красный цвет при раскрытии мины
    }
    setEnabled(false);
}

bool GameCell::isRevealed() const {
    return revealed;
}

void GameCell::toggleFlag() {
    if (revealed) return;
    flagged = !flagged;
//    if (flagged) {
//        setText("F");
//        setStyleSheet("background-color: lightcoral"); // Красим клетку в красный цвет при отметке флага
//    } else {
//        setText("");
//        setStyleSheet("");
//    }
}

void GameCell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit cellClicked(posX, posY);
    } else if (event->button() == Qt::RightButton) {
        emit cellRightClicked(posX, posY);
    }
}
