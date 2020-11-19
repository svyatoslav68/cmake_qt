#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

int main()
{
	using namespace boost::gregorian;
	date my_date(2020, Nov, 14);
	std::cout << "Date: " << my_date << std::endl;
	//std::cout << "Number: " << gregorian_calendar::day_number(my_date) << std::endl;
	std::cout << "Number: " << my_date.day_number() << std::endl;
	gregorian_calendar::ymd_type ymd = gregorian_calendar::from_day_number(2459777);
	date new_date = { ymd.year, ymd.month, ymd.day };
	std::cout << "New date: " << new_date << std::endl;
	//date new_date = from_day_number(2459777);
}
