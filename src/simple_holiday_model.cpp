/***************************************************
*		  Реализация класса простой модели         *
*		   файл simple_holiday_model.cpp           *
***************************************************/

#include <iostream>
#include <QList>
#include <list>
#include <QModelIndex>
#include "tholiday.hpp"
#include "simple_holiday_model.hpp"
 

SimpleHolidayModel::SimpleHolidayModel():content(nullptr)
{
}

QVariant SimpleHolidayModel::data(const QModelIndex &index, int role) const{
using namespace boost::gregorian;
	if (!index.isValid() || index.row() < 0 || index.row() > content->size()
		|| index.column() < 0 || index.column() > COLUMN_COUNT)
		return QVariant();
	if (role == Qt::DisplayRole || role == Qt::EditRole){
		THoliday buffer = content->at(index.row());
		if (buffer.numberDaysTravel() > 0)
			return QVariant(QString("%1.%2.%3 - %4.%5.%6 + %7").arg(buffer.beginDate().day()).arg(buffer.beginDate().month()).arg(buffer.beginDate().year()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).day()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).month()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).year()).arg(buffer.numberDaysTravel()));
		else
			return QVariant(QString("%1.%2.%3 - %4.%5.%6").arg(buffer.beginDate().day()).arg(buffer.beginDate().month()).arg(buffer.beginDate().year()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).day()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).month()).arg((buffer.beginDate()+days(buffer.numberDaysHoliday())).year()));
	}
	return QVariant();
}

QModelIndex SimpleHolidayModel::index ( int row, int column, const QModelIndex &) const
{
	return createIndex(row, column);
}

int SimpleHolidayModel::rowCount(const QModelIndex& index) const {
	return ((index.isValid()) or !content) ? 0 : content->size();
}

void SimpleHolidayModel::fillContent(std::vector<THoliday> *source)
{
	beginResetModel();
	content = source;
	endResetModel();
	//emit dataChanged(createIndex(0,0), createIndex(content->size(),0));
}

