/*******************************************
***  Заголовок класса сотрудника TPerson  **
***      Файл tperson.hpp                 **
*******************************************/
#pragma once
extern clParametrs appParametrs;

class TPerson {
public:
	TPerson();
	TPerson(const TPerson &p);
	TPerson(int id);
	TPerson(int id, std::string _family, std::string _name, std::string _parent);
	~TPerson();
	int getId() const {return dataId;}
	void displayCard();
	std::string getFamilyIO() const;
private:
	const std::string *templateCardPersonSQL;
	int dataId;		// Идентификатор записи о сотруднике в БД
	std::string family;
	std::string name;
	std::string parent;
};



