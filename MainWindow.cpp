#include "MainWindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(nullptr), saveFileName(QDir::homePath() + "/minesweeper_save.ini") {
    createMenus();
    createToolBar();

    QFile file(saveFileName);
    if (file.exists()) {
        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->loadGameState(saveFileName);
        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
    } else {
        newGame();
    }
}

MainWindow::~MainWindow() {
    if (gameBoard) {
        gameBoard->saveGameState(saveFileName);
    }
    delete gameBoard;
}

void MainWindow::createMenus() {
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    QAction *newGameAction = gameMenu->addAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Game"));
    QAction *newGameAction = new QAction(tr("New Game"), this);
    toolBar->addAction(newGameAction);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
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
