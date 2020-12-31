/***************************************************************
****** Заголовочный файл диалога создания  редактирования ******
******               отпусков сотрудников.       ***************
********             dialog_holiday.hpp               **********
***************************************************************/
#ifndef DIALOG_HOLIDAY_HPP
#define DIALOG_HOLIDAY_HPP

#include <QDialog>
class QDateEdit;
class QLineEdit;
class QPushButton;

class DialogHoliday : public QDialog
{
	Q_OBJECT
public:
	DialogHoliday(QWidget *parent = nullptr);
	~DialogHoliday();
private:
	QPushButton *buOk, *buCancel;
	QDateEdit *deBeginHoliday;
	QLineEdit *leDurationHoliday;
	QLineEdit *leTravelDays;
};

#endif // DIALOG_HOLIDAY_HPP


