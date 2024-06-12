#include "MainWindow.h"

MainWindow::MainWindow(bool debug, QWidget *parent) :
	QMainWindow(parent), gameBoard(nullptr), saveFileName(QDir::currentPath() + "/autosave.ini")
{
	debugMode = debug;
	createMenus();
	createToolBar();
	QFile file(saveFileName);
	if (file.exists())
	{
		gameBoard = new GameBoard(this);
		setCentralWidget(gameBoard);
		gameBoard->loadGameState(saveFileName);
		connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
		file.remove();
	}
	else
	{
		newGame();
	}
}

MainWindow::~MainWindow()
{
	if (gameBoard && !gameOver)
	{
		gameBoard->saveGameState(saveFileName);
	}
	delete gameBoard;
}

void MainWindow::createMenus()
{
	QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
	QAction *newGameAction = gameMenu->addAction(tr("New Game"));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

	if (debugMode)
	{
		debugAction = gameMenu->addAction(tr("Toggle Debug Mode"));
		debugAction->setCheckable(true);
		connect(debugAction, &QAction::toggled, this, &MainWindow::toggleDebugMode);
	}

	QMenu *languageMenu = menuBar()->addMenu(tr("Language"));
	switchToEnglishAction = languageMenu->addAction(tr("English"));
	switchToRussianAction = languageMenu->addAction(tr("Русский"));

	connect(switchToEnglishAction, &QAction::triggered, this, &MainWindow::switchToEnglish);
	connect(switchToRussianAction, &QAction::triggered, this, &MainWindow::switchToRussian);
}

void MainWindow::createToolBar()
{
	updateToolBar();
}

void MainWindow::updateToolBar()
{
	QToolBar *toolBar = findChild<QToolBar*>("gameToolBar");
	if (toolBar)
	{
		removeToolBar(toolBar);
		delete toolBar;
	}

	toolBar = addToolBar(tr("Game"));
	toolBar->setObjectName("gameToolBar");
	QAction *newGameAction = new QAction(tr("New Game"), this);
	toolBar->addAction(newGameAction);
	connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

	if (debugMode)
	{
		debugAction = toolBar->addAction(tr("Toggle Debug Mode"));
		debugAction->setCheckable(true);
		connect(debugAction, &QAction::toggled, this, &MainWindow::toggleDebugMode);
	}
}

void MainWindow::toggleDebugMode(bool checked)
{
	if (gameBoard)
	{
		gameBoard->revealAllMines(checked);
	}
}

void MainWindow::newGame()
{
	NewGameDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		gameOver = false;
		int width = dialog.getWidth();
		int height = dialog.getHeight();
		int mines = dialog.getMines();

		if (gameBoard)
		{
			delete gameBoard;
		}

		gameBoard = new GameBoard(this);
		setCentralWidget(gameBoard);
		gameBoard->setupBoard(width, height, mines, 0, 0);

		connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
	}
}

void MainWindow::gameFinished(bool won)
{
	if (!gameOver)
	{
		QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
		QMessageBox::information(this, tr("Game Over"), message);
		gameOver = true;
	}
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	QSize newSize = event->size();
	int side = qMin(newSize.width(), newSize.height());
	resize(side, side);
}

void MainWindow::switchToEnglish()
{
	qApp->removeTranslator(&translator);
	translator.load(":/translations/minesweeper_en.qm");
	qApp->installTranslator(&translator);
	translateUi();
}

void MainWindow::switchToRussian()
{
	qApp->removeTranslator(&translator);
	translator.load(":/translations/minesweeper_ru.qm");
	qApp->installTranslator(&translator);
	translateUi();
}

void MainWindow::translateUi()
{
	menuBar()->clear();
	createMenus();
	updateToolBar();
}
