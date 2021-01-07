/**********************************************************************
 ******* Файл реализации диалога создания и редактирования ************
 *******               отпусков сотрудников                ************
 *********************************************************************/
#include <string>
#include <QLineEdit>
#include <QValidator>
#include <QDateEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "dialog_holiday.hpp"

using bd=boost::gregorian::date;
using bdd=boost::gregorian::date_duration;

DialogHoliday::DialogHoliday(QWidget *parent)
{
	setWindowTitle("Параметры планируемого отпуска");
	buOk = new QPushButton("Ok", this);
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buCancel = new QPushButton("Отмена", this);
	QLabel *laBeginHoliday = new QLabel("Дата начала отпуска:");
	deBeginHoliday = new QDateEdit(this);
	QValidator *durationValidator = new QIntValidator(1, 366, this);
	QLabel *laDuration = new QLabel("Продолжительность (сут.):");
	leDurationHoliday = new QLineEdit(this);
	leDurationHoliday->setValidator(durationValidator);
	QLabel *laTravelDays = new QLabel("Дорога (сут.):");
	QValidator *travelValidator = new QIntValidator(2, 100, this);
	leTravelDays = new QLineEdit(this);
	leTravelDays->setValidator(travelValidator);
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

bd DialogHoliday::getBeginDate()
{
	bd d(boost::gregorian::from_string(deBeginHoliday->date().toString("yyyy-MM-dd").toStdString()));
	return d;
}

bdd DialogHoliday::getDuration()
{
	return bdd(leDurationHoliday->text().toInt());
}

bdd DialogHoliday::getTravelDay()
{
		return bdd(leTravelDays->text().toInt());
}

