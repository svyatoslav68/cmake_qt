/********************************************************
*** Заголовок диалога редактирования данных           ***
*** файл dialogEditData.hpp 
********************************************************/
#ifndef DIALOGEDITDATA_HPP
#define DIALOGEDITDATA_HPP

#include <QDialog>

class QListView;
class QPushButton;
class HolidayTableModel;
class SimpleHolidayModel;
class MyListView;

class DialogEditData: public QDialog {
	Q_OBJECT
public:
	DialogEditData(QWidget *parent = nullptr);
private:
	HolidayTableModel *model;
	SimpleHolidayModel *holidaysModel;
	QListView  *holidaysView;
	MyListView *personsView;
	QPushButton *okButton, *cancelButton;
	void createWidgets();
private slots:
	void changeFillHolidays();	
};


#endif //DIALOGEDITDATA_HPP
