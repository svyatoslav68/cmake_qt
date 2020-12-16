/************************************************************
******** Файл реализации класса HolidayListModel ***********
********     файл holiday_list_model.cpp         ***********
************************************************************/

#include <QModelIndex>
#include <QVariant>
#include <tuple>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "tholiday.hpp"
#include "holiday_list_model.hpp"

extern clParametrs appParametrs;

std::map<int, std::tuple<int, std::vector<THoliday>>> HolidayListModel::content;

HolidayListModel::HolidayListModel(int row, int cod_person,  MODE mode, QObject *parent):QAbstractItemModel(parent), indexRow(row)
{
	if (mode == SQL) {
		std::string template_SQL_for_Fill = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getPersonHolidays");
		MYSQL_RES *data_from_BD = nullptr;
		std::stringstream ss;
		ss << boost::format(template_SQL_for_Fill)%appParametrs.getYear()%cod_person << std::flush;
		std::string SQL = ss.str(); // Запрос для заполнения content
		std::cout << "SQL = " << SQL << std::endl;
		int mysql_status = 0;
		mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
		if (mysql_status){
			std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
		}
		else {
			data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
			MYSQL_ROW row_holiday;
			std::vector<THoliday> vectorOfHolidays;
			row_holiday = mysql_fetch_row(data_from_BD);
			while (row_holiday){ 
				THoliday newHoliday(boost::lexical_cast<int>(row_holiday[0]), row_holiday[2], boost::lexical_cast<int>(row_holiday[3]), boost::lexical_cast<int>(row_holiday[4]), row_holiday[1]);
				vectorOfHolidays.push_back(newHoliday);
				row_holiday = mysql_fetch_row(data_from_BD);
			}
			content[row] = std::make_tuple(cod_person, vectorOfHolidays);
		}
	}
	else if (mode == TXT) {
		//ValuesFromXML PersonsFile("holidays.xml");			
	}
	else {
		std::cerr << "Constructor PersonModel executed with incorrect parametrs\n";
	}
}

Qt::ItemFlags HolidayListModel::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled|Qt::ItemIsSelectable;
	}
	return theFlags;
}
bool HolidayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return true;
}

QVariant HolidayListModel::data(const QModelIndex &index, int role) const
{
	const int& key = indexRow;
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > 1)
		return QVariant();
	if (role == Qt::DisplayRole){
		std::vector<THoliday> vectorOfHolidays = std::get<1>(content.at(key));
		return vectorOfHolidays.at(index.row()).toMap();
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

QModelIndex HolidayListModel::parent ( const QModelIndex & ) const
{
	return QModelIndex();
}

void HolidayListModel::loadFromTxt()
{
}

void HolidayListModel::loadFromBd()
{
}

void HolidayListModel::clearContent()
{
	content.clear();
}
