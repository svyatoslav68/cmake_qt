/* Заголовочный файл класса THoliday, представляющего собой
				описание отпуска сотрудника. 
				    файл  tholiday.hpp
***********************************************************/
#ifndef THOLIDAY_HPP
#define THOLIDAY_HPP

#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>
#include "cl_parametrs.hpp"

using bt=boost::gregorian::date;


class THoliday {
public:
	THoliday(){};
	THoliday(int _cod_holiday);
	THoliday(	int _cod_holiday, 
				int _cod_person, 
				std::string s_date, 
				int duration, 
				int travel_duration, 
				std::string _name_holiday):
				cod_holiday(_cod_holiday), cod_person(_cod_person), date_begin(boost::gregorian::from_simple_string(std::string(s_date))), holiday_duration(boost::gregorian::date_duration(duration)), travel_duration(boost::gregorian::date_duration(travel_duration)),  name_holiday(_name_holiday){};
	THoliday& operator+(int numdays);
	THoliday& operator-(int numdays);
	THoliday& move(int number_days); // +x - move to next date, -x - to prev date
	bt beginDate() const;
	int firstDay ()const;
	int numberDaysHoliday() const;
	int numberDaysTravel() const;
	void displayHoliday() const;
	std::set<bt> datesHoliday();
private:
	static const std::string templateSQLHoliday;
	int cod_holiday;
	int cod_person;
	bt date_begin;	
	boost::gregorian::date_duration holiday_duration;
	boost::gregorian::date_duration travel_duration;
	std::string name_holiday;
};
#endif //THOLIDAY_HPP
