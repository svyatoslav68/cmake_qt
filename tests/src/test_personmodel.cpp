#include <iostream>
#include <QApplication>
#include "cl_parametrs.hpp"
#include "dialog_edit_file.hpp"
#include "person_model.hpp"

clParametrs appParametrs; // Параметры приложения - глобальная переменная
int main(int argc, char **argv)
{
	appParametrs.setArgs(argc, argv);
	appParametrs.loadFromFile();
	QApplication app(argc, argv);
	DialogEditTxtFile *mainDialog = new DialogEditTxtFile(DialogEditTxtFile::TXT);
	mainDialog->show();
	return app.exec();
}

