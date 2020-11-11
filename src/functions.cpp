#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "functions.hpp"

std::vector<boost::gregorian::date_period> setToPeriods(std::set<boost::gregorian::date> input_set){
	std::vector<boost::gregorian::date_period> result;
	using boost_date = boost::gregorian::date;
	/*auto begin_period = input_set.begin();
	boost_date prev_date(begin_period);
	boost_date end_period(begin_period);
	boost_date current_date(begin_period);
	current_date++;
	while (current_date != input_set.end()){
		if(*current_date == *prev_date + boost::gregorian::days(1)) {
			continue;
		}
	}*/
	std::set<boost::gregorian::date>::iterator current_date = input_set.begin();
	boost_date prev_date(*current_date);
	boost_date begin_of_period(*current_date);
	boost_date end_of_period(prev_date);
	//current_date++;
	while (current_date != input_set.end()) {
		//std::cout << *current_date << std::endl;
		current_date++;
		if (*current_date == prev_date  + boost::gregorian::days(1)){
			prev_date = *current_date;
			continue;
		}
		else {
			end_of_period = boost_date(prev_date);
			//std::cout << "begin_of_period in cicle = " << begin_of_period << std::endl;
			//std::cout << "end_of_period in cicle = " << end_of_period << std::endl;
			result.push_back(boost::gregorian::date_period(begin_of_period, end_of_period + boost::gregorian::days(1)));
			begin_of_period=*current_date;
			prev_date = *current_date;
		}
		//*current_date = *current_date + boost::gregorian::days(1);
		//current_period++;
	}
	prev_date  - boost::gregorian::days(1);
	/*end_of_period = boost_date(prev_date);
	std::cout << "begin_of_period = " << begin_of_period << std::endl;
	std::cout << "end_of_period = " << end_of_period << std::endl;
	result.push_back(boost::gregorian::date_period(begin_of_period, end_of_period + boost::gregorian::days(1)));*/
	//std::cout << "Content of vector of date_period\n";
	/*for (boost::gregorian::date_period current_period : result){	
		std::cout << current_period <<std::endl;
	}*/
	return result;
}
