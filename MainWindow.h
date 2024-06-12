#pragma once

#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H

#include "GameBoard.h"
#include "NewGameDialog.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QTranslator>

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
	void switchToEnglish();
	void switchToRussian();

  protected:
	void resizeEvent(QResizeEvent *event) override;

  private:
	void createMenus();
	void createToolBar();
	void translateUi();
	bool debugMode;
	bool gameOver;
	GameBoard *gameBoard;
	QString saveFileName;
	QAction *debugAction;
	QAction *switchToEnglishAction;
	QAction *switchToRussianAction;
	QTranslator translator;
	void updateToolBar();
};

#endif	  // MINESWEEPER_MAINWINDOW_H
