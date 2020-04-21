/******************************************************
 *** Файл реализациии класса отвечающего за  **********
 *** разбор параметров командной строки для  **********
 *** запуска приложения. cl_parametrs.сpp    **********
 *****************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "version.hpp"
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"

clParametrs::clParametrs():idUnit(-1),workYear(-1),conf_file(std::string("settings.xml")),year_from_cl(false){
	ops_desc.add_options()
		("unit,u", po::value<int>(&idUnit), "Persons unit")
		("year,y", po::value<int>(), "Year")
		("file,f", po::value<std::string>(&conf_file)->default_value(std::string("settings.xml")), "Congiguration file (default settings.xml)")
		("help,h", "Show help")
		("save,s", "Save parametrs")
		("version,v", "Show version of application")
	;
	bd_connect(&descriptorBD, name_bd);
}

clParametrs::clParametrs(int argc, char *argv[]):clParametrs(){
	po::store(po::command_line_parser(argc, argv).options(ops_desc).positional(pos_desc).run(), op_store);
	po::notify(op_store);
	if(op_store.count("help")){
		std::cout << "Помощь по программе:" << std::endl;
		std::cout << ops_desc << std::endl;
		std::exit(0);
	}
	if(op_store.count("version")){
		std::cout << "Версия № " << g_version << std::endl;
		std::exit(0);
	}
}

clParametrs::~clParametrs(){
	/* FIXME
	Возможно нужо предусмотреть освобождение 
	MYSQL descriptorBD */
}

void clParametrs::loadFromFile(){
	std::cout << "conf file = " << conf_file <<std::endl;
	ValuesFromXML vx(conf_file.c_str());
	int tmp;
	tmp = vx.getIntValue("unit");
	if ((tmp !=-1) && (idUnit == -1))
		idUnit = tmp;
	tmp = vx.getIntValue("year");
	if ((tmp != -1) && (!year_from_cl))
		workYear = tmp;
}

void clParametrs::saveSettings(){
	ValuesFromXML vx(conf_file.c_str());
	vx.putIntValue("unit", idUnit);
	vx.putIntValue("year", workYear);
	vx.saveSettings("FILE.SettingsApplication");
}


void clParametrs::setArgs(int argc, char *argv[]){
	po::store(po::command_line_parser(argc, argv).options(ops_desc).positional(pos_desc).run(), op_store);
	po::notify(op_store);
	if(op_store.count("help")){
		std::cout << "Помощь по программе:" << std::endl;
		std::cout << ops_desc << std::endl;
		std::exit(0);
	}
	if(op_store.count("version")){
		std::cout << "Версия № " << g_version << std::endl;
		std::exit(0);
	}
	/* Поле idUnit также будем сохранять в конструкторе cl_Parametrs 
	if(op_store.count("unit"))
		idUnit=op_store["unit"].as<int>();
	*/
	if(op_store.count("year")){
		workYear = op_store["year"].as<int>();
		year_from_cl = true;
	}
	else {
		using bt=boost::gregorian::date;
		bt today = boost::gregorian::day_clock::local_day();
		workYear = today.year();
	}
	/* Этот участок кода, возможно, больше не понадобится, если переменную conf_file
	получится сохранять в конструкторе cl_Parametrs
	if(op_store.count("file")) 
		conf_file = op_store["file"].as<std::string>();*/
	if(op_store.count("save")){
	/* Сохраняем значения параметров параметров командной строки в
	конфигурационном файле settings.xml */
		saveSettings();
	}
}

bool clParametrs::isHelp(){
	return op_store.count("help");
}

bool clParametrs::isVersion(){
	return op_store.count("version");
}

bool clParametrs::isSave(){
	return op_store.count("save");
}

MYSQL *clParametrs::getDescriptorBD(){
/*Нужно перед возвращение дескриптора проверить, указывает ли он 
на реально подключенную базу данных. Если нет, то выдавать исключение.*/
	return &descriptorBD;
}


