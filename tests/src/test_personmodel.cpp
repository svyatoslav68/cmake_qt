#include <iostream>
#include <QApplication>
#include "cl_parametrs.hpp"
#include "dialog_edit_file.hpp"
#include "person_model.hpp"

clParametrs appParametrs; // Параметры приложения - глобальная переменная
int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	DialogEditTxtFile *mainDialog = new DialogEditTxtFile();
	mainDialog->show();
	return app.exec();
}

