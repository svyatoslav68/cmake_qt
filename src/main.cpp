/***********************************************************************************
***************** Файл содержащий main() приложения с GUI qt ***********************
***********************************************************************************/

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "main_window.hpp"
#include "cl_parametrs.hpp"

clParametrs appParametrs; // Параметры приложения - глобальная переменная

int main(int argc, char **argv){
	appParametrs.setArgs(argc, argv);
	appParametrs.loadFromFile();
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}

