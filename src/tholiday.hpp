/* Заголовочный файл класса THoliday, представляющего собой
				описание отпуска сотрудника. 
				    файл  tholiday.hpp
***********************************************************/
#ifndef THOLIDAY_HPP
#define THOLIDAY_HPP

#include <boost/lexical_cast.hpp>
#include <string>
#include "cl_parametrs.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

using bt=boost::gregorian::date;

extern clParametrs appParametrs;

class THoliday {
public:
	THoliday();
	THoliday(int cod_holiday);
	THoliday(int _cod_person, std::string s_date, int duration, std::string _name_holiday):cod_person(_cod_person), date_begin(boost::gregorian::from_simple_string(std::string(s_date))), holiday_duration(boost::gregorian::date_duration(duration)), name_holiday(_name_holiday){};
	THoliday& operator+(int numdays);
	THoliday& operator-(int numdays);
	void displayHoliday() const;
private:
	static const std::string templateSQLHoliday;
	MYSQL_RES *data_from_BD;
	int cod_person;
	bt date_begin;	
	boost::gregorian::date_duration holiday_duration;
	std::string name_holiday;
};
#endif //THOLIDAY_HPP
