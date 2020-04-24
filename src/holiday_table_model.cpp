/************************************************************
******** Файл реализации класса HolidayTableModel ***********
********     файл holiday_table_model.cpp         ***********
************************************************************/
#include <utility>
#include <map>
#include <vector>
#include <string>
#include <QVariant>
#include <QGraphicsScene>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "holiday_table_model.hpp"

using std::pair;
using std::vector;
using std::map;
using std::string;

//const string HolidayTableModel::Template_SQL_Fill =  ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "ListPerson", "getListPersonsUnit");

HolidayTableModel::HolidayTableModel(vector<std::string> persons):
										HolidayTableModel(){
	auto person_iterator = persons.cbegin();
	int number = 0;
	while(person_iterator < persons.cend()){
		pair<std::string, std::vector<THoliday>*> 
				p(*person_iterator, new vector<THoliday>());
		content[++number] = p;
		person_iterator++;
	}
}

HolidayTableModel::HolidayTableModel():								
	Template_SQL_Fill(ValuesFromXML(appParametrs.getNameConfFile().c_str()).
	getStrSQL("FILE.SQL", "ListPerson", "getListPersonsUnit")),
	Template_SQL_Holidays(ValuesFromXML(appParametrs.getNameConfFile().
	c_str()).getStrSQL("FILE.SQL", "THoliday", "getPersonHolidays"))
{
	MYSQL_RES *data_from_BD = nullptr;
	boost::format fmter(Template_SQL_Fill);
	std::stringstream ss;
	ss << fmter%appParametrs.getIdUnit();
	std::string SQL = ss.str();
	int mysql_status = 0;
	mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
	if (mysql_status){
		std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
	}
	else {
		data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
		MYSQL_ROW row;
		row = mysql_fetch_row(data_from_BD);
		int number = 0;
		while (row){
			pair<std::string, std::vector<THoliday>*> p(row[1], new vector<THoliday>());
			content[++number] = p;
			row = mysql_fetch_row(data_from_BD);
		}
	}

}

HolidayTableModel::~HolidayTableModel(){
	for (auto content_iterator: content){
		delete content_iterator.second.second;
	}
}

Qt::ItemFlags HolidayTableModel::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;// = QAbstractItemModel::flags(index);
	if (index.isValid()){
		theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled;
	}
	return theFlags;
}

QVariant HolidayTableModel::data(const QModelIndex &index, int role) const{
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > COLUMN_COUNT)
		return QVariant();
	if (role == Qt::DisplayRole || role == Qt::EditRole){
		switch (index.column()){
			case Number: return index.row() + 1;
			case FIO: return content.at(index.row() + 1).first.c_str();
			default: return -1;
		}
	}
	return QVariant();
}

QVariant HolidayTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal){
		switch (section){
			case Number: return "№ п/п";
			case FIO: return "ФИО";
			case Holidays: return "Отпуска";
			default: QVariant();
		}
	}
	return QVariant();
}

int HolidayTableModel::rowCount(const QModelIndex &index) const{
	return index.isValid() ? 0 : content.size();
}

int HolidayTableModel::columnCount(const QModelIndex &index) const{
	return index.isValid() ? 0 : COLUMN_COUNT;
}

QModelIndex HolidayTableModel::index ( int row, int column, const QModelIndex &) const
{
	return createIndex(row, column);
}

QModelIndex HolidayTableModel::parent ( const QModelIndex & ) const
{
	return QModelIndex();
}
