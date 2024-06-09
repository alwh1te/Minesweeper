#include "Gameboard.h"
#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>

GameBoard::GameBoard(QWidget *parent) : QWidget(parent), boardSize(0), mineCount(0), firstClick(true) {
    setMinimumSize(300, 300);// Устанавливаем минимальный размер для игрового поля
}

void GameBoard::setupBoard(int size, int mines) {
    boardSize = size;
    mineCount = mines;
    firstClick = true;

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(1);// Устанавливаем расстояние между ячейками
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

void GameBoard::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int cellSize = qMin(width() / boardSize, height() / boardSize);

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            cells[i][j]->setFixedSize(cellSize, cellSize);
        }
    }
}

void GameBoard::placeMines(int firstX, int firstY) {
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
    updateNumbers();
}

void GameBoard::updateNumbers() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (!cells[i][j]->hasMine()) {
                int mineCount = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize && cells[nx][ny]->hasMine()) {
                            mineCount++;
                        }
                    }
                }
                cells[i][j]->setNumber(mineCount);
            }
        }
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
    checkForWin();// Проверка победы после каждого открытия ячейки
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
    setDisabled(true);// Отключаем игровое поле после окончания игры
}

void GameBoard::handleCellClick(int x, int y) {
    qDebug() << "Cell clicked at (" << x << ", " << y << ")";
    if (firstClick) {
        placeMines(x, y);
        firstClick = false;
    }
    revealCell(x, y);
}

void GameBoard::handleCellRightClick(int x, int y) {
    qDebug() << "Cell right-clicked at (" << x << ", " << y << ")";
    cells[x][y]->toggleFlag();
}

void GameBoard::handleCellMiddleClick(int x, int y) {
    qDebug() << "Cell middle-clicked at (" << x << ", " << y << ")";
    if (!cells[x][y]->isRevealed()) return;

    int flaggedCount = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize && cells[nx][ny]->text() == "F") {
                flaggedCount++;
            }
        }
    }

    if (flaggedCount == cells[x][y]->getNumber()) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                revealCell(x + dx, y + dy);
            }
        }
    } else {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize && !cells[nx][ny]->isRevealed()) {
                    cells[nx][ny]->setStyleSheet("background-color: yellow");
                }
            }
        }
    }
}

void GameBoard::checkForWin() {
    int revealedCount = 0;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (cells[i][j]->isRevealed()) {
                revealedCount++;
            }
        }
    }
    if (revealedCount == (boardSize * boardSize) - mineCount) {
        gameOver(true);
    }
}
