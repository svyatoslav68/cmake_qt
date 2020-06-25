/*********************************************
**   Заголовок класса диалога установки     **
**   настроек плана отпусков.               **
**        Файл: dialog_conditions.hpp       **
*********************************************/
#ifndef DIALOG_CONDITIONS_HPP
#define DIALOG_CONDITIONS_HPP

#include <QDialog>

class PersonModel;
class QListView;
class QTableView;
class QPushButton;
class QModelIndex;
class QVariant;
class QLineEdit;
class QStringListModel;
class ModelGroups;
class ModelMembersGroup;
class QLabel;

class DialogConditions : public QDialog{
	Q_OBJECT
public:
	DialogConditions(QWidget *parent = nullptr);
	virtual ~DialogConditions();
public slots:
	void loadConfiguration();
	void saveConfiguration();
	void addGroup();
	void deleteGroup();
	void changeIndexGroup(const QModelIndex &);
	void addMemberSlot();
	void deleteMemberSlot();
private:
	void createWidgets();
	PersonModel *personModel;
	ModelGroups *groupsModel;
	ModelMembersGroup *membersModel;
	QPushButton *buAppend, *buDelete, *buAddCondition, *buDeleteCondition;
	QPushButton *buAddGroup, *buDeleteGroup;
	QLineEdit *editGroup;
	QPushButton *buOk, *buSave;
	QTableView *viewTablePersons;
	QLabel *laContentGroup;
	QListView *viewListGroups, *viewListContentGroup;
};
#endif // DIALOG_CONDITIONS_HPP
