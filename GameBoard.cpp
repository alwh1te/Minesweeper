#include "GameBoard.h"

GameBoard::GameBoard(QWidget *parent) :
	QWidget(parent), boardWidth(0), boardHeight(0), mineCount(0), flaggedMines(0), firstClick(true), elapsedTime(0)
{
	setMinimumSize(500, 500);
	timer = new QTimer(this);
	timerLabel = new QLabel(this);
	mineCounterLabel = new QLabel(this);
	QLabel *note = new QLabel(this);
	note->setText(tr("Use mouse 5 and mouse 4 buttons like analog for middle mouse button"));
	note->adjustSize();
	connect(timer, &QTimer::timeout, this, &GameBoard::updateTimer);
	updateMineCounter();
	timerLabel->setText(tr("Time: ") + QString::number(0));
}

GameBoard::~GameBoard()
{
	for (int i = 0; i < boardWidth; ++i)
	{
		for (int j = 0; j < boardHeight; ++j)
		{
			delete cells[i][j];
		}
	}
}

void GameBoard::saveGameState(const QString &fileName)
{
	QSettings settings(fileName, QSettings::IniFormat);

	settings.beginGroup("GameState");
	settings.setValue("Width", boardWidth);
	settings.setValue("Height", boardHeight);
	settings.setValue("MineCount", mineCount);
	settings.setValue("FlaggedMines", flaggedMines);
	settings.setValue("ElapsedTime", elapsedTime);
	settings.endGroup();

	settings.beginGroup("Cells");
	for (int x = 0; x < boardWidth; ++x)
	{
		for (int y = 0; y < boardHeight; ++y)
		{
			QString key = QString("Cell_%1_%2").arg(x).arg(y);
			settings.setValue(key + "_Mine", cells[x][y]->hasMine());
			settings.setValue(key + "_Number", cells[x][y]->getNumber());
			settings.setValue(key + "_Revealed", cells[x][y]->isRevealed());
			settings.setValue(key + "_Flagged", cells[x][y]->isFlagged());
		}
	}
	settings.endGroup();
}

void GameBoard::loadGameState(const QString &fileName)
{
	QSettings settings(fileName, QSettings::IniFormat);

	settings.beginGroup("GameState");
	bool valid = true;

	boardWidth = settings.value("Width", -1).toInt();
	boardHeight = settings.value("Height", -1).toInt();
	mineCount = settings.value("MineCount", -1).toInt();
	flaggedMines = settings.value("FlaggedMines", -1).toInt();
	elapsedTime = settings.value("ElapsedTime", -1).toInt();

	if (boardWidth <= 0 || boardHeight <= 0 || mineCount < 0 || flaggedMines < 0 || elapsedTime < 0)
	{
		valid = false;
	}

	settings.endGroup();

	settings.beginGroup("Cells");

	for (int x = 0; x < boardWidth; ++x)
	{
		for (int y = 0; y < boardHeight; ++y)
		{
			QString key = QString("Cell_%1_%2").arg(x).arg(y);
			if (!settings.contains(key + "_Mine") || !settings.contains(key + "_Number") ||
				!settings.contains(key + "_Revealed") || !settings.contains(key + "_Flagged"))
			{
				valid = false;
				goto label;
			}
		}
	}
	label:

	settings.endGroup();

	if (!valid)
	{
		QMessageBox::warning(this, tr("Invalid autosave data"), tr("Invalid save file. Starting a new game with default settings."));

		setupBoard(10, 10, 10, 0, 0);
		return;
	}

	// Load the game state if the save file is valid
	settings.beginGroup("GameState");
	setupBoard(boardWidth, boardHeight, mineCount, elapsedTime, flaggedMines);
	settings.endGroup();

	firstClick = false;
	settings.beginGroup("Cells");
	for (int x = 0; x < boardWidth; ++x)
	{
		for (int y = 0; y < boardHeight; ++y)
		{
			QString key = QString("Cell_%1_%2").arg(x).arg(y);
			bool mine = settings.value(key + "_Mine").toBool();
			int number = settings.value(key + "_Number").toInt();
			bool revealed = settings.value(key + "_Revealed").toBool();
			bool flagged = settings.value(key + "_Flagged").toBool();
			qDebug() << "Cell_" << x << "_" << y << " Mine: " << mine << " Number: " << number;
			cells[x][y]->setMine(mine);
			cells[x][y]->setNumber(number);
			if (revealed)
			{
				cells[x][y]->reveal();
			}
			if (flagged)
			{
				cells[x][y]->toggleFlag();
			}
		}
	}
	settings.endGroup();

	startTimer();
}


void GameBoard::setupBoard(int width, int height, int mines, int time, int flMines)
{
	boardWidth = width;
	boardHeight = height;
	mineCount = mines;
	flaggedMines = flMines;
	elapsedTime = time;
	firstClick = true;
	timer->stop();
	timerLabel->setText(tr("Time: ") + QString::number(elapsedTime));
	updateMineCounter();

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QHBoxLayout *infoLayout = new QHBoxLayout();

	infoLayout->addWidget(timerLabel);
	infoLayout->addWidget(mineCounterLabel);
	mainLayout->addLayout(infoLayout);

	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->setSpacing(1);
	cells.resize(width);
	for (int i = 0; i < width; ++i)
	{
		cells[i].resize(height);
		for (int j = 0; j < height; ++j)
		{
			cells[i][j] = new GameCell(i, j, this);
			gridLayout->addWidget(cells[i][j], i, j);
			connect(cells[i][j], &GameCell::cellClicked, this, &GameBoard::handleCellClick);
			connect(cells[i][j], &GameCell::cellRightClicked, this, &GameBoard::handleCellRightClick);
			connect(cells[i][j], &GameCell::middleButtonPressed, this, &GameBoard::handleCellMiddlePress);
			connect(cells[i][j], &GameCell::middleButtonReleased, this, &GameBoard::handleCellMiddleRelease);
		}
	}

	mainLayout->addLayout(gridLayout);
	setLayout(mainLayout);
}

void GameBoard::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	int cellSize = qMin(width() / boardWidth, height() / boardHeight);

	for (int i = 0; i < boardWidth; ++i)
	{
		for (int j = 0; j < boardHeight; ++j)
		{
			cells[i][j]->setFixedSize(cellSize, cellSize);
			cells[i][j]->setIconSize(QSize(cellSize, cellSize));
		}
	}

	int side = qMin(boardWidth * cellSize, boardHeight * cellSize);
	resize(side, side);
}

void GameBoard::placeMines(int firstX, int firstY)
{
	int placedMines = 0;
	while (placedMines < mineCount)
	{
		int x = QRandomGenerator::global()->bounded(boardWidth);
		int y = QRandomGenerator::global()->bounded(boardHeight);
		if ((x == firstX && y == firstY) || cells[x][y]->hasMine())
		{
			continue;
		}
		cells[x][y]->setMine(true);
		placedMines++;
	}
	updateNumbers();
}

void GameBoard::updateNumbers()
{
	for (int i = 0; i < boardWidth; ++i)
	{
		for (int j = 0; j < boardHeight; ++j)
		{
			if (!cells[i][j]->hasMine())
			{
				int mineCount = 0;
				for (int dx = -1; dx <= 1; ++dx)
				{
					for (int dy = -1; dy <= 1; ++dy)
					{
						int nx = i + dx;
						int ny = j + dy;
						if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && cells[nx][ny]->hasMine())
						{
							mineCount++;
						}
					}
				}
				cells[i][j]->setNumber(mineCount);
			}
		}
	}
}

void GameBoard::revealCell(int x, int y)
{
	updateNumbers();
	if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight || cells[x][y]->isRevealed() || cells[x][y]->isFlagged())
	{
		return;
	}
	cells[x][y]->reveal();
	if (cells[x][y]->hasMine())
	{
		gameOver(false, x, y);
	}
	else if (cells[x][y]->getNumber() == 0)
	{
		revealEmptyCells(x, y);
	}
	checkForWin();
}

void GameBoard::revealEmptyCells(int x, int y)
{
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			revealCell(x + dx, y + dy);
		}
	}
}

void GameBoard::revealAllMines(bool reveal)
{
	for (int i = 0; i < boardWidth; ++i)
	{
		for (int j = 0; j < boardHeight; ++j)
		{
			if (cells[i][j]->hasMine())
			{
				if (reveal)
				{
					cells[i][j]->setIcon(QIcon(":/icons/bomb.png"));
				}
				else if (cells[i][j]->isFlagged())
				{
					cells[i][j]->setIcon(QIcon(":/icons/flag.png"));
				}
				else if (!cells[i][j]->isRevealed())
				{
					cells[i][j]->setIcon(QIcon(":/icons/cell.png"));
				}
			}
		}
	}
}

void GameBoard::gameOver(bool won, int lastX, int lastY)
{
	if (lastX != -1 && lastY != -1)
	{
		cells[lastX][lastY]->setStyleSheet("background-color: lightcoral");
	}
	if (!won)
	{
		revealAllMines(true);
	}
	setDisabled(true);
	stopTimer();
	emit gameOverSignal(won);
}

void GameBoard::handleCellClick(int x, int y)
{
	qDebug() << "Cell clicked at (" << x << ", " << y << ")";
	if (firstClick)
	{
		placeMines(x, y);
		startTimer();
		firstClick = false;
	}
	revealCell(x, y);
	updateMineCounter();
}

void GameBoard::handleCellRightClick(int x, int y)
{
	qDebug() << "Cell right-clicked at (" << x << ", " << y << ")";
	if (cells[x][y]->isRevealed())
	{
		return;
	}

	cells[x][y]->toggleFlag();
	if (cells[x][y]->isFlagged())
	{
		flaggedMines++;
	}
	else
	{
		flaggedMines--;
	}
	updateMineCounter();
}

void GameBoard::handleCellMiddlePress(int x, int y)
{
	if (!cells[x][y]->isRevealed())
		return;

	int flaggedCount = 0;
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			int nx = x + dx;
			int ny = y + dy;
			if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && cells[nx][ny]->isFlagged())
			{
				flaggedCount++;
			}
		}
	}
	if (flaggedCount == cells[x][y]->getNumber())
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				int nx = x + dx;
				int ny = y + dy;
				if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight)
				{
					revealCell(x + dx, y + dy);
				}
			}
		}
	}
	else
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				int nx = x + dx;
				int ny = y + dy;
				if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight && !cells[nx][ny]->isRevealed() &&
					!cells[nx][ny]->isFlagged())
				{
					cells[nx][ny]->setTemporaryIcon(true);
				}
			}
		}
	}
}

void GameBoard::handleCellMiddleRelease(int x, int y)
{
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			int nx = x + dx;
			int ny = y + dy;
			if (nx > 0 && nx < boardWidth && ny > 0 && ny < boardHeight && !cells[nx][ny]->isRevealed() &&
				!cells[nx][ny]->isFlagged())
			{
				cells[nx][ny]->setTemporaryIcon(false);
			}
		}
	}
}

void GameBoard::checkForWin()
{
	int revealedCount = 0;
	for (int i = 0; i < boardWidth; ++i)
	{
		for (int j = 0; j < boardHeight; ++j)
		{
			if (cells[i][j]->isRevealed())
			{
				revealedCount++;
			}
		}
	}
	if (revealedCount == (boardWidth * boardHeight) - mineCount)
	{
		gameOver(true);
	}
}

void GameBoard::updateTimer()
{
	timerLabel->setText(tr("Time: ") + QString::number(++elapsedTime));
}

void GameBoard::updateMineCounter()
{
	mineCounterLabel->setText(tr("Mines: ") + QString::number(mineCount - flaggedMines));
}

void GameBoard::startTimer()
{
	timer->start(1000);
}

void GameBoard::stopTimer()
{
	timer->stop();
}
