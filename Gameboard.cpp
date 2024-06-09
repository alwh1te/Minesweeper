//
// Created by Alexander on 09.06.2024.
//
#include "gameboard.h"
#include <QGridLayout>
#include <QRandomGenerator>
#include <QMessageBox>

GameBoard::GameBoard(QWidget *parent) : QWidget(parent) {}

void GameBoard::setupBoard(int size, int mines) {
    boardSize = size;
    mineCount = mines;

    QGridLayout *layout = new QGridLayout(this);
    cells.resize(size);
    for (int i = 0; i < size; ++i) {
        cells[i].resize(size);
        for (int j = 0; j < size; ++j) {
            cells[i][j] = new GameCell(i, j, this);
            layout->addWidget(cells[i][j], i, j);
            connect(cells[i][j], &GameCell::clicked, this, &GameBoard::handleCellClick);
        }
    }
    setLayout(layout);
}

void GameBoard::placeMines(int firstX, int firstY) {
    // Расстановка мин с учетом того, что первая открытая клетка не может быть миной
}

void GameBoard::revealCell(int x, int y) {
    // Открытие клетки и реализация автоматического открытия соседних клеток
}

void GameBoard::gameOver(bool won) {
    QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
    QMessageBox::information(this, tr("Game Over"), message);
}

void GameBoard::handleCellClick(int x, int y) {
    // Обработка кликов по клетке
}

void GameBoard::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // Обработка изменения размера окна для корректного отображения поля
}
