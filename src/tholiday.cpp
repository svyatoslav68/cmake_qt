/* Файл реализации класса THoliday, представляющего собой
				описание отпуска сотрудника. 
				    файл  tholiday.cpp
***********************************************************/
#include <iostream>
#include <QVariant>
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "tholiday.hpp"
#include "str_from_file.hpp"
using bdd=boost::gregorian::date_duration;
using bd=boost::gregorian::date;

extern clParametrs appParametrs;

THoliday::THoliday()//:m_year(appParametrs.getYear()){}
{
}

THoliday::THoliday(int _cod_holiday):cod_holiday(_cod_holiday){//:templateSQLHoliday(ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday")){
using namespace boost::gregorian;
	std::string templateSQLHoliday = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday");;
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
		/*cod_person = boost::lexical_cast<int>(row[2]);		*/
		date_begin = from_simple_string(std::string(row[3]));
		holiday_duration = date_duration(boost::lexical_cast<int>(row[4]));
		travel_duration = date_duration(boost::lexical_cast<int>(row[5]));
	}
}

THoliday::THoliday(QVariant varHoliday)
{
	using namespace boost::gregorian;
	//std::cout << "THoliday from map\n";
	QMap<QString, QVariant> mapHoliday = varHoliday.toMap();
	gregorian_calendar::ymd_type ymd = gregorian_calendar::from_day_number(mapHoliday["begin"].toInt());
	date load_date = { ymd.year, ymd.month, ymd.day };
	//std::cout << "date = " << load_date << std::endl;
	date_begin = load_date;
	holiday_duration = bdd(mapHoliday["duration"].toInt());
	travel_duration = bdd(mapHoliday["travel"].toInt());
}

void THoliday::displayHoliday() const {
using namespace boost::gregorian;
	date_facet *facet(new date_facet("%d.%m.%Y"));
	std::cout.imbue(std::locale(std::cout.getloc(), facet));
	std::cout << std::setw(40) << std::setfill(' ') << std::left << name_holiday << std::setw(13) << date_begin << std::setw(3) << std::right << holiday_duration << std::endl;
}

THoliday& THoliday::operator+=(const int numdays){
	holiday_duration += bdd(numdays);
	return *this;
}

THoliday& THoliday::operator-=(const int numdays){
	if (holiday_duration.days() > numdays - 1){
		holiday_duration -= bdd(numdays);
		std::cout << "From operator -=: " << holiday_duration.days() << std::endl;
	}
	return *this;
}

THoliday THoliday::operator+(const int numdays){
	THoliday result;
	return result+=numdays;
}

THoliday THoliday::operator-(const int numdays){
	THoliday result;
	return result-=numdays;
}

THoliday& THoliday::operator++(){	
	return (*this)+=1;
}
THoliday& THoliday::operator--(){
	return (*this)-=1;
}

bool THoliday::operator==(const bool x){
	return (holiday_duration.days() == 0)?!x:x;
}
THoliday& THoliday::move(int number_days)
{
	date_begin += bdd(number_days);
	return *this;
}

bd THoliday::beginDate() const{
	return date_begin;
}

int THoliday::getCodHoliday() const{
	return cod_holiday;
}

int THoliday::firstDay() const{
	return date_begin.day_of_year();
}

QVariant THoliday::toMap() const{
	QMap<QString, QVariant> resultMap { {QString("begin"), QVariant(date_begin.day_number())},
									{QString("duration"), QVariant(static_cast<int>(holiday_duration.days()))},
									{QString("travel"), QVariant(static_cast<int>(travel_duration.days()))}
	};
	/*std::cout << "From toMap():\n";
	std::cout << "begin = " << resultMap["begin"].toInt() << std::endl;
	std::cout << "duration = " << resultMap["duration"].toInt() << std::endl;
	std::cout << "travel = " << resultMap["travel"].toInt() << std::endl;*/
	return QVariant(resultMap);
}

int THoliday::numberDaysHoliday() const{
	return holiday_duration.days();
}

int THoliday::numberDaysTravel() const{
	return travel_duration.days();
}

std::set<bd> THoliday::datesHoliday() const{
	using namespace boost::gregorian;
	std::set<bd> result;
	date_period holiday_period(date_begin, holiday_duration + travel_duration);
	//std::cout << "holiday_period = " << holiday_period << std::endl;
	day_iterator di(holiday_period.begin());
	while (di != holiday_period.end()){
		result.insert(*di);
		++di;
	}
	return result;
}

std::set<int> THoliday::numbersDayHoliday() const{
	using namespace boost::gregorian;
	std::set<int> result;
	date_period holiday_period(date_begin, holiday_duration + travel_duration);
	day_iterator di(holiday_period.begin());
	while (di != holiday_period.end()){
		result.insert(di->day_of_year());
		++di;
	}
	return result;
}


THoliday &THoliday::moveTo(int dayInYear)
{
	using namespace boost::gregorian;
	greg_year year_holiday = date_begin.year();
	date first_day_year(year_holiday, 1, 1);
	date_begin = date_begin + date_duration(dayInYear);
	return *this;
}

