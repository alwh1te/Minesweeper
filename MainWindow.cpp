#include "MainWindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QCloseEvent>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(nullptr) {
    createMenus();
    createToolBar();
    autoLoadGame();
}

MainWindow::~MainWindow() {
    delete gameBoard;
}

void MainWindow::createMenus() {
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    QAction *newGameAction = gameMenu->addAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    QAction *saveGameAction = gameMenu->addAction(tr("Save Game"));
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::saveGame);

    QAction *loadGameAction = gameMenu->addAction(tr("Load Game"));
    connect(loadGameAction, &QAction::triggered, this, &MainWindow::loadGame);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Game"));
    QAction *newGameAction = new QAction(tr("New Game"), this);
    toolBar->addAction(newGameAction);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    QAction *saveGameAction = new QAction(tr("Save Game"), this);
    toolBar->addAction(saveGameAction);
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::saveGame);

    QAction *loadGameAction = new QAction(tr("Load Game"), this);
    toolBar->addAction(loadGameAction);
    connect(loadGameAction, &QAction::triggered, this, &MainWindow::loadGame);
}

void MainWindow::newGame() {
    NewGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        int mines = dialog.getMines();

        if (gameBoard) {
            delete gameBoard;
        }

        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->setupBoard(width, height, mines);

        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
    }
}

void MainWindow::gameFinished(bool won) {
    QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
    QMessageBox::information(this, tr("Game Over"), message);
}

void MainWindow::saveGame() {
    if (gameBoard) {
        gameBoard->saveGameState("autosave.ini");
    }
}

void MainWindow::loadGame() {
    if (QFile::exists("autosave.ini")) {
        if (gameBoard) {
            delete gameBoard;
        }

        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->loadGameState("autosave.ini");

        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
    } else {
        QMessageBox::warning(this, tr("Load Game"), tr("No saved game found."));
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveGame();
    QMainWindow::closeEvent(event);
}

void MainWindow::autoLoadGame() {
    if (QFile::exists("autosave.ini")) {
        loadGame();
    } else {
        newGame();
    }
}


void MainWindow::createMenus() {
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    QAction *newGameAction = gameMenu->addAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    QAction *saveGameAction = gameMenu->addAction(tr("Save Game"));
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::saveGame);

    QAction *loadGameAction = gameMenu->addAction(tr("Load Game"));
    connect(loadGameAction, &QAction::triggered, this, &MainWindow::loadGame);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Game"));
    QAction *newGameAction = new QAction(tr("New Game"), this);
    toolBar->addAction(newGameAction);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    QAction *saveGameAction = new QAction(tr("Save Game"), this);
    toolBar->addAction(saveGameAction);
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::saveGame);

    QAction *loadGameAction = new QAction(tr("Load Game"), this);
    toolBar->addAction(loadGameAction);
    connect(loadGameAction, &QAction::triggered, this, &MainWindow::loadGame);
}



void MainWindow::newGame() {
    NewGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        int mines = dialog.getMines();

        if (gameBoard) {
            delete gameBoard;
        }

        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->setupBoard(width, height, mines);

        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
    }
}


void MainWindow::gameFinished(bool won) {
    QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
    QMessageBox::information(this, tr("Game Over"), message);
//    newGame();
    // TODO пофиксить баг при выигрыше с первого хода
}


