/**********************************************************************
 ******* Файл реализации диалога редактирования текстового ************
 ******* файла, содержащего график отпусков сотрудников    ************
 *********************************************************************/
#include <string>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include "tholiday.hpp"
#include "tperson.hpp"
#include "person_model.hpp"
#include "dialog_edit_file.hpp"

DialogEditTxtFile::DialogEditTxtFile(QWidget *parent)
{
	setWindowTitle(QString("Содержимое файла %1").arg(name_file.c_str()));
	personModel = new PersonModel(this);
	buSelectFile = new QPushButton("Выбор файла");
	buSelectFile->setObjectName("buSelectFile");
	connect(buSelectFile, &QPushButton::clicked, this, [this](){this->selectFile();});
	buSave = new QPushButton("Сохранить");
	connect(buSave, &QPushButton::clicked, this, [this](){this->saveFile();});
	buOk = new QPushButton("Ok");
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buCancel = new QPushButton("Cancel");
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(buSave);
	bottomLayout->addWidget(buSelectFile);
	bottomLayout->addStretch();
	bottomLayout->addWidget(buOk);
	bottomLayout->addWidget(buCancel);
	viewPersons = new QListView();
	edNewPerson = new QLineEdit();
	buAddPerson = new QPushButton("+");
	buEditPerson = new QPushButton("...");
	buDeletePerson = new QPushButton("-");
	QHBoxLayout *personControlLayout = new QHBoxLayout;
	personControlLayout->addWidget(buAddPerson);
	personControlLayout->addWidget(buEditPerson);
	personControlLayout->addWidget(buDeletePerson);
	QVBoxLayout *personLayout = new QVBoxLayout;
	personLayout->addWidget(new QLabel("Сотрудники:"));
	personLayout->addWidget(viewPersons);
	personLayout->addWidget(edNewPerson);
	personLayout->addLayout(personControlLayout);
	QVBoxLayout *holidaysLayout = new QVBoxLayout;
	viewHolidays = new QListView();
	buAddHoliday = new QPushButton("+");
	buDeleteHoliday = new QPushButton("-");
	buEditHoliday = new QPushButton("...");
	QHBoxLayout *holidaysButtonsLayout = new QHBoxLayout;
	holidaysButtonsLayout->addWidget(buAddHoliday);
	holidaysButtonsLayout->addWidget(buEditHoliday);
	holidaysButtonsLayout->addWidget(buDeleteHoliday);
	holidaysLayout->addWidget(new QLabel("Отпуска:"));
	holidaysLayout->addWidget(viewHolidays);
	holidaysLayout->addLayout(holidaysButtonsLayout);
	holidaysLayout->addStretch();
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(personLayout);
	topLayout->addLayout(holidaysLayout);
	/*QHBoxLayout *selectLayout = new QHBoxLayout;
	selectLayout->addStretch();
	selectLayout->addWidget(buSelectFile);
	selectLayout->addStretch();
	//mainLayout->addLayout(topLayout, 1);
	mainLayout->addLayout(selectLayout);*/
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

DialogEditTxtFile::~DialogEditTxtFile()
{
	
}

void DialogEditTxtFile::saveToFile()
{

}

void DialogEditTxtFile::addPerson()
{

}

void DialogEditTxtFile::deletePerson()
{

}

void DialogEditTxtFile::selectFile()
{
	name_file = QFileDialog::getOpenFileName(this, "Выбор файла", "./", "XML (*.xml)").toStdString();
}

void DialogEditTxtFile::saveFile()
{
}

void DialogEditTxtFile::showDialogHoliday()
{

}

