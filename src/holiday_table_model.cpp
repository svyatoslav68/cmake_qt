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
#include <boost/date_time/gregorian/gregorian.hpp>
#include "tcondition.hpp"
#include "modelMembersGroup.hpp"
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "holiday_table_model.hpp"

using std::pair;
using std::vector;
using std::map;
using std::string;

//const string HolidayTableModel::Template_SQL_Fill =  ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "ListPerson", "getListPersonsUnit");

/*HolidayTableModel::HolidayTableModel(vector<std::string> persons):
										HolidayTableModel(){
	auto person_iterator = persons.cbegin();
	int number = 0;
	while(person_iterator < persons.cend()){
		pair<std::string, std::vector<THoliday>*> 
				p(*person_iterator, new vector<THoliday>());
		content[++number] = p;
		person_iterator++;
	}
}*/

HolidayTableModel::HolidayTableModel():								
	Template_SQL_Fill(ValuesFromXML(appParametrs.getNameConfFile().c_str()).
	getStrSQL("FILE.SQL", "ListPerson", "getMilitaryPersonsUnit")),
	Template_SQL_Holidays(ValuesFromXML(appParametrs.getNameConfFile().
	c_str()).getStrSQL("FILE.SQL", "THoliday", "getPersonHolidays"))
{
	MYSQL_RES *data_from_BD = nullptr;
	//boost::format fmter(Template_SQL_Fill);
	std::stringstream ss;
	//ss << fmter%appParametrs.getIdUnit();
	ss << boost::format(Template_SQL_Fill)%appParametrs.getIdUnit() << std::flush;
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
			MYSQL_RES *data_holidays;
			ss.str("");
			ss.clear();
			ss << boost::format(Template_SQL_Holidays)%appParametrs.getYear()%row[0] << std::flush;
			std::string SQL = ss.str();
			/*Создадим здесь вектор*/
			vector<THoliday> *vec = new vector<THoliday>();
			std::set<boost::gregorian::date> *conflicts = new std::set<boost::gregorian::date>();
			mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
			if (mysql_status){
				std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
			}
			else{
				data_holidays = mysql_store_result(appParametrs.getDescriptorBD());
				MYSQL_ROW row_holiday = mysql_fetch_row(data_holidays);
				while(row_holiday){
					vec->push_back(THoliday(boost::lexical_cast<int>(row_holiday[0]), /*boost::lexical_cast<int>(row[0]),*/ row_holiday[2], boost::lexical_cast<int>(row_holiday[3]), boost::lexical_cast<int>(row_holiday[4]), row_holiday[1]));
					row_holiday = mysql_fetch_row(data_holidays);
				}
			}
			std::tuple<int, std::string, vector<THoliday>*, std::set<boost::gregorian::date>*> t(boost::lexical_cast<int>(row[0]), row[2], vec, conflicts);
			content[++number] = t;
			row = mysql_fetch_row(data_from_BD);
		}
	}
}

HolidayTableModel::~HolidayTableModel(){
	for (auto content_iterator: content){
		delete std::get<2>(content_iterator.second);
		delete std::get<3>(content_iterator.second);
	}
}

Qt::ItemFlags HolidayTableModel::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;// = QAbstractItemModel::flags(index);
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled;
		theFlags |= Qt::ItemIsSelectable;
		if(index.column() == Holidays)
			theFlags |= Qt::ItemIsEditable;
	}
	return theFlags;
}

QVariant HolidayTableModel::data(const QModelIndex &index, int role) const{
	using namespace boost::gregorian;
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > COLUMN_COUNT)
		return QVariant();
	if (role ==Qt::SizeHintRole){
		return QVariant();
	}
	if (role == Qt::DisplayRole || role == Qt::EditRole){
		switch (index.column()){
			case Number: return index.row() + 1;
			case FIO: return std::get<1>(content.at(index.row() + 1)).c_str();
			case Holidays:{
			 /* Создаем QList из QMap, содержащих словарь параметров отпуска */
			 QList<QVariant> list_person_holidays;
			 for (auto holiday : *(std::get<2>(content.at(index.row()+1)))){
				 QMap<QString, QVariant> map_days { {"begin", QVariant(holiday.firstDay())},
											 	    {"duration", QVariant(holiday.numberDaysHoliday())},
												    {"travel", QVariant(holiday.numberDaysTravel())}
				 };
				 list_person_holidays.append(QVariant(map_days));
			 }
			 return list_person_holidays;
			}
			default: return -1;
		}
	}
	if (role == Qt::DecorationRole) {
		return QVariant();
	}
	return QVariant();
}

QVariant HolidayTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
	/*if (role != Qt::DisplayRole)
		return QVariant();*/
	if (role == Qt::DisplayRole){
	if (orientation == Qt::Horizontal){
		switch (section){
			case Number: return "№ п/п";
			case FIO: return "ФИО";
			case Holidays: return "Отпуска";
			default: QVariant();
		}
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
	//std::cout << "Index: row = " << row << ", column = " << column << "\n";
	return createIndex(row, column);
}

QModelIndex HolidayTableModel::parent ( const QModelIndex & ) const
{
	return QModelIndex();
}

bool HolidayTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	using namespace boost::gregorian;
	if (!index.isValid() || role != Qt::EditRole || 
		index.column() != Holidays || index.row() < 0 || index.row() > content.size())
		return false;
	int idPerson = std::get<0>(content.at(index.row()+1));
	std::string FIO = std::get<1>(content.at(index.row()+1));
	vector<THoliday> *vec = std::get<2>(content.at(index.row()+1));
	std::set<boost::gregorian::date> *conflicts = std::get<3>(content.at(index.row()+1));
	QVector<QVariant> vectorOfRectHolidays = value.toList().toVector();
	int i = 0;
	date firstDayYear = date(appParametrs.getYear(), Jan, 1);
	for (auto &holiday : *vec) {
		date_duration dd(vectorOfRectHolidays[i].toRect().left());
		holiday.setBeginDate(firstDayYear + dd);
		date_duration hd(vectorOfRectHolidays[i].toRect().right() - vectorOfRectHolidays[i].toRect().left());
		holiday.setHolidayDuration(hd);
		++i;
		//std::cout << "begin = " << holiday.beginDate() << std::endl;
	}
	std::tuple<int, std::string, vector<THoliday>*, std::set<boost::gregorian::date>*> t(idPerson, FIO, vec, conflicts);
	content[index.row()+1] = t;
	emit dataChanged(index, index);
	return true;
}

void HolidayTableModel::fillConflicts(std::vector<int> changedRows) {
	std::vector<ItemOfGroup> groups;
	ValuesFromXML vx = ValuesFromXML(appParametrs.getNameConfFile().c_str());
	auto vectorNames = vx.getNamesGroup("FILE.Groups");
	for (auto name_group: vectorNames){
		/* Создаем группу*/
		ItemOfGroup item_group(name_group);
		//std::cout << "Create group " << item_group.name << std::endl;
		/* Заполняем вектор списка членов группы */
		for (auto member : vx.getMembersGroup("FILE.Groups", name_group)){
			MemberGroup item_member(boost::lexical_cast<int>(member), &item_group);
			item_group.children->push_back(item_member);
		}
		for (auto pair_condition : vx.getConditionsGroup("FILE.Groups", name_group)){
			ConditionOfGroup item_condition(pair_condition, &item_group);
			item_group.conditions->push_back(item_condition);
		}
		for (auto it : content){
			if (std::find(item_group.children->begin(), item_group.children->end(), std::get<0>(it)))
				std::cout << std::get<0>(it) << std::endl;
			it++;
		}
		/* Присваиваем соотвествующему полю указатель на вектор списка членов */
		groups.push_back(item_group);
	}
	for(auto row:changedRows){
		
	}
}

