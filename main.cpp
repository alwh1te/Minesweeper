#include <QApplication>
#include <QPushButton>
#include "Minesweeper.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow game;
    game.show();
    return app.exec();
}
