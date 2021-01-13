/************************************************************
******* Заголовочный файл класса, реализующего **************
******* модель, содержащую отпуска сотрудников.		*********
******         файл holiday_list_model.hpp         *********
************************************************************/
#pragma once

//#include "tholiday.hpp"
//#include <QAbstractItemModel>
#include "holiday_table_model.hpp"
#include "str_from_file.hpp"

class THoliday;
class QModelIndex;

class HolidayListModel: public HolidayTableModel {
	Q_OBJECT
public:
	enum MODE {SQL, TXT};
	/*HolidayTableModel(std::vector<std::string> persons);*/
	explicit HolidayListModel(std::vector<std::pair<int, int>> persons, std::string nameFile = "", MODE mode = TXT);
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
	std::vector<PersonsFile::holiday> getHolidaysForSaving();
	void printContent();
public slots:
	void setPosition(const int &row, const int &codPerson);
	void addHoliday(const THoliday holiday);
	void loadFromTxt();
	void loadFromBd();
private:
	MODE m_mode;
	int m_indexRow;
	std::string m_nameFile;
private slots:
	void clearContent();
};
