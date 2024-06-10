#include "MainWindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), gameBoard(nullptr) {
    createMenus();
    createToolBar();

    gameBoard = new GameBoard(this);
    setCentralWidget(gameBoard);

    newGame();
}

MainWindow::~MainWindow() {
}

void MainWindow::createMenus() {
    gameMenu = menuBar()->addMenu(tr("&Game"));

    newGameAction = new QAction(tr("&New Game"), this);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    gameMenu->addAction(newGameAction);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Game Controls"));
    toolBar->addAction(newGameAction);
}

void MainWindow::newGame() {
    NewGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        int mines = dialog.getMines();

        if (gameBoard) {
            centralWidget()->setParent(nullptr);
            delete gameBoard;
        }

        gameBoard = new GameBoard(this);
        setCentralWidget(gameBoard);
        gameBoard->setupBoard(width, height, mines);

        // Подключаем слот завершения игры к игровому полю
        connect(gameBoard, &GameBoard::gameOverSignal, this, &MainWindow::gameFinished);
    }
}

void MainWindow::gameFinished(bool won) {
    QString message = won ? tr("Congratulations, you won!") : tr("Game over, you lost!");
    QMessageBox::information(this, tr("Game Over"), message);
    newGame();
}
