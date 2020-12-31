/**********************************************************************
 ******* Файл реализации диалога создания и редактирования ************
 *******               отпусков сотрудников                ************
 *********************************************************************/
#include <string>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include "dialog_holiday.hpp"

DialogHoliday::DialogHoliday(QWidget *parent)
{
	setWindowTitle("Параметры планируемого отпуска");
	buOk = new QPushButton("Ok", this);
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buCancel = new QPushButton("Отмена", this);
	QLabel *laBeginHoliday = new QLabel("Дата начала отпуска:");
	deBeginHoliday = new QDateEdit(this);
	QLabel *laDuration = new QLabel("Продолжительность (сут.):");
	leDurationHoliday = new QLineEdit(this);
	QLabel *laTravelDays = new QLabel("Дорога (сут.):");
	leTravelDays = new QLineEdit(this);
	QVBoxLayout *beginDateLayout = new QVBoxLayout;
	beginDateLayout->addWidget(laBeginHoliday);
	beginDateLayout->addWidget(deBeginHoliday);
	QVBoxLayout *durationLayout = new QVBoxLayout;
	durationLayout->addWidget(laDuration);
	durationLayout->addWidget(leDurationHoliday);
	QVBoxLayout *travelLayout = new QVBoxLayout;
	travelLayout->addWidget(laTravelDays);
	travelLayout->addWidget(leTravelDays);
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(beginDateLayout);
	topLayout->addLayout(durationLayout);
	topLayout->addLayout(travelLayout);
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(buOk);
	bottomLayout->addWidget(buCancel);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

DialogHoliday::~DialogHoliday() 
{

}

