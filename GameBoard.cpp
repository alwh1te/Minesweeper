#include "GameBoard.h"


GameBoard::GameBoard(QWidget *parent) : QWidget(parent), boardWidth(0), boardHeight(0), mineCount(0), flaggedMines(0), firstClick(true), elapsedTime(0) {
    setMinimumSize(500, 500);
    timer = new QTimer(this);
    timerLabel = new QLabel(this);
    mineCounterLabel = new QLabel(this);
    connect(timer, &QTimer::timeout, this, &GameBoard::updateTimer);
    updateMineCounter();
    timerLabel->setText("Time: 0");
}

GameBoard::~GameBoard() {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            delete cells[i][j];
        }
    }
}

void GameBoard::saveGameState(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << boardWidth << " " << boardHeight << " " << mineCount << " " << flaggedMines << " " << (firstClick ? 1 : 0) << "\n";
        for (int i = 0; i < boardWidth; ++i) {
            for (int j = 0; j < boardHeight; ++j) {
                out << (cells[i][j]->hasMine() ? 1 : 0) << " "
                    << cells[i][j]->getNumber() << " "
                    << (cells[i][j]->isRevealed() ? 1 : 0) << " "
                    << (cells[i][j]->isFlagged() ? 1 : 0) << "\n";
            }
        }
    }
}

void GameBoard::loadGameState(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        int firstClickInt;
        in >> boardWidth >> boardHeight >> mineCount >> flaggedMines >> firstClickInt;
        firstClick = (firstClickInt == 1);
        setupBoard(boardWidth, boardHeight, mineCount);

        for (int i = 0; i < boardWidth; ++i) {
            for (int j = 0; j < boardHeight; ++j) {
                int hasMineInt, isRevealedInt, isFlaggedInt, number;
                in >> hasMineInt >> number >> isRevealedInt >> isFlaggedInt;
                cells[i][j]->setMine(hasMineInt == 1);
                cells[i][j]->setNumber(number);
                if (isRevealedInt == 1) cells[i][j]->reveal();
                if (isFlaggedInt == 1) cells[i][j]->toggleFlag();
            }
        }
    }
}

void GameBoard::setupBoard(int width, int height, int mines) {
    boardWidth = width;
    boardHeight = height;
    mineCount = mines;
    flaggedMines = 0;
    firstClick = true;
    elapsedTime = 0;
    timer->stop();
    timerLabel->setText("Time: 0");
    updateMineCounter();

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
            connect(cells[i][j], &GameCell::middleButtonPressed, this, &GameBoard::handleCellMiddlePress);
            connect(cells[i][j], &GameCell::middleButtonReleased, this, &GameBoard::handleCellMiddleRelease);
        }
    }

    layout->addWidget(timerLabel, width, 0, 1, height / 2);
    layout->addWidget(mineCounterLabel, width, height / 2, 1, height / 2);
    setLayout(layout);
}

void GameBoard::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int cellSize = qMin(width() / boardWidth, height() / boardHeight);

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            cells[i][j]->setFixedSize(cellSize, cellSize);
            cells[i][j]->setIconSize(QSize(cellSize, cellSize));
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

void GameBoard::revealAllMines(bool reveal) {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            if (cells[i][j]->hasMine()) {
                if (reveal) {
                    cells[i][j]->setIcon(QIcon(":/icons/bomb.png"));
                } else if (!cells[i][j]->isRevealed()) {
                    cells[i][j]->setIcon(QIcon(":/icons/cell.png"));
                }
            }
        }
    }
}


void GameBoard::gameOver(bool won, int lastX, int lastY) {
    if (lastX != -1 && lastY != -1) {
        cells[lastX][lastY]->setStyleSheet("background-color: lightcoral");
    }
    if (!won) {
        revealAllMines(false);
    }
    setDisabled(true);
    stopTimer();
    emit gameOverSignal(won);
}

void GameBoard::handleCellClick(int x, int y) {
    qDebug() << "Cell clicked at (" << x << ", " << y << ")";
    if (firstClick) {
        placeMines(x, y);
        startTimer();
        firstClick = false;
    }
    revealCell(x, y);
    updateMineCounter();
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
    updateMineCounter();
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
//                    cells[nx][ny]->setStyleSheet("background-color: rgb(64, 64, 64);");
                }
            }
        }
    }
}

void GameBoard::handleCellMiddlePress(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && !cells[nx][ny]->isRevealed() && !cells[nx][ny]->isFlagged()) {
                //                    cells[nx][ny]->setStyleSheet("background-color: rgb(64, 64, 64);");
                cells[nx][ny]->setTemporaryIcon(true);
            }
        }
    }
}

void GameBoard::handleCellMiddleRelease(int x, int y) {
for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx > 0 && nx < boardWidth && ny > 0 && ny < boardHeight && !cells[nx][ny]->isRevealed() && !cells[nx][ny]->isFlagged()) {
            cells[nx][ny]->setTemporaryIcon(false);
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


void GameBoard::updateTimer() {
    timerLabel->setText("Time: " + QString::number(++elapsedTime));
}

void GameBoard::updateMineCounter() {
    mineCounterLabel->setText("Mines: " + QString::number(mineCount - flaggedMines));
}

void GameBoard::startTimer() {
    timer->start(1000);
}

void GameBoard::stopTimer() {
    timer->stop();
}