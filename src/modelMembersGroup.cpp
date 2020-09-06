/***********************************************************
**             Модель состава группы(реализация)          **
**               Файл ModelGroups.cpp               **
***********************************************************/
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
#include <str_from_file.hpp>
#include <boost/lexical_cast.hpp>
#include "cl_parametrs.hpp"
#include "tperson.hpp"
#include "modelMembersGroup.hpp"

extern clParametrs appParametrs; 

ConditionOfGroup::ConditionOfGroup(std::pair<std::string, int> condition, ItemOfGroup *_parent):number_members(condition.second), parent(_parent){
	if (condition.first == "great"){
		symbol_functor = '>';
		return;
	}
	if (condition.first == "less"){
		symbol_functor = '<';
		return;
	}
	if (condition.first == "equal"){
		symbol_functor = '=';
		return;
	}
}


MemberGroup::MemberGroup(int _idBD, ItemOfGroup *_parent):idBD(_idBD), parent(_parent){
	TPerson person(_idBD);
	name = person.getFamilyIO();
	//std::cout << "id: " << idBD << ", name = " << name << std::endl;
}

MemberGroup::MemberGroup(const MemberGroup &member){
	idBD = member.idBD;
	name = member.name;
	parent = member.parent;//new ItemOfGroup(*(member.parent));
}

MemberGroup::~MemberGroup(){
	//delete parent;
}

ItemOfGroup::ItemOfGroup(std::string _name):name(_name){
		children = new std::vector<MemberGroup>;
		conditions = new std::vector<ConditionOfGroup>;
}

ItemOfGroup::ItemOfGroup(const ItemOfGroup &group){
	name = group.name;
	children = new std::vector<MemberGroup>(group.children->begin(), group.children->end());
	conditions = new std::vector<ConditionOfGroup>(group.conditions->begin(), group.conditions->end());
	//copy(group.children->begin(), group.children->end(), children->begin());
}

ItemOfGroup::~ItemOfGroup(){
	delete conditions;
	delete children;
}

Qt::ItemFlags ModelGroups::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;// = QAbstractItemModel::flags(index);
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled| Qt::ItemIsSelectable;
	}
	return theFlags;
}

bool ModelGroups::insertRows(int row, int count, const QModelIndex &){
	beginInsertRows(QModelIndex(), row, row+count-1);
	auto it = members.cbegin();
	members.insert(it+row, count, ItemOfGroup(""));
	endInsertRows();
	return true;
}

bool ModelGroups::removeRows(int row, int count, const QModelIndex &){
	beginRemoveRows(QModelIndex(), row, row+count-1);
	auto it = members.cbegin();
	for (int i = 0; i < count; ++i){
		members.erase(it+row);
	}
	endRemoveRows();
	return true;
}

int ModelGroups::rowCount(const QModelIndex &index) const {
	return members.size();
}

QModelIndex ModelGroups::parent(const QModelIndex &child) const {
	if (!child.isValid())
		return QModelIndex();
	return QModelIndex();
}

QModelIndex ModelGroups::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();
	ItemOfGroup *group = const_cast<ItemOfGroup *>(&members[row]);
	QModelIndex result =  createIndex(row, column, group);
	return result;
}

QVariant ModelGroups::data(const QModelIndex &index, int role) const{
	if (!index.isValid() || index.row() < 0 || index.row() > members.size())
		return QVariant();
	if (role == Qt::DisplayRole){
		return members[index.row()].name.c_str();
	}
	return QVariant();
}

bool ModelGroups::setData(const QModelIndex &index, const QVariant &value, int role){
	if (!index.isValid() || index.row() < 0 || index.row() > members.size())
		return false;
	ItemOfGroup &item = members[index.row()];
	item.name = value.toString().toStdString();
	emit dataChanged(index, index);
	return true;
}

int ModelGroups::loadFromXML(){
	ValuesFromXML vx = ValuesFromXML(appParametrs.getNameConfFile().c_str());
	auto vectorNames = vx.getNamesGroup("FILE.Groups");
	for (auto name_group: vectorNames){
		/* Создаем группу*/
		ItemOfGroup item_group(name_group);
		//std::cout << "Create group " << item_group.name << std::endl;
		/* Заполняем вектор списка членов группы */
		for (auto member : vx.getMembersGroup("FILE.Groups", name_group)){
			MemberGroup item_member(boost::lexical_cast<int>(member), &item_group);
			//std::cout << "Create member " << item_member.name << std::endl;
			item_group.children->push_back(item_member);
		}
		for (auto pair_condition : vx.getConditionsGroup("FILE.Groups", name_group)){
			ConditionOfGroup item_condition(pair_condition, &item_group);
			item_group.conditions->push_back(item_condition);
		}
		/* Присваиваем соотвествующему полю указатель на вектор списка членов */
		members.push_back(item_group);
	}
	return vectorNames.size();
}

std::vector<ItemOfGroup> ModelGroups::getMembers() const{
	return members;
}

Qt::ItemFlags ModelMembersGroup::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;// = QAbstractItemModel::flags(index);
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled| Qt::ItemIsSelectable;
	}
	return theFlags;
}

QModelIndex ModelMembersGroup::index(int row, int column, const QModelIndex &parent) const{
	if (row < 0 || column != 0)
		return QModelIndex();
	/* Указатель на MemberGroup члена группы передаем в возвращаемый индекс */
	return createIndex(row, column, nullptr); // FIXME!!!
}

QModelIndex ModelMembersGroup::parent(const QModelIndex &child) const{
	return QModelIndex();
}

QVariant ModelMembersGroup::data(const QModelIndex &index, int role) const{
	if (!index.isValid() || index.row() < 0 || index.row() > members->size())
		return QVariant();
	if (role == Qt::DisplayRole){
		return members->at(index.row()).name.c_str();
	}
	if (role == Qt::UserRole) {
		return QVariant();
	}
	return QVariant();
}

bool ModelMembersGroup::insertRows(int row, int count, const QModelIndex &){
	beginInsertRows(QModelIndex(), row, row+count-1);
	auto it = members->cbegin();
	members->insert(it+row, count, MemberGroup());
	endInsertRows();
	return true;
}

bool ModelMembersGroup::removeRows(int row, int count, const QModelIndex &){
	beginRemoveRows(QModelIndex(), row, row+count-1);
	auto it = members->cbegin();
	for (int i = 0; i < count; ++i){
		members->erase(it+row);
	}
	endRemoveRows();
	return true;
}

int ModelMembersGroup::rowCount(const QModelIndex&) const{
	return members->size();
}

bool ModelMembersGroup::setData(const QModelIndex &index, const QVariant &value, int role){
	if (!index.isValid() || index.row() < 0 || index.row() > members->size())
		return false;
	MemberGroup &item = members->at(index.row());
	item.idBD = value.toStringList().at(0).toInt();
	item.name = value.toStringList().at(1).toStdString();
	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags ModelConditionsGroup::flags(const QModelIndex &index) const {
	Qt::ItemFlags theFlags;
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled| Qt::ItemIsSelectable;
	}
	return theFlags;
}

QModelIndex ModelConditionsGroup::index(int row, int column, const QModelIndex &parent) const {
	if (row < 0 || column != 0)
		return QModelIndex();
	/* Указатель на ConditionOfGroup группы передаем в возвращаемый индекс */
	ConditionOfGroup *condition = const_cast<ConditionOfGroup *>(&conditions->at(row));
	return createIndex(row, column, condition); 
}

QModelIndex ModelConditionsGroup::parent(const QModelIndex &child) const {
	return QModelIndex();
}

QVariant ModelConditionsGroup::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || index.row() < 0 || index.row() > conditions->size())
		return QVariant();
	if (role == Qt::DisplayRole){
		ConditionOfGroup con = conditions->at(index.row());
		QString strCondition = QString("%1 %2").arg(con.symbol_functor).arg(con.number_members);
		return QVariant(strCondition);
	}
	return QVariant();
}

bool ModelConditionsGroup::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (!index.isValid() || index.row() < 0 || index.row() > conditions->size())
		return false;
	ConditionOfGroup &condition = conditions->at(index.row());
	condition.symbol_functor = value.toList()[0].toChar().unicode();
	condition.number_members = value.toList()[1].toInt();
	emit dataChanged(index, index);
	return true;
}

int ModelConditionsGroup::rowCount(const QModelIndex &index) const{
	return conditions->size();
}

bool ModelConditionsGroup::insertRows(int row, int count, const QModelIndex &parent){
	beginInsertRows(QModelIndex(), row, row+count-1);
	auto it = conditions->cbegin();
	conditions->insert(it+row, count, ConditionOfGroup());
	endInsertRows();
	return true;
}

bool ModelConditionsGroup::removeRows(int row, int count, const QModelIndex &parent){
	beginRemoveRows(QModelIndex(), row, row+count-1);
	auto it = conditions->cbegin();
	for (int i = 0; i < count; ++i){
		conditions->erase(it+row);
	}
	endRemoveRows();
	return true;
}

