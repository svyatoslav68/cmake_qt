/***************************************************************
****** Заголовочный файл диалога создания  *********************
*** текстового файла содержащего график отпусков ***************
********             dialog_edit_file.hpp             **********
***************************************************************/
#ifndef DIALOG_EDIT_FILE_HPP
#define DIALOG_EDIT_FILE_HPP

#include <QDialog>
class QListView;
class QLineEdit;
class PersonListModel;
class QPushButton;
class HolidayListModel;


class DialogEditTxtFile : public QDialog{
	Q_OBJECT
public:
	enum MODE {SQL, TXT};
	explicit DialogEditTxtFile(MODE source, QWidget *parent = nullptr);
	virtual ~DialogEditTxtFile();
public slots:
	void saveFile();
private slots:
	void addPerson();
	void deletePerson();
	void addHoliday();
	void deleteHoliday();
	std::string selectFile();
	void slotLoadBD();
	void showDialogHoliday();
	void changeIndexPerson(const QModelIndex &);
private:
	MODE m_source;
	QListView *viewPersons;
	QListView *viewHolidays;
	QLineEdit *edNewPerson;
	QPushButton *buLoadBD;
	QPushButton *buSelectFile;
	QPushButton *buOk, *buCancel, *buSave, *buAddPerson, *buDeletePerson, *buEditPerson;
	QPushButton *buAddHoliday, *buDeleteHoliday, *buEditHoliday;
	PersonListModel *personModel;
	HolidayListModel *holidaysModel;
	std::string m_name_file;	
};

#endif //DIALOG_EDIT_FILE_HPP

