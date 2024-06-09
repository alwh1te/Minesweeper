#include "Gameboard.h"
#include <QGridLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QInputDialog>

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
            connect(cells[i][j], &GameCell::cellClicked, this, &GameBoard::handleCellClick);
            connect(cells[i][j], &GameCell::cellRightClicked, this, &GameBoard::handleCellRightClick);
            connect(cells[i][j], &GameCell::cellMiddleClicked, this, &GameBoard::handleCellMiddleClick);
        }
    }
    setLayout(layout);
}

void GameBoard::placeMines(int firstX, int firstY) {
    // Расстановка мин с учетом того, что первая открытая клетка не может быть миной
    int placedMines = 0;
    while (placedMines < mineCount) {
        int x = QRandomGenerator::global()->bounded(boardSize);
        int y = QRandomGenerator::global()->bounded(boardSize);
        if ((x == firstX && y == firstY) || cells[x][y]->hasMine()) {
            continue;
        }
        cells[x][y]->setMine(true);
        placedMines++;
    }
}

void GameBoard::revealCell(int x, int y) {
    if (x < 0 || x >= boardSize || y < 0 || y >= boardSize || cells[x][y]->isRevealed()) {
        return;
    }
    cells[x][y]->reveal();
    if (cells[x][y]->hasMine()) {
        gameOver(false);
    } else if (cells[x][y]->getNumber() == 0) {
        revealEmptyCells(x, y);
    }
}

void GameBoard::revealEmptyCells(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            revealCell(x + dx, y + dy);
        }
    }
}

void GameBoard::gameOver(bool won) {
    QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
    QMessageBox::information(this, tr("Game Over"), message);
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            cells[i][j]->reveal();
        }
    }
}

void GameBoard::handleCellClick(int x, int y) {
    if (!cells[x][y]->isRevealed()) {
        if (cells[x][y]->hasMine()) {
            gameOver(false);
        } else {
            revealCell(x, y);
        }
    }
}

void GameBoard::handleCellRightClick(int x, int y) {
    if (!cells[x][y]->isRevealed()) {
        // Установка/снятие флажка на клетке
        // Например, изменение текста или иконки клетки
        if (cells[x][y]->text() == "F") {
            cells[x][y]->setText("");
        } else {
            cells[x][y]->setText("F");
        }
    }
}

void GameBoard::handleCellMiddleClick(int x, int y) {
    if (cells[x][y]->isRevealed() && cells[x][y]->getNumber() > 0) {
        int flagsAround = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (x + dx >= 0 && x + dx < boardSize && y + dy >= 0 && y + dy < boardSize) {
                    if (cells[x + dx][y + dy]->text() == "F") {
                        flagsAround++;
                    }
                }
            }
        }
        if (flagsAround == cells[x][y]->getNumber()) {
            revealEmptyCells(x, y);
        } else {
            // Подсветка неоткрытых клеток, если количество флагов не совпадает
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (x + dx >= 0 && x + dx < boardSize && y + dy >= 0 && y + dy < boardSize) {
                        if (!cells[x + dx][y + dy]->isRevealed() && cells[x + dx][y + dy]->text() != "F") {
                            // Пример: изменение цвета фона клетки
                            cells[x + dx][y + dy]->setStyleSheet("background-color: yellow");
                        }
                    }
                }
            }
        }
    }
}

void GameBoard::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // Обработка изменения размера окна для корректного отображения поля
}
