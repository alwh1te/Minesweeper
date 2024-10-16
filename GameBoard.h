#pragma once

#ifndef MINESWEEPER_GAMEBOARD_H
#define MININESWEEPER_GAMEBOARD_H

#include "GameCell.h"

#include <QCheckBox>
#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
#include <QTextStream>
#include <QTimer>
#include <QVector>
#include <QWidget>

class GameBoard : public QWidget
{
	Q_OBJECT

  public:
	explicit GameBoard(QWidget *parent = nullptr);
	~GameBoard();
	void setupBoard(int width, int height, int mines, int time, int flMines);
	void saveGameState(const QString &fileName);
	void loadGameState(const QString &fileName);
	void revealAllMines(bool reveal);

  protected:
	void resizeEvent(QResizeEvent *event) override;

  private:
	QVector< QVector< GameCell * > > cells;
	int boardWidth;
	int boardHeight;
	int mineCount;
	int flaggedMines;
	bool firstClick;
	QTimer *timer;
	QLabel *timerLabel;
	QLabel *mineCounterLabel;
	int elapsedTime;
	void placeMines(int firstX, int firstY);
	void revealCell(int x, int y);
	void revealEmptyCells(int x, int y);
	void checkForWin();
	void updateNumbers();
	void gameOver(bool won, int lastX = -1, int lastY = -1);
	void updateMineCounter();
	void startTimer();
	void stopTimer();

  signals:
	void gameOverSignal(bool won);

  private slots:
	void handleCellClick(int x, int y);
	void handleCellRightClick(int x, int y);
	void handleCellMiddlePress(int x, int y);
	void handleCellMiddleRelease(int x, int y);
	void updateTimer();
};

#endif	  // MINESWEEPER_GAMEBOARD_H
