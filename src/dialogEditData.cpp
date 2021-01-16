/**************************************************
*	 Диалог редактирования данных об отпусках     *
*					сотрудников                   * 
*			  Файл dialogEditData.cpp             *
**************************************************/
#include <QPushButton>
#include <QListView>
#include <QBoxLayout>
#include "holiday_table_model.hpp"
#include "simple_holiday_model.hpp"
#include "tholiday.hpp"
#include "my_list_view.hpp"
#include "dialogEditData.hpp"

DialogEditData::DialogEditData(QWidget *parent):QDialog(parent)
{
	model = new HolidayTableModel();
	model->loadFromBD();
	holidaysModel = new SimpleHolidayModel();
	createWidgets();
}

void DialogEditData::createWidgets()
{
	personsView = new MyListView(this);
	personsView->setModel(model);
	personsView->setModelColumn(HolidayTableModel::FIO);
	connect (personsView, &MyListView::changeCurrentIndex, this, [this](){this->changeFillHolidays();});
	holidaysView = new QListView(this);
	holidaysView->setModel(holidaysModel);
	okButton = new QPushButton("Ok", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	cancelButton = new QPushButton("Cancel", this);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(personsView);
	topLayout->addWidget(holidaysView);
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(okButton);
	bottomLayout->addWidget(cancelButton);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout, 1);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void DialogEditData::changeFillHolidays()
{
	std::cout << "receive signal changeFillHolidays()\n";
	holidaysModel->fillContent(model->getHolidaysForPerson(personsView->currentIndex().row()));
	//std::cout << "uint = " model->data(model->currentIndex(), Qt::UserRole).toUint() << std::endl;
}

