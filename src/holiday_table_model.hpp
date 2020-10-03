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
	enum Column {Number, FIO, Holidays};
	/*HolidayTableModel(std::vector<std::string> persons);*/
	HolidayTableModel();
	~HolidayTableModel();
	/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &) const;
	QModelIndex parent ( const QModelIndex & ) const;
	/*********************************************************************/
	void fillConflicts(std::vector<int> changedRows);
private:
	const std::string Template_SQL_Fill;
	const std::string Template_SQL_Holidays;
	//HolidayTableModel(){};
	static const int COLUMN_COUNT = 3;
	std::map<int, // Number row in table
			std::tuple<int, // cod_person
			std::string, // FIO
			std::vector<THoliday>*, //pointer on vector holidays
			std::set<boost::gregorian::date>* // Dates of conflicts
				>> content;
	std::vector<std::pair<std::vector<int>, int>> conditions;
};

  
