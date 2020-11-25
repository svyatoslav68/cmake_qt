/*******************************************
*** Реализация класса TPerson             **
***      Файл tperson.cpp                 **
*******************************************/
#include <iostream>
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "tperson.hpp"

using std::string;
TPerson::TPerson()//:templateCardPersonSQL(nullptr)
{
}

TPerson::TPerson(const TPerson &p)//:templateCardPersonSQL(nullptr)
{
	//MYSQL_RES *data_from_BD;
	dataId = p.dataId;
	family = p.family;
	name = p.name;
	parent = p.parent;
}

TPerson::TPerson(int id)//:templateCardPersonSQL(nullptr),dataId(id)
{
	std::string templateCardPersonSQL(ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "TPerson", "getDataPerson"));
	MYSQL_RES *data_from_BD;
	std::stringstream ss;
	try{
	boost::format fmter(templateCardPersonSQL);
	ss << fmter%id;
	}
	catch (...){
		std::cout << "From listPersons TPerson()\n";
	}
	string SQL = ss.str();
	SQL = ss.str();
	//std::cout << "Выполняется запрос :" << SQL << std::endl;
	int mysql_status = 0;
	mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
	if (mysql_status){
		std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
	}
	else {
		data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
		MYSQL_ROW row;
		row = mysql_fetch_row(data_from_BD);
		family = row[1];
		name = row[2];
		parent = row[3];
		delete data_from_BD;
	}
}

TPerson::TPerson(int id, string _family, string _name, string _parent):dataId(id), family(_family), name(_name), parent(_parent){
}

TPerson::~TPerson()
{
}

string TPerson::getFamilyIO() const{
	return family + " " + name;
}

void TPerson::displayCard(){
	//ListPersons::const_iterator it = listPersons.findRecordId(recordId);
	//MYSQL descriptorBD = listPersons.getDescriptorBD();
	/*char cardPersonSQL[strlen(templateCardPersonSQL)+10];
	sprintf(cardPersonSQL, templateCardPersonSQL, getId());
	std::cout << "SQL =" << cardPersonSQL << std::endl;*/
	MYSQL_RES *data_from_BD;
	if (!data_from_BD)
		return;
	std::cout << "Данные о сотруднике:" << std::endl;
	MYSQL_ROW row;
	row = mysql_fetch_row(data_from_BD);
	std::cout << "звание: " << row[14] << std::endl;
	std::cout << "фамилия: " << row[1] << std::endl;
}

