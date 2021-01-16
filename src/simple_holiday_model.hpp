/***************************************************
*		  Заголовок класса простой модели,         *
*			 хранящей список отпусков              *
*		   файл simple_holiday_model.hpp           *
***************************************************/
#ifndef SIMPLE_HOLIDAY_MODEL_HPP
#define SIMPLE_HOLIDAY_MODEL_HPP

#include <QAbstractListModel>

class THoliday; 

class SimpleHolidayModel: public QAbstractListModel
{
	Q_OBJECT
public: 
	SimpleHolidayModel();
	QModelIndex index ( int row, int column = 0, const QModelIndex &index = QModelIndex()) const;
	int rowCount(const QModelIndex&) const;
	QVariant data(const QModelIndex &index, int role) const;
	void fillContent(std::vector<THoliday> *source);
private:
	static const int COLUMN_COUNT = 1;
	std::vector<THoliday> *content;
};
#endif // SIMPLE_HOLIDAY_MODEL_HPP
