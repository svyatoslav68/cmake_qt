/************************************************************
******* Заголовочный файл класса, реализующего **************
******* модель, содержащую отпуска сотрудников.		*********
******         файл holiday_list_model.hpp         *********
************************************************************/
#pragma once

//#include "tholiday.hpp"
//#include <QAbstractItemModel>
#include "holiday_table_model.hpp"
class THoliday;
class QModelIndex;

class HolidayListModel: public HolidayTableModel {
	Q_OBJECT
public:
	enum MODE {SQL, TXT};
	/*HolidayTableModel(std::vector<std::string> persons);*/
	explicit HolidayListModel(std::vector<std::pair<int, int>> persons, MODE mode = TXT);
	~HolidayListModel() {}
	/* Методы QAbstractItemModel, которые необходимо переопределять *********
	 * в классе-потомке            *****************************************/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data(const QModelIndex &index, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &) const;
	QModelIndex parent ( const QModelIndex & ) const;
	bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
	/***********************************************************************/
	int getIndexRow() {return m_indexRow;}
	void printContent();
public slots:
	void setPosition(int row, int codPerson);
	void addHoliday(const THoliday holiday);
	void loadFromTxt();
	void loadFromBd();
private:
	MODE m_mode;
	int m_indexRow;
private slots:
	void clearContent();
};
