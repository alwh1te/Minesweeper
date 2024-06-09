//
// Created by Alexander on 09.06.2024.
//
#include "MainWindow.h"
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(new GameBoard(this)) {
    setCentralWidget(gameBoard);
    createMenus();
    newGame();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction *newGameAction = gameMenu->addAction(tr("&New Game"), this, &MainWindow::newGame, QKeySequence::New);
    newGameAction->setShortcuts(QKeySequence::New);
    gameMenu->addSeparator();
    QAction *exitAction = gameMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAction->setShortcuts(QKeySequence::Quit);
}

void MainWindow::newGame() {
    bool ok;
    int size = QInputDialog::getInt(this, tr("New Game"), tr("Enter board size:"), 10, 2, 100, 1, &ok);
    if (ok) {
        int mines = QInputDialog::getInt(this, tr("New Game"), tr("Enter number of mines:"), 10, 1, size*size-1, 1, &ok);
        if (ok) {
            gameBoard->setupBoard(size, mines);
        }
    }
}
