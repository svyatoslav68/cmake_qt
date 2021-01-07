/******************************************************
 *** Заголовочный файл класса отвечающего за **********
 *** разбор параметров командной строки для  **********
 *** запуска приложения. cl_parametrs.hpp    **********
 *****************************************************/
#ifndef CL_PARAMETRS_HPP
#define CL_PARAMETRS_HPP

extern "C"{
#include "mysql_connect.h"
}
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class clParametrs{
public:
	const char *name_bd = "unit";
	clParametrs();
	clParametrs(int argc, char *argv[]);
	~clParametrs();
	void setArgs(int argc, char *argv[]);
	void loadFromFile();
	void saveSettings();
	void setIdUnit(int _idUnit) {idUnit = _idUnit;}
	int getIdUnit() const {return idUnit;}
	int getYear() const {return workYear;}
	std::string getNameConfFile() const {return conf_file;}
	void setYear(int _workYear) {workYear = _workYear;}
	bool isHelp();
	bool isVersion();
	bool isSave();
	MYSQL *getDescriptorBD();
private:
	MYSQL descriptorBD;		// Дескриптор базы данных
	//std::string nameFile;
	po::options_description ops_desc; // Options description (Описание опций)
	po::positional_options_description pos_desc; // Positional options description
	po::variables_map op_store;		// Container for store received options
	/***********************************************************
	* опции, значения которых могут меняться из меню приложения*
	***********************************************************/
	int idUnit; // Идентификатор подразделения
	int workYear; // Год, с которым будет работать программа
	std::string conf_file; // Имя файла, где хранятся настройки программы
	/*********************************************************************/
	bool year_from_cl; // True, если workYear установлен явно из командной строки
};
#endif //CL_PARAMETRS_HPP
