/* Файл реализации класса THoliday, представляющего собой
				описание отпуска сотрудника. 
				    файл  tholiday.cpp
***********************************************************/
#include <iostream>
#include <boost/format.hpp>
#include "tholiday.hpp"
#include "str_from_file.hpp"
using bdd=boost::gregorian::date_duration;
using bt=boost::gregorian::date;

extern clParametrs appParametrs;

const std::string THoliday::templateSQLHoliday=ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday");

THoliday::THoliday(int _cod_holiday):cod_holiday(_cod_holiday){//:templateSQLHoliday(ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday")){
using namespace boost::gregorian;
	MYSQL_RES *data_from_BD = nullptr;
	std::stringstream ss;
	ss << boost::format(templateSQLHoliday)%cod_holiday;
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
		name_holiday = row[1];
		cod_person = boost::lexical_cast<int>(row[2]);		
		date_begin = from_simple_string(std::string(row[3]));
		holiday_duration = date_duration(boost::lexical_cast<int>(row[4]));
		travel_duration = date_duration(boost::lexical_cast<int>(row[5]));
	}
}

void THoliday::displayHoliday() const {
using namespace boost::gregorian;
	date_facet *facet(new date_facet("%d.%m.%Y"));
	std::cout.imbue(std::locale(std::cout.getloc(), facet));
	std::cout << std::setw(40) << std::setfill(' ') << std::left << name_holiday << std::setw(13) << date_begin << std::setw(3) << std::right << holiday_duration << std::endl;
}

THoliday& THoliday::operator+(int numdays){
	holiday_duration += bdd(numdays);
	return *this;
}

THoliday& THoliday::operator-(int numdays){
	holiday_duration -= bdd(numdays);
	return *this;
}

THoliday& THoliday::move(int number_days)
{
	date_begin += bdd(number_days);
	return *this;
}

bt THoliday::beginDate() const{
	return date_begin;
}

int THoliday::firstDay() const{
	return date_begin.day_of_year();
}

int THoliday::numberDaysHoliday() const{
	return holiday_duration.days();
}

int THoliday::numberDaysTravel() const{
	return travel_duration.days();
}

std::set<bt> THoliday::datesHoliday(){
	std::set<bt> result;
	return result;
}

THoliday &THoliday::moveTo(int dayInYear)
{

}

