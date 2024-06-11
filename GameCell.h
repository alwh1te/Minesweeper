#pragma once

#ifndef MINESWEEPER_GAMECELL_H
#define MINESWEEPER_GAMECELL_H

#include <QFont>
#include <QIcon>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>

class GameCell : public QPushButton
{
	Q_OBJECT

  public:
	GameCell(int x, int y, QWidget *parent = nullptr);
	void setMine(bool hasMine);
	bool hasMine() const;
	void setNumber(int number);
	int getNumber() const;
	void reveal();
	bool isRevealed() const;
	void toggleFlag();
	bool isFlagged() const;
	void setTemporaryIcon(bool temporary);
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

  signals:
	void middleButtonPressed(int x, int y);
	void middleButtonReleased(int x, int y);
	void cellClicked(int x, int y);
	void cellRightClicked(int x, int y);

  private:
	int posX, posY;
	bool mine;
	int number;
	bool revealed;
	bool flagged;
	bool temporaryIcon;
	QIcon mineIcon;
	QIcon flagIcon;
	QIcon emptyIcon;
	QIcon cellIcon;
	void paintEvent(QPaintEvent *event);
	QColor getColorForNumber(int number);
};

#endif	  // MINESWEEPER_GAMECELL_H
