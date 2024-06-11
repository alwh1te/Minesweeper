#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	bool debug = false;
	if (argc > 1 && strcmp(argv[1], "dbg") == 0)
	{
		debug = true;
	}
	MainWindow game(debug);
	game.show();
	return app.exec();
}
