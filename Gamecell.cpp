//
// Created by Alexander on 09.06.2024.
//
#include "gamecell.h"
#include <QMouseEvent>

GameCell::GameCell(int x, int y, QWidget *parent)
    : QPushButton(parent), posX(x), posY(y), mine(false) {
    setFixedSize(40, 40);
}

void GameCell::reveal() {
    // Открытие клетки и отображение содержимого
}

bool GameCell::hasMine() const {
    return mine;
}

void GameCell::setMine(bool hasMine) {
    mine = hasMine;
}

void GameCell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(posX, posY);
    }
    // Обработка правого и среднего клика для установки флага и быстрого открытия клеток
}
