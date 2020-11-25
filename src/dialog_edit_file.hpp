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
class PersonModel;
class QPushButton;


class DialogEditTxtFile : public QDialog{
	Q_OBJECT
public:
	DialogEditTxtFile(QWidget *parent = nullptr);
	virtual ~DialogEditTxtFile();
public slots:
	void saveToFile();
private slots:
	void addPerson();
	void deletePerson();
	void addHoliday();
	void deleteHoliday();
	void selectFile();
	void saveFile();
	void showDialogHoliday();
private:
	QListView *viewPersons;
	QListView *viewHolidays;
	QLineEdit *edNewPerson;
	QPushButton *buSelectFile;
	QPushButton *buOk, *buCancel, *buSave, *buAddPerson, *buDeletePerson, *buEditPerson;
	QPushButton *buAddHoliday, *buDeleteHoliday, *buEditHoliday;
	PersonModel *personModel;
	std::string name_file;	
};

#endif //DIALOG_EDIT_FILE_HPP

