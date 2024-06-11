#pragma once

#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include "GameBoard.h"
#include "NewGameDialog.h"

#include <QAction>
#include <QDir>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(bool debug, QWidget *parent = nullptr);
	~MainWindow();

  private slots:
	void newGame();
	void gameFinished(bool won);
	void toggleDebugMode(bool checked);

  protected:
	void resizeEvent(QResizeEvent *event) override;

  private:
	void createMenus();
	void createToolBar();
	bool debugMode;
	bool gameOver;
	GameBoard *gameBoard;
	QString saveFileName;
	QAction *debugAction;
};

#endif	  // MINESWEEPER_MAINWINDOW_H
