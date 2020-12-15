/************************************************************
******** Файл реализации класса HolidayListModel ***********
********     файл holiday_list_model.cpp         ***********
************************************************************/

#include <QModelIndex>
#include <QVariant>
#include "cl_parametrs.hpp"
#include "tholiday.hpp"
#include "holiday_list_model.hpp"

extern clParametrs appParametrs;

HolidayListModel::HolidayListModel(MODE mode, int row):indexRow(row)
{
	if (mode == SQL) {
		MYSQL_RES *data_from_BD = nullptr;
	}
	else if (mode == TXT) {
		//ValuesFromXML PersonsFile("holidays.xml");			
	}
	else {
		std::cerr << "Constructor PersonModel executed with incorrect parametrs\n";
	}
}

bool HolidayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return true;
}

QVariant HolidayListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > 1)
		return QVariant();
	if (role == Qt::DisplayRole){
		return content.at(index.row()).toMap();
	}
	return QVariant();
}

int HolidayListModel::rowCount(const QModelIndex& index) const
{
	return index.isValid() ? 0 : content.size();
}

int HolidayListModel::columnCount(const QModelIndex& index) const
{
	return index.isValid() ? 0 : 1;
}

QModelIndex HolidayListModel::index ( int row, int, const QModelIndex &) const
{
	return createIndex(row, 0);
}

void HolidayListModel::loadFromTxt()
{
}

void HolidayListModel::loadFromBd()
{
}
