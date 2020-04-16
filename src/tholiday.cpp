/* Файл реализации класса THoliday, представляющего собой
				описание отпуска сотрудника. 
				    файл  tholiday.cpp
***********************************************************/
#include <iostream>
#include <boost/format.hpp>
#include "tholiday.hpp"
#include "str_from_file.hpp"

const std::string THoliday::templateSQLHoliday=ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday");

THoliday::THoliday(){//:templateSQLHoliday(ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday")){

}

THoliday::THoliday(int cod_holiday){//:templateSQLHoliday(ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "THoliday", "getHoliday")){
using namespace boost::gregorian;
	data_from_BD = nullptr;
	boost::format fmter(templateSQLHoliday);
	std::stringstream ss;
	ss << fmter%cod_holiday;
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
		holiday_duration = date_duration(boost::lexical_cast<int>(row[4])+boost::lexical_cast<int>(row[5]));
	}
}

void THoliday::displayHoliday() const {
using namespace boost::gregorian;
	date_facet *facet(new date_facet("%d.%m.%Y"));
	std::cout.imbue(std::locale(std::cout.getloc(), facet));
	std::cout << std::setw(40) << std::setfill(' ') << std::left << name_holiday << std::setw(13) << date_begin << std::setw(3) << std::right << holiday_duration << std::endl;
}

