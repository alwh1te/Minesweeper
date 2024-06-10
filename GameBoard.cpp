#include "GameBoard.h"
#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>

GameBoard::GameBoard(QWidget *parent) : QWidget(parent), boardWidth(0), boardHeight(0), mineCount(0), flaggedMines(0), firstClick(true) {
    setMinimumSize(500, 500);
}

GameBoard::~GameBoard() {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
//            if (cells[i][j])
            delete cells[i][j];
        }
    }
}

#include <QSettings>

void GameBoard::saveGameState(const QString &fileName) {
    QSettings settings(fileName, QSettings::IniFormat);

    settings.beginGroup("GameBoard");
    settings.setValue("Width", boardWidth);
    settings.setValue("Height", boardHeight);
    settings.setValue("Mines", mineCount);
    settings.setValue("FlaggedMines", flaggedMines);
    settings.setValue("FirstClick", firstClick);
    settings.endGroup();

    settings.beginGroup("Cells");
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            QString cellKey = QString("Cell_%1_%2").arg(i).arg(j);
            settings.setValue(cellKey + "/Mine", cells[i][j]->hasMine());
            settings.setValue(cellKey + "/Number", cells[i][j]->getNumber());
            settings.setValue(cellKey + "/Revealed", cells[i][j]->isRevealed());
            settings.setValue(cellKey + "/Flagged", cells[i][j]->isFlagged());
        }
    }
    settings.endGroup();
}

void GameBoard::loadGameState(const QString &fileName) {
    QSettings settings(fileName, QSettings::IniFormat);

    settings.beginGroup("GameBoard");
    int width = settings.value("Width").toInt();
    int height = settings.value("Height").toInt();
    int mines = settings.value("Mines").toInt();
    flaggedMines = settings.value("FlaggedMines").toInt();
    firstClick = settings.value("FirstClick").toBool();
    settings.endGroup();

    setupBoard(width, height, mines);

    settings.beginGroup("Cells");
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            QString cellKey = QString("Cell_%1_%2").arg(i).arg(j);
            cells[i][j]->setMine(settings.value(cellKey + "/Mine").toBool());
            cells[i][j]->setNumber(settings.value(cellKey + "/Number").toInt());
            if (settings.value(cellKey + "/Revealed").toBool()) {
                cells[i][j]->reveal();
            }
            if (settings.value(cellKey + "/Flagged").toBool()) {
                cells[i][j]->toggleFlag();
            }
        }
    }
    settings.endGroup();
}


void GameBoard::setupBoard(int width, int height, int mines) {
    boardWidth = width;
    boardHeight = height;
    mineCount = mines;
    flaggedMines = 0;
    firstClick = true;

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(1);
    cells.resize(width);
    for (int i = 0; i < width; ++i) {
        cells[i].resize(height);
        for (int j = 0; j < height; ++j) {
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
    int cellSize = qMin(width() / boardWidth, height() / boardHeight);

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            cells[i][j]->setFixedSize(cellSize, cellSize);
        }
    }
}

void GameBoard::placeMines(int firstX, int firstY) {
    int placedMines = 0;
    while (placedMines < mineCount) {
        int x = QRandomGenerator::global()->bounded(boardWidth);
        int y = QRandomGenerator::global()->bounded(boardHeight);
        if ((x == firstX && y == firstY) || cells[x][y]->hasMine()) {
            continue;
        }
        cells[x][y]->setMine(true);
        placedMines++;
    }
    updateNumbers();
}

void GameBoard::updateNumbers() {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            if (!cells[i][j]->hasMine()) {
                int mineCount = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && cells[nx][ny]->hasMine()) {
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
    if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight || cells[x][y]->isRevealed() || cells[x][y]->isFlagged()) {
        return;
    }
    cells[x][y]->reveal();
    if (cells[x][y]->hasMine()) {
        gameOver(false, x, y);
    } else if (cells[x][y]->getNumber() == 0) {
        revealEmptyCells(x, y);
    }
    checkForWin();
}

void GameBoard::revealEmptyCells(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            revealCell(x + dx, y + dy);
        }
    }
}

void GameBoard::revealAllMines() {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            if (!cells[i][j]->isRevealed()) {
                cells[i][j]->reveal();
            }
        }
    }
}

void GameBoard::gameOver(bool won, int lastX, int lastY) {
    if (lastX != -1 && lastY != -1) {
        cells[lastX][lastY]->setStyleSheet("background-color: lightcoral");
    }
    if (!won) {
        revealAllMines();
    }
    setDisabled(true);
    emit gameOverSignal(won);
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
    if (cells[x][y]->isRevealed()) {
        return;
    }

    cells[x][y]->toggleFlag();
    if (cells[x][y]->isFlagged()) {
        flaggedMines++;
    } else {
        flaggedMines--;
    }
}


void GameBoard::handleCellMiddleClick(int x, int y) {
    qDebug() << "Cell middle-clicked at (" << x << ", " << y << ")";
    if (!cells[x][y]->isRevealed()) return;

    int flaggedCount = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && cells[nx][ny]->isFlagged()) {
                flaggedCount++;
            }
        }
    }
    if (flaggedCount == cells[x][y]->getNumber()) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight)
                    revealCell(x + dx, y + dy);
            }
        }
    } else {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && !cells[nx][ny]->isRevealed() && !cells[nx][ny]->isFlagged()) {
                    //                    if (cells[nx][ny]->styleSheet() == "background-color: yellow") {
                    //                        cells[nx][ny]->setStyleSheet("background-color: yellow");
                    //                    } else {
                    //                        cells[nx][ny]->setStyleSheet("background-color: yellow");
                    //                    }
                    cells[nx][ny]->setStyleSheet("background-color: yellow");
                }
            }
        }
    }
}

void GameBoard::checkForWin() {
    int revealedCount = 0;
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            if (cells[i][j]->isRevealed()) {
                revealedCount++;
            }
        }
    }
    if (revealedCount == (boardWidth * boardHeight) - mineCount) {
        gameOver(true);
    }
}
