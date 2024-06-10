#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(new GameBoard(this)) {
    setCentralWidget(gameBoard);
    createMenus();
    createToolBar();  // Создаем тулбар
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

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar(tr("Game"));
    QAction *newGameAction = toolBar->addAction(tr("New Game"));
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
}

void MainWindow::newGame() {
    NewGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        int mines = dialog.getMines();
        gameBoard->setupBoard(width, height, mines);
    }
}
