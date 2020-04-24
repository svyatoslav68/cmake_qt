/************************************************************
******* Заголовочный файл класса, реализующего **************
******* табличную модель, в одном из столбцов  **************
**** которой, будут прямоугольники, соответствующие *********
****            отпускам сотрудников.				*********
******         файл holiday_table_model.hpp         *********
************************************************************/
#pragma once

#include <QAbstractItemModel>
#include "tholiday.hpp"

class THoliday;
class QGraphicsScene;
class QVariant;
class QModelIndex;

extern clParametrs appParametrs;

class HolidayTableModel: public QAbstractItemModel {
	Q_OBJECT
public:
	HolidayTableModel(std::vector<std::string> persons);
	HolidayTableModel();
	~HolidayTableModel();
	/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &) const;
	QModelIndex parent ( const QModelIndex & ) const;
private:
	const std::string Template_SQL_Fill;
	const std::string Template_SQL_Holidays;
	//HolidayTableModel(){};
	enum Column {Number, FIO, Holidays};
	static const int COLUMN_COUNT = 3;
	std::map<int, std::pair<std::string, std::vector<THoliday>*>> content;
};

  
