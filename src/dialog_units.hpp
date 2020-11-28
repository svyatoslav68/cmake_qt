/************************************************
*	  Заголовок диалога выбора подразделения    *
*			  файл dialog_units.hpp             *
************************************************/

#ifndef DIALOG_UNITS_H
#define DIALOG_UNITS_H

#include <QDialog>

class QTreeView;
class QPushButton;
class QLineEdit;
class QCheckBox;
class ModelUnits;
class UnitsTreeView;

class TDialogUnits: public QDialog
{
	Q_OBJECT
public:
	TDialogUnits(int unit, int year, QWidget *parent=0);
	int getUnit() const;
	int getYear() const;
	void setYear(int year);
	void setUnit(int unit);
	bool getSave();
	~TDialogUnits();
private:
	int _cod_unit;
	int _current_year;
	ModelUnits *treeModelUnits;
	UnitsTreeView *treeUnits;
	QLineEdit *editYear;
	QLineEdit *editIdUnit;
	QCheckBox *checkSave;
	QPushButton *buOk, *buCancel;
private slots:
	 void activateUnitSlot(const QModelIndex &);
};
#endif //DIALOG_UNITS_H

