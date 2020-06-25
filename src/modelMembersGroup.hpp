/***********************************************************
**             Модель состава группы(заголовок)           **
**               Файл modelMembersGroup.hpp               **
***********************************************************/
#pragma once
#include<QAbstractItemModel>

struct ItemOfGroup;

struct MemberGroup {
	MemberGroup():idBD(-1), name(""), parent(nullptr){}
	MemberGroup(int _idBD, ItemOfGroup *_parent = nullptr);
	MemberGroup(int _idBD, std::string _name, ItemOfGroup *_parent = nullptr):idBD(_idBD), name(_name), parent(_parent){}
	MemberGroup(const MemberGroup &);
	~MemberGroup();
	int idBD;
	std::string name;
	ItemOfGroup *parent;
};

struct ItemOfGroup {
	ItemOfGroup(std::string _name);
	ItemOfGroup(const ItemOfGroup &);
	~ItemOfGroup();
	std::string name;
	std::vector<MemberGroup>  *children;
};

class ModelGroups : public QAbstractItemModel{
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
	Q_OBJECT
public:
	ModelMembersGroup(QObject *parent = nullptr, std::vector<MemberGroup> *_members = nullptr):QAbstractItemModel(parent), members(_members){}
	~ModelMembersGroup() {}
/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	int columnCount(const QModelIndex&) const {return 1;}
	int rowCount(const QModelIndex &index = QModelIndex()) const;
	bool insertRows(int row, int column, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int column, const QModelIndex &parent = QModelIndex());
	/*************************************************/
private:
	std::vector<MemberGroup> *members;
};

