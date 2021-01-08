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

//std::map<int, std::tuple<int, std::vector<THoliday>>> HolidayListModel::content;

HolidayListModel::HolidayListModel(std::vector<std::pair<int, int>> persons, MODE mode):HolidayTableModel(), m_mode(mode), m_indexRow(-1)
{
	std::cout << "Constructor of HolidayListModel\n";
	if (mode == SQL) {
		std::cout << "Mode of HolidayListModel = SQL\n";
		std::string template_SQL_for_Fill = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getPersonHolidays");
		MYSQL_RES *data_from_BD = nullptr;
		std::stringstream ss;
		for (auto personPair : persons){
			ss << boost::format(template_SQL_for_Fill)%appParametrs.getYear()%personPair.second << std::flush;
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
				std::vector<THoliday> *vectorOfHolidays = new std::vector<THoliday>();
				row_holiday = mysql_fetch_row(data_from_BD);
				while (row_holiday){ 
					THoliday newHoliday(boost::lexical_cast<int>(row_holiday[0]), row_holiday[2], boost::lexical_cast<int>(row_holiday[3]), boost::lexical_cast<int>(row_holiday[4]), row_holiday[1]);
					vectorOfHolidays->push_back(newHoliday);
					row_holiday = mysql_fetch_row(data_from_BD);
				}
				content[personPair.first] = std::make_tuple(personPair.second, std::string(""), vectorOfHolidays, nullptr);
			}
		}
	}
	else if (mode == TXT) {
		std::cout << "Mode of HolidayListModel = TXT\n";
		for (auto personPair : persons){
			std::vector<THoliday> *vectorOfHolidays = new std::vector<THoliday>();
			content[personPair.first] = std::make_tuple(personPair.second, std::string(""), vectorOfHolidays, nullptr);
		}
	}
	else {
		std::cerr << "Constructor PersonModel executed with incorrect parametrs\n";
	}
}

Qt::ItemFlags HolidayListModel::flags(const QModelIndex &index) const{
	std::cout << "HolidayTableModel::flags()\n";
	Qt::ItemFlags theFlags;
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled|Qt::ItemIsSelectable;
	}
	return theFlags;
}

bool HolidayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	std::cout << "HolidayTableModel::setData()\n";
	return true;
}

QVariant HolidayListModel::data(const QModelIndex &index, int role) const
{
	std::cout << "HolidayTableModel::data()\n";
	const int& key = m_indexRow;
	if (!index.isValid() || index.row() < 0 || index.row() > std::get<2>(content.at(key))->size() - 1
		|| index.column() < 0 || index.column() > 1)
		return QVariant();
	if (role == Qt::DisplayRole){
		using namespace boost::gregorian;
		std::vector<THoliday> *vectorOfHolidays = std::get<2>(content.at(key));
		if (vectorOfHolidays) {
			QVariant result = vectorOfHolidays->at(index.row()).toMap();
			gregorian_calendar::ymd_type ymd = gregorian_calendar::from_day_number(result.toMap()["begin"].toInt());
			return QVariant(QString("%1.%2.%3").arg(ymd.day).arg(ymd.month).arg(ymd.year));
		}
	}
	return QVariant();
}

int HolidayListModel::rowCount(const QModelIndex& index) const
{
	std::cout << "HolidayListModel::rowCount()\n";
	const int& key = m_indexRow;
	std::cout << "row = " << m_indexRow << std::endl;
	try {
		return index.isValid() ? 0 : std::get<2>(content.at(key))->size();
	}
	catch (...) {
		return 0;
	}
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

bool HolidayListModel::removeRows(int row, int count, const QModelIndex & parent)
{
	beginRemoveRows(parent, row, row+count - 1);
	auto it = std::get<2>(content.at(m_indexRow))->cbegin();
	for (int i = row; i < row+count; ++i){
		std::get<2>(content.at(m_indexRow))->erase(it+i);
	}
	endRemoveRows();
	return true;
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

void HolidayListModel::printContent()
{
	for (auto it : content){
		std::cout << "Row:" << std::get<0>(it) << ", id = " << std::get<0>(std::get<1>(it)) << std::endl;
		int numrow = 0;
		for (auto it_holiday : *(std::get<2>(std::get<1>(it)))){
			std::cout << "Holiday from: " << it_holiday.beginDate() << std::endl;
			QModelIndex index = createIndex(numrow, 0);
			std::cout << "Data from model: " << data(index, Qt::DisplayRole).toString().toStdString() << std::endl;
			++numrow;
		}
	}
}

void HolidayListModel::addHoliday(const THoliday holiday)
{
	std::vector<THoliday> *holidays = std::get<2>(content[m_indexRow]);
	holidays->push_back(holiday);
	emit dataChanged(createIndex(0, 0), createIndex(std::get<2>(content[m_indexRow])->size() - 1, 0));
}

void HolidayListModel::setPosition(int row, int codPerson)
{
	m_indexRow = row;
	emit dataChanged(createIndex(0, 0), createIndex(std::get<2>(content[m_indexRow])->size() - 1, 0));
}

