/************************************************************
******** Файл реализации класса HolidayTableModel ***********
********     файл holiday_table_model.cpp         ***********
************************************************************/
#include <utility>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <QVariant>
#include <QGraphicsScene>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <iomanip>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "tcondition.hpp"
#include "modelMembersGroup.hpp"
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "holiday_table_model.hpp"
#include "functions.hpp"

using std::pair;
using std::vector;
using std::map;
using std::string;

extern clParametrs appParametrs;

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

HolidayTableModel::HolidayTableModel()
{
}

HolidayTableModel::~HolidayTableModel(){
	for (auto content_iterator: content){
		std::vector<THoliday> *vectorHolidays = std::get<2>(content_iterator.second);
		if (vectorHolidays)
			delete vectorHolidays;
		std::set<boost::gregorian::date> *setConflicts = std::get<3>(content_iterator.second);
		if (setConflicts)
			delete setConflicts;
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
			case FIO: return std::get<1>(content.at(index.row())).c_str();
			case Holidays:{
			 /* Создаем QList из QMap, содержащих словарь параметров отпуска */
			 QList<QVariant> list_person_holidays;
			 for (auto holiday : *(std::get<2>(content.at(index.row())))){
				 list_person_holidays.append(QVariant(holiday.toMap()));
			 }
			 return list_person_holidays;
			}
			default: return -1;
		}
	}
	if (role == Qt::DecorationRole) {
		if (index.column() == Holidays){
			QList<QVariant> list_periods;
			for (auto period : setToPeriods(*(std::get<3>(content.at(index.row()))))){
				QMap<QString, QVariant> map_period { {"begin_day", QVariant(period.begin().day_of_year())},
													 {"number_days", QVariant(qlonglong(period.length().days()))}
				};
				list_periods.append(QVariant(map_period));
			}
			return list_periods;
		}
		else
			return QVariant();
	}
	return QVariant();
}

/*QVariant HolidayTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
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
}*/

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
	int idPerson = std::get<0>(content.at(index.row()));
	varCodPerson.insert(index.row());
	std::string FIO = std::get<1>(content.at(index.row()));
	vector<THoliday> *vec = std::get<2>(content.at(index.row()));
	std::set<date> *conflicts = std::get<3>(content.at(index.row()));
	//QVector<QVariant> vectorOfRectHolidays = value.toList().toVector();
	QVector<QVariant> vecMaps = value.toList().toVector();
	int i = 0;
	//date firstDayYear = date(appParametrs.getYear(), Jan, 1);
	for (auto &holiday : *vec) {
		//date_duration dd(vectorOfRectHolidays[i].toRect().left());
		gregorian_calendar::ymd_type ymd = gregorian_calendar::from_day_number(vecMaps[i].toMap()["begin"].toInt());
		date load_date = { ymd.year, ymd.month, ymd.day };
		holiday.setBeginDate(load_date);
		//date_duration hd(vectorOfRectHolidays[i].toRect().right() - vectorOfRectHolidays[i].toRect().left());
		holiday.setHolidayDuration(date_duration(vecMaps[i].toMap()["duration"].toInt()));
		++i;
		//std::cout << "begin = " << holiday.beginDate() << std::endl;
	}
	std::tuple<int, std::string, vector<THoliday>*, std::set<date>*> t(idPerson, FIO, vec, conflicts);
	content[index.row()] = t;
	fillConflicts();
	emit dataChanged(index, index);
	return true;
}

void HolidayTableModel::loadFromBD(){
	for (auto content_iterator: content){
		std::vector<THoliday> *vectorHolidays = std::get<2>(content_iterator.second);
		if (vectorHolidays)
			delete vectorHolidays;
		std::set<boost::gregorian::date> *setConflicts = std::get<3>(content_iterator.second);
		if (setConflicts)
			delete setConflicts;
	}
	content.clear();
	const std::string Template_SQL_Fill = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "ListPerson", "getMilitaryPersonsUnit");
	const std::string Template_SQL_Holidays = 
	ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getPersonHolidays");
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
			//std::cout << "Отпускник: " << row[2] << std::endl;
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
					//std::cout << "Вызывается конструктор THoliday(), begin=" << row_holiday[2] << ", duration=" << row_holiday[3] << ", travel=" << row_holiday[4] << std::endl; 
					vec->push_back(THoliday(boost::lexical_cast<int>(row_holiday[0]), /*boost::lexical_cast<int>(row[0]),*/ row_holiday[2], boost::lexical_cast<int>(row_holiday[3]), boost::lexical_cast<int>(row_holiday[4]), row_holiday[1]));
					row_holiday = mysql_fetch_row(data_holidays);
				}
			}
			std::tuple<int, std::string, vector<THoliday>*, std::set<boost::gregorian::date>*> t(boost::lexical_cast<int>(row[0]), row[2], vec, conflicts);
			content[number++] = t;
			row = mysql_fetch_row(data_from_BD);
		}
		delete data_from_BD;
	}
}

void HolidayTableModel::fillConflicts(){ //const std::vector<int> &changedRows) {
	using bt=boost::gregorian::date;
	for (auto it : content){ // Проходим по всему содержимому таблицы отпусков
		std::set<boost::gregorian::date> *setConflicts = std::get<3>(it.second);
		if (setConflicts)
			setConflicts->clear();
	}
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
		for (auto it : content){ // Проходим по всему содержимому таблицы отпусков
			if (std::find(item_group.children->begin(), item_group.children->end(), std::get<0>(it.second)) != item_group.children->end()){					// Если идентификатор сотрудника есть среди идентификаторов в условии
				//std::set<boost::gregorian::date> *setOfConditions = new std::set<boost::gregorian::date>();
				std::set<bt> holidays_person; // Множество дат составляющих все отпуска сотрудника
				//std::cout << std::get<0>(it.second) << ":" << std::get<1>(it.second) << std::endl;
				for (auto it_holiday : *(std::get<2>(it.second))) { // Пройти по всем отпускам сотрудника
					std::set<bt> dates = it_holiday.datesHoliday();
					holidays_person.insert(dates.begin(), dates.end());
				}
				for (auto it_date : holidays_person){ // Проходим по всем датам, составляющим отпуска сотрудников
					// Инкрементируем член массива, соответсвующий дню года, на который попадает отпуск сотрудника //
					++(item_group.holidays_in_year[it_date.day_of_year()].first);
					item_group.holidays_in_year[it_date.day_of_year()].second.push_back(it.first);
					//++item_group.holidays_in_year[it_date.day_of_year()];
					for (auto condition : *item_group.conditions){
						//std::cout <<it_date.day_of_year() << ":" << item_group.holidays_in_year[it_date.day_of_year()];
						if (item_group.holidays_in_year[it_date.day_of_year()].first > condition.number_members) {
							for (auto num_row : item_group.holidays_in_year[it_date.day_of_year()].second) { 
								std::get<3>(content[num_row])->insert(it_date);
								//std::cout << std::get<1>(it.second) << '-' << it_date << " : ";
							}
						}
					}
					//std::cout<<std::endl;
				}
			}
		}
		/* Присваиваем соотвествующему полю указатель на вектор списка членов */
		groups.push_back(item_group);
		/*std::cout << "Группа условий: " << item_group.name << std::endl;
		uint16_t number_day {0};
		for (int n : item_group.holidays_in_year){
			++number_day;
			for (auto condition : *item_group.conditions){
				if (n > condition.number_members) {
					using bd = boost::gregorian::date_duration;
					//std::cout << std::showpos << n << ':';
					bt fail_date = bt(2020, boost::gregorian::Jan, 1) + bd(number_day-1);
					//std::cout << fail_date << " : ";
					for (auto member_of_group : *item_group.children){
						if (){}
					}
				}
				else
					;//std::cout << std::noshowpos << n << ':';
			}
		}
		std::cout << std::endl;*/
	}
}

void HolidayTableModel::saveToTxt(std::string nameTxtFile){
	/*using std::cout;
	using std::setw;
	using std::setfill;
	using std::left;
	using std::right;
	using std::ios_base;*/
	using namespace std;
	for (auto the_person:content){
		cout << std::get<0>(the_person.second) << ":" << std::get<1>(the_person.second) << std::endl;
			/*cout << right <>< setw(25) << "Начало";
			cout << right << setw(10) << "Длина" << std::endl;
			cout.setf(ios_base::left, ios_base::adjustfield);*/
		for (auto the_holiday:*(std::get<2>(the_person.second))){
			cout << right;
			cout.width(25);
			cout  << the_holiday.beginDate();
			cout.width(10);
			cout <<  the_holiday.numberDaysHoliday();
			cout.width(6);
			cout << right << the_holiday.numberDaysTravel() << std::endl;
		}
	}
}

void HolidayTableModel::saveToBD(){
	const std::string Template_SQL_Update = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "updateHoliday");
	MYSQL_RES *data_from_BD = nullptr; // Pointer to return data from query
	for (auto it: varCodPerson){
		for (auto holiday:*(std::get<2>(content[it]))){
			std::stringstream ss;
			ss << boost::format(Template_SQL_Update)%boost::gregorian::to_iso_extended_string(holiday.beginDate())%holiday.numberDaysHoliday()%holiday.numberDaysTravel()%holiday.getCodHoliday() << std::flush;
			std::string SQL = ss.str();
			int mysql_status = 0;
			mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
			if (mysql_status){
				std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
			}
			else {
				std::cout << "Выполнен запрос: " << SQL << std::endl;
				data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
			}
		}
		std::cout << std::endl;
	}
	varCodPerson.clear();
}

void HolidayTableModel::outChangedData(){
	std::cout << "Измененные сотрудники:\n";
	for (auto it: varCodPerson){
		std::cout << it << std::endl;
		std::cout << std::get<1>(content[it]) << std::endl;
		for (auto holiday:*(std::get<2>(content[it]))){
			std::cout << std::right;
			std::cout.width(20);
			std::cout << holiday.beginDate();
			std::cout.width(10);
			std::cout <<holiday.numberDaysHoliday();
			std::cout.width(10);
			std::cout <<holiday.numberDaysTravel();
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

