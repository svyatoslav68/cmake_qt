/***********************************************************
**             Модель состава группы(заголовок)           **
**               Файл modelMembersGroup.hpp               **
***********************************************************/
#pragma once
#include<QAbstractItemModel>
#include <boost/date_time/gregorian/gregorian.hpp>

using bt=boost::gregorian::date;

struct ItemOfGroup;

struct ConditionOfGroup {
	/* Класс условия для группы */
	ConditionOfGroup():symbol_functor(0x00), number_members(0), parent(nullptr) {}
	ConditionOfGroup(std::pair<std::string, int> condition, ItemOfGroup *_parent = nullptr);
	ConditionOfGroup(char _symbol_functor, int _number, ItemOfGroup *_parent = nullptr):symbol_functor(_symbol_functor), number_members(_number), parent(_parent) {}
	char symbol_functor;/* Символ оператора ('=','<','>') */
	int number_members; /* Количество сотрудников */
	ItemOfGroup *parent;
};

struct MemberGroup {
	/* Класс члена группы */
	MemberGroup():idBD(-1), name(""), parent(nullptr){}
	MemberGroup(int _idBD, ItemOfGroup *_parent = nullptr);
	MemberGroup(int _idBD, std::string _name, ItemOfGroup *_parent = nullptr):idBD(_idBD), name(_name), parent(_parent){}
	MemberGroup(const MemberGroup &);
	~MemberGroup();
	int idBD; /* Поле содержит уникальный ключ записи в базе данных, 
	соответствующий сотруднику-члену группы */
	std::string name;
	ItemOfGroup *parent;
};

struct ItemOfGroup {
	ItemOfGroup(std::string _name);
	ItemOfGroup(const ItemOfGroup &);
	~ItemOfGroup();
	std::string name;
	std::vector<MemberGroup>  *children;
	std::vector<ConditionOfGroup> *conditions;
	std::vector<bt> getFailDates();
};

class ModelGroups : public QAbstractItemModel{
	/* Модель содержит группы сотрудников */
	Q_OBJECT
public:
	//ModelGroups(std::string _name_group = std::string("")):name_group(_name_group){}
	ModelGroups(QObject *parent = nullptr):QAbstractItemModel(parent){}
	~ModelGroups(){}
	/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool insertRows(int row, int column, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int column, const QModelIndex &parent = QModelIndex());
	int columnCount(const QModelIndex &index=QModelIndex()) const {return 1;}
	int rowCount(const QModelIndex &index=QModelIndex()) const;
	/*************************************************/
	int loadFromXML();
	std::vector<ItemOfGroup> getMembers() const;
private:
	//std::string name_group;	
	std::vector<ItemOfGroup> members; // Id's of members group 	and family
};

class ModelMembersGroup : public QAbstractItemModel {
	/* Модель содержит имена сотрудников с условиями, которые должны выполняться 
	графиком отпусков. Например, в отпуск может идти не более одно человека из группы
	ответственных за получение имущества */
	Q_OBJECT
public:
	ModelMembersGroup(QObject *parent = nullptr, std::vector<MemberGroup> *_members = nullptr):QAbstractItemModel(parent), members(_members){}
	~ModelMembersGroup() {}//if (members) delete members;} FIXME!!!!
/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	int columnCount(const QModelIndex&) const {return 1;}
	int rowCount(const QModelIndex &index = QModelIndex()) const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
/*************************************************/
private:
	std::vector<MemberGroup> *members;
};

class ModelConditionsGroup : public QAbstractItemModel {
	/* Модель содержит условия, которые должны выполняться в группе */
	Q_OBJECT
public:
	ModelConditionsGroup(QObject *parent = nullptr, std::vector<ConditionOfGroup> *_conditions = nullptr):QAbstractItemModel(parent), conditions(_conditions) {}
	~ModelConditionsGroup() {}//if (conditions) delete conditions;}
/* Методы QAbstractItemModel, которые необходимо переопределять 
 * в классе-потомке  */
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	int columnCount(const QModelIndex&) const {return 1;}
	int rowCount(const QModelIndex &index = QModelIndex()) const;
	bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
/*************************************************/
private:
	std::vector<ConditionOfGroup> *conditions;
};

