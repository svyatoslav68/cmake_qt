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

class QVariant;

class THoliday {
using bd=boost::gregorian::date;
using bdd=boost::gregorian::date_duration;
public:
	THoliday();
	THoliday(const bd date_begin, const bdd  holiday_duration, const bdd travel_duration):cod_holiday(-1), m_date_begin(date_begin), holiday_duration(holiday_duration), travel_duration(travel_duration), name_holiday("manual"){};
	THoliday(int _cod_holiday);
	THoliday(	int _cod_holiday, 
				/*int _cod_person, */
				std::string s_date, 
				int duration, 
				int travel_duration, 
				std::string _name_holiday):
				cod_holiday(_cod_holiday), /*cod_person(_cod_person),*/ m_date_begin(boost::gregorian::from_simple_string(std::string(s_date))), holiday_duration(boost::gregorian::date_duration(duration)), travel_duration(boost::gregorian::date_duration(travel_duration)),  name_holiday(_name_holiday){};
	THoliday(QVariant varHoliday);
	THoliday& operator+=(const int numdays);
	THoliday& operator-=(const int numdays);
	THoliday operator+(const int numdays);
	THoliday operator-(const int numdays);
	THoliday& operator++();	
	THoliday& operator--();	
	bool operator==(const bool);
	THoliday& move(int number_days); // +x - move to next date, -x - to prev date
	THoliday& moveTo(int dayInYear);
	bd beginDate() const;
	int firstDay ()const;
	int getCodHoliday() const;
	void setBeginDate(bd newDate){m_date_begin = newDate;}
	void setHolidayDuration(bdd newHolidayDuration){holiday_duration = newHolidayDuration;}
	QVariant toMap() const;
	int numberDaysHoliday() const;
	int numberDaysTravel() const;
	void displayHoliday() const;
	std::set<bd> datesHoliday() const;
	std::set<int> numbersDayHoliday() const;
private:
	//const int m_year;
	int cod_holiday;
	/*int cod_person;*/
	bd m_date_begin;	
	bdd holiday_duration;
	bdd travel_duration;
	std::string name_holiday;
};
#endif //THOLIDAY_HPP
