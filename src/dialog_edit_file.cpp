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
#include <str_from_file.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "tholiday.hpp"
#include "tperson.hpp"
#include "person_list_model.hpp"
#include "holiday_list_model.hpp"
#include "dialog_edit_file.hpp"
#include "dialog_holiday.hpp"

DialogEditTxtFile::DialogEditTxtFile(MODE source, QWidget *parent):m_source(source),personModel(nullptr),holidaysModel(nullptr)
{
	viewPersons = new QListView();
	viewHolidays = new QListView();
	if (m_source == TXT){
		setWindowTitle(QString("Содержимое файла: %1").arg(m_name_file.c_str()));
		personModel = new PersonListModel(PersonModel::TXT, this);
		connect(viewPersons, &QListView::clicked, this, [this](const QModelIndex &index){this->changeIndexPerson(viewPersons->currentIndex());});
		holidaysModel = new HolidayListModel(personModel->pairOfPersons(), HolidayListModel::TXT);
	}
	else if (m_source == SQL) {
		setWindowTitle(QString("Содержимое БД"));
		personModel = new PersonListModel(PersonModel::SQL, this);
		holidaysModel = new HolidayListModel(personModel->pairOfPersons(), HolidayListModel::SQL);
	}
	viewPersons->setModel(personModel);
	viewHolidays->setModel(holidaysModel);
	buSelectFile = new QPushButton("Загрузить из файла");
	buSelectFile->setObjectName("buSelectFile");
	connect(buSelectFile, &QPushButton::clicked, this, [this](){this->selectFile();});
	buLoadBD = new QPushButton("Загрузить из БД");
	buLoadBD->setObjectName("buLoadBD");
	connect(buLoadBD, &QPushButton::clicked, this, [this](){this->slotLoadBD();});
	buSave = new QPushButton("Сохранить");
	connect(buSave, &QPushButton::clicked, this, [this](){this->saveFile();});
	buOk = new QPushButton("Ok");
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buCancel = new QPushButton("Cancel");
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(buSave);
	bottomLayout->addWidget(buSelectFile);
	bottomLayout->addWidget(buLoadBD);
	bottomLayout->addStretch();
	bottomLayout->addWidget(buOk);
	bottomLayout->addWidget(buCancel);
	edNewPerson = new QLineEdit();
	buAddPerson = new QPushButton("+");
	connect(buAddPerson, &QPushButton::clicked, this, [this](){this->addPerson();});
	buEditPerson = new QPushButton("...");
	buDeletePerson = new QPushButton("-");
	connect(buDeletePerson, &QPushButton::clicked, this, [this](){this->deletePerson();});
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
	buAddHoliday = new QPushButton("+");
	connect(buAddHoliday, &QPushButton::clicked, this, [this](){this->addHoliday();});
	buDeleteHoliday = new QPushButton("-");
	connect(buDeleteHoliday, &QPushButton::clicked, this, [this](){this->deleteHoliday();});
	buEditHoliday = new QPushButton("...");
	QHBoxLayout *holidaysButtonsLayout = new QHBoxLayout;
	buAddHoliday->setEnabled(false);
	buEditHoliday->setEnabled(false);
	buDeleteHoliday->setEnabled(false);
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
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

DialogEditTxtFile::~DialogEditTxtFile()
{
	
}

void DialogEditTxtFile::addPerson()
{
	personModel->addPerson(-1, "", edNewPerson->text().toStdString());
	edNewPerson->clear();
}

void DialogEditTxtFile::deletePerson()
{
	if (viewPersons->currentIndex().isValid())
		personModel->removeRows(viewPersons->currentIndex().row(), 1);
}

std::string DialogEditTxtFile::selectFile()
{
	m_source = TXT;
	if (personModel){
		delete personModel;
	}
	if (holidaysModel) {
		delete holidaysModel;
	}
	personModel = new PersonListModel(PersonListModel::TXT, this);
	viewPersons->setModel(personModel);
	m_name_file = QFileDialog::getOpenFileName(this, "Выбор файла", "", "XML (*.xml)").toStdString();
	PersonsFile file(m_name_file.c_str());
	setWindowTitle(QString("Содержимое файла: %1").arg(m_name_file.c_str()));
	std::vector<PersonsFile::employee> v = file.getPersons();
	for (auto p : v ){
		//std::cout << "Employee: " << p.id_employee << ":" << p.family << std::endl;
		personModel->addPerson(p.id_employee, p.position, p.family);
	}
	holidaysModel = new HolidayListModel(personModel->pairOfPersons(), HolidayListModel::TXT);
	viewHolidays->setModel(holidaysModel);
	return m_name_file;
}

void DialogEditTxtFile::saveFile()
{
	bool openFile = false;
	PersonsFile file(m_name_file.c_str());
	file.savePersons(personModel->getContent());
	std::cout << "В файл: " << m_name_file << "\n";
	std::cout << "Мы пытались сохранить следующее:\n";
	for (auto [id, position, fio] : personModel->getContent()){
		const char * str_position = position.size()?std::string(" "+position).c_str():""; 
		std::cout << id << ":" << str_position << fio.c_str() << std::endl;
	}
}

void DialogEditTxtFile::showDialogHoliday()
{

}

void DialogEditTxtFile::addHoliday(){
	DialogHoliday *dialogHoliday = new DialogHoliday();
	if (dialogHoliday->exec() == QDialog::Accepted){
		THoliday newHoliday(dialogHoliday->getBeginDate(), dialogHoliday->getDuration(), dialogHoliday->getTravelDay());		
		newHoliday.displayHoliday();
		if (holidaysModel && viewPersons->currentIndex().isValid()) {
			holidaysModel->setPosition(viewPersons->currentIndex().row(), personModel->PersonModel::data(viewPersons->currentIndex(), Qt::DisplayRole).toInt());
			std::cout << "holidaysModel.m_indexRow = " << holidaysModel->getIndexRow() << std::endl;
			holidaysModel->addHoliday(newHoliday);
		}
	}
}

void DialogEditTxtFile::deleteHoliday(){
	if (holidaysModel){
		QModelIndex indexHoliday = viewHolidays->currentIndex();
		if (indexHoliday.isValid())
			holidaysModel->removeRows(indexHoliday.row(), 1);
	}
}

void DialogEditTxtFile::slotLoadBD()
{
	m_source = SQL;
	setWindowTitle(QString("Содержимое БД"));
	if (personModel){
		delete personModel;
	}
	personModel = new PersonListModel(PersonModel::SQL, this);
	viewPersons->setModel(personModel);
	if (holidaysModel) {
		delete holidaysModel;
	}
	holidaysModel = new HolidayListModel(personModel->pairOfPersons(), HolidayListModel::SQL);
	viewHolidays->setModel(holidaysModel);
}

void DialogEditTxtFile::changeIndexPerson(const QModelIndex &index)
{
	if (!index.isValid()) {
		buAddHoliday->setEnabled(false);
		buEditHoliday->setEnabled(false);
		buDeleteHoliday->setEnabled(false);
		return;
	}
	buAddHoliday->setEnabled(true);
	buEditHoliday->setEnabled(true);
	buDeleteHoliday->setEnabled(true);
	holidaysModel->setPosition(index.row(), personModel->PersonModel::data(index, Qt::DisplayRole).toInt());
	//QModelIndex modifyIndex = index.siblingAtColumn(PersonModel::ID);
	//std::cout << "row = " << index.row() << ", column = " << index.column() << ", id = " << personModel->PersonModel::data(index, Qt::DisplayRole).toInt() << std::endl;
	//holidaysModel = new HolidayListModel(index.row(), personModel->PersonModel::data(index, Qt::DisplayRole).toInt(), (m_source == SQL)?HolidayListModel::SQL:HolidayListModel::TXT);
	//viewHolidays->setModel(holidaysModel);
	//holidaysModel->printContent();
}
