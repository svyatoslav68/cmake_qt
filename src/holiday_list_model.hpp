/************************************************************
******* Заголовочный файл класса, реализующего **************
******* модель, содержащую отпуска сотрудников.		*********
******         файл holiday_list_model.hpp         *********
************************************************************/
#pragma once

#include "holiday_table_model.hpp"
#include "tholiday.hpp"

class QModelIndex;

class HolidayListModel: public QAbstractItemModel {
	Q_OBJECT
public:
	enum MODE {SQL, TXT};
	/*HolidayTableModel(std::vector<std::string> persons);*/
	explicit HolidayListModel(MODE mode, int row);
	~HolidayListModel() {}
	/* Методы QAbstractItemModel, которые необходимо переопределять *********
	 * в классе-потомке            *****************************************/
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data(const QModelIndex &index, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &) const;
	/***********************************************************************/
	void loadFromTxt();
	void loadFromBd();
private:
	int indexRow;
	std::vector<THoliday> content;
};
