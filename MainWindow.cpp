#include "MainWindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(nullptr) {
    createMenus();
    createToolBar();
    newGame();
}

MainWindow::~MainWindow() {
    delete gameBoard;
}

void MainWindow::saveGame() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Game"), "", tr("INI Files (*.ini)"));
    if (!fileName.isEmpty()) {
        gameBoard->saveGameState(fileName);
    }
}

void MainWindow::loadGame() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Game"), "", tr("INI Files (*.ini)"));
    if (!fileName.isEmpty()) {
        if (gameBoard) {
            delete gameBoard;
        }
        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->loadGameState(fileName);

        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
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


