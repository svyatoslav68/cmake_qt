/************************************************
*	  Реализация диалога выбора подразделения   *
*			  файл dialog_units.cpp             *
************************************************/
#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QTreeView>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>
#include <QHeaderView>
#include <QRect>
#include <boost/lexical_cast.hpp>
#include "modelUnits.hpp"
#include "dialog_units.hpp"


TDialogUnits::TDialogUnits(int unit, int year, QWidget *parent):QDialog(parent),
			_cod_unit(unit), _current_year(year)
{
	setWindowTitle(QString("Настройки"));
	QRect rectDialog;
	rectDialog.setWidth(600);
	rectDialog.setHeight(400);
	setGeometry(rectDialog);
	buOk = new QPushButton("Ok", this);
	buOk->setObjectName("buOk");
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buCancel = new QPushButton("Cancel", this);
	buCancel->setObjectName("buCancel");
	connect(buCancel, SIGNAL(clicked()), this, SLOT(reject()));
	treeUnits = new UnitsTreeView(this);
	treeModelUnits = new ModelUnits();
	treeUnits->setModel(treeModelUnits);
	treeUnits->setSizeColumns();
	connect(treeUnits, SIGNAL(activated(const QModelIndex &)), this, SLOT(activateUnitSlot(const QModelIndex &)));
	connect(treeUnits, SIGNAL(entered(const QModelIndex &)), this, SLOT(activateUnitSlot(const QModelIndex &)));
	connect(treeUnits, SIGNAL(clicked(const QModelIndex &)), this, SLOT(activateUnitSlot(const QModelIndex &)));
	/*QHeaderView *header = treeUnits->header();
	header->setResizeContentsPrecision(0);
	header->setSectionResizeMode(0, QHeaderView::Stretch);
	header->setSectionResizeMode(1, QHeaderView::ResizeToContents);*/
	QLabel *laIdUnit = new QLabel("Код подразделения:", this);
	editIdUnit = new QLineEdit(); 
	editIdUnit->setText(QString::number(_cod_unit));
	editIdUnit->setReadOnly(true);
	QLabel *laYear = new QLabel("Год:", this);
	editYear = new QLineEdit(); 
	editYear->setText(QString::number(_current_year));
	checkSave = new QCheckBox("Запомнить", this);
	QHBoxLayout *top_layout = new QHBoxLayout;
	QVBoxLayout *tree_layout = new QVBoxLayout;
	tree_layout->addWidget(treeUnits);
	QVBoxLayout *year_layout = new QVBoxLayout;
	year_layout->addWidget(laIdUnit);
	year_layout->addWidget(editIdUnit);
	year_layout->addWidget(laYear);
	year_layout->addWidget(editYear);
	year_layout->addWidget(checkSave);
	year_layout->addStretch();
	top_layout->addLayout(tree_layout, 3);
	top_layout->addLayout(year_layout, 1);
	QHBoxLayout *bottom_layout = new QHBoxLayout;
	bottom_layout->addStretch();
	bottom_layout->addWidget(buOk);
	bottom_layout->addWidget(buCancel);
	QVBoxLayout *main_layout = new QVBoxLayout;
	main_layout->addLayout(top_layout);
	main_layout->addLayout(bottom_layout);
	setLayout(main_layout);
}

TDialogUnits::~TDialogUnits()
{
}

int TDialogUnits::getUnit() const
{
	int unit = treeModelUnits->data(treeUnits->currentIndex(), Qt::UserRole).toInt();
	return unit;
}

int TDialogUnits::getYear() const
{
	int result = -1;
	if (!editYear->text().isEmpty()){
		try{
			result = boost::lexical_cast<int>(editYear->text().toStdString());
		//result = boost::conversion::try_lexical_convert<int>(editYear->text().toStdString(), _current_year);
		}
		catch (boost::bad_lexical_cast){
			std::cerr << editYear->text().toStdString() << " is not number.\n}";
			result = -1;
		}
	}
	return result;
}

void TDialogUnits::setYear(int year){
	_current_year = year;
}

void TDialogUnits::setUnit(int unit){
	_cod_unit = unit;
}

bool TDialogUnits::getSave(){
	return static_cast<bool>(checkSave->checkState());
}

void TDialogUnits::activateUnitSlot(const QModelIndex &index){
	//std::cout << "Row = " << index.row() << std::endl;
	int unit = treeModelUnits->data(index, Qt::UserRole).toInt();
	editIdUnit->setText(QString::number(unit));
}
