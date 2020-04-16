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

class HolidayTableModel: public QAbstractItemModel {
	Q_OBJECT
public:
	HolidayTableModel(std::vector<std::pair<int, std::string>> persons);
	HolidayTableModel(std::string SQL_persons);
	/* Методы QAbstractItemModel, которые необходимо переопределять в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation, int role) const;
private:
	std::map<int, std::pair<std::string, std::vector<THoliday>>> content;
};


