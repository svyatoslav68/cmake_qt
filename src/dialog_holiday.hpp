/***************************************************************
****** Заголовочный файл диалога создания  редактирования ******
******               отпусков сотрудников.       ***************
********             dialog_holiday.hpp               **********
***************************************************************/
#ifndef DIALOG_HOLIDAY_HPP
#define DIALOG_HOLIDAY_HPP

#include <QDialog>

class DialogHoliday : public QDialog
{
	Q_OBJECT
public:
	DialogHoliday(QWidget *parent = nullptr);
	~DialogHoliday();
};

#endif // DIALOG_HOLIDAY_HPP


