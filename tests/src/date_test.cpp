	/* Тест для проверки правильности работы с датами */
#include <iostream>
#include <algorithm>
#include "tholiday.hpp"
#include "functions.hpp"

#include "cl_parametrs.hpp"

	clParametrs appParametrs; // Параметры приложения - глобальная переменная

int main() {
	using boostdate=boost::gregorian::date;
	using namespace boost::gregorian;
	date d(day_clock::local_day());
	std::cout << "Сегодня - " << d << std::endl;
	THoliday h1(-1, "2020-09-02", 15, 2, "");
	THoliday h2(-1, "2020-09-10", 21, 4, "");
	THoliday h3(-1, "2020-09-16", 15, 0, "");
	//h.displayHoliday();
	std::set<boostdate> set_dates1=h1.datesHoliday();
	std::set<boostdate> set_dates2=h2.datesHoliday();
	std::set<boostdate> set_dates3=h3.datesHoliday();
	std::vector<std::set<boostdate>> boost_sets;
	boost_sets.push_back(set_dates1);
	boost_sets.push_back(set_dates2);
	boost_sets.push_back(set_dates3);
	std::set<boostdate> result_set;
	result_set = more_intersection(boost_sets);
	//std::insert_iterator<std::set<boostdate>> insit = result_set.begin();
	//std::set_intersection(set_dates1.begin(),set_dates1.end(), set_dates2.begin(), set_dates2.end(), std::inserter(result_set, result_set.begin()));
	result_set.insert(boostdate(2020,Jun,10)); 
	result_set.insert(boostdate(2020,Jun,11)); 
	result_set.insert(boostdate(2020,Nov,19)); 
	for (auto date : result_set)
		std::cout << date << std::endl;
	boost::gregorian::date_period dp(boost::gregorian::date(2020,Sep,20), boost::gregorian::date(2020,Sep,25));
	std::vector<boost::gregorian::date_period> vector_periods;
	//vector_sets.push_back(result_set);
	vector_periods.push_back(dp);
	vector_periods = setToPeriods(result_set);
}

