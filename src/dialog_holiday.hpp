/***************************************************************
****** Заголовочный файл диалога создания  редактирования ******
******               отпусков сотрудников.       ***************
********             dialog_holiday.hpp               **********
***************************************************************/
#ifndef DIALOG_HOLIDAY_HPP
#define DIALOG_HOLIDAY_HPP

#include <QDialog>
#include <boost/date_time/gregorian/gregorian.hpp>

class QDateEdit;
class QLineEdit;
class QPushButton;

class DialogHoliday : public QDialog
{
	Q_OBJECT
public:
	using bd=boost::gregorian::date;
	using bdd=boost::gregorian::date_duration;
	DialogHoliday(QWidget *parent = nullptr);
	~DialogHoliday();
	bd getBeginDate();
	bdd getDuration();
	bdd getTravelDay();

private:
	QPushButton *buOk, *buCancel;
	QDateEdit *deBeginHoliday;
	QLineEdit *leDurationHoliday;
	QLineEdit *leTravelDays;
};

#endif // DIALOG_HOLIDAY_HPP


