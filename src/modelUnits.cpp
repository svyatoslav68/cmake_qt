/************************************************
*	  Реализация модели дерева организаций      *
*			   Файл modelUnits.cpp              *
************************************************/

#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <iostream>
#include <map>
#include <string>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "modelUnits.hpp"

extern clParametrs appParametrs;

UnitsTreeView::UnitsTreeView(QWidget *parent):QTreeView(parent)
{
}

void UnitsTreeView::setSizeColumns()
{
	/*setColumnWidth(ModelUnits::C_NAME, 50);
	setColumnWidth(ModelUnits::C_PARENT, 150);*/
	header()->resizeSection(ModelUnits::C_NAME, 250);
	//header()->setSectionResizeMode(ModelUnits::C_NAME, QHeaderView::Stretch);
	header()->resizeSection(ModelUnits::C_PARENT, 50);
	//header()->setSectionResizeMode(ModelUnits::C_PARENT, QHeaderView::Fixed);
	//header()->resizeSection(ModelUnits::C_NAME, 150);
}

ModelUnits::TUnit::TUnit(int idUnit, TUnit *parentUnit):_idUnit(idUnit),_parentUnit(parentUnit)
{
	if (_idUnit != -1 ) {
		std::string _template_SQL_Unit = ValuesFromXML(appParametrs.getNameConfFile().c_str()).getStrSQL("FILE.SQL", "TUnits", "getPropertyUnit");
		MYSQL_RES *data_from_BD = nullptr;
		std::stringstream ss;
		ss << boost::format(_template_SQL_Unit)%_idUnit << std::flush;
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
			_name = row[1];
			_full_name = row[2];
			_level = boost::lexical_cast<int>(row[3]);
			_parent_int = boost::lexical_cast<int>(row[4]);
		}
	}
}

std::string ModelUnits::TUnit::getNameUnit() const
{
	return _name;
}

std::string ModelUnits::TUnit::getFullNameUnit() const
{
	return _full_name;
}

bool ModelUnits::TUnit::operator==(const TUnit &arg) const{
	return this->_idUnit == arg._idUnit;
}

int ModelUnits::TUnit::getNumRowInParent() const
{
	if (_parentUnit){
		auto find_iterator = find(_parentUnit->_children.cbegin(), _parentUnit->_children.cend(), this);
		if (find_iterator == _parentUnit->_children.cend())
			return 0;
		else{
			return std::distance(_parentUnit->_children.cbegin(), find_iterator);;
		}
	}
	else
		return 0;
}

ModelUnits::TUnit *ModelUnits::TUnit::getParent() const
{
	return _parentUnit;
}

void ModelUnits::TUnit::addChild(TUnit *child){
	_children.push_back(child);
}

ModelUnits::TUnit *ModelUnits::TUnit::getChild(int row) const
{
	return _children.at(row);
}

ModelUnits::ModelUnits():rootUnit(nullptr),
			_SQL_Fill(ValuesFromXML(appParametrs.getNameConfFile().c_str()).
			getStrSQL("FILE.SQL", "TUnits", "getNameUnits"))

{
	fillContent();
}

ModelUnits::~ModelUnits()
{
}

ModelUnits::TUnit *ModelUnits::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		TUnit *item = static_cast<TUnit*>(index.internalPointer());
		if (item) return item;
	}
	return rootUnit;
}

QVariant ModelUnits::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole){
		if (orientation == Qt::Horizontal){
			switch (section){
			case C_NAME:
				return QVariant("Название");		
			case C_PARENT:
				return QVariant("В составе");
			default: return QVariant();
			}
		}
	}
	return QVariant();
}

Qt::ItemFlags ModelUnits::flags(const QModelIndex &index) const
{
	Qt::ItemFlags theFlags;// = QAbstractItemModel::flags(index);
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled|Qt::ItemIsSelectable;
	}
	return theFlags;
}

QModelIndex ModelUnits::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}
	if (!rootUnit)
		return QModelIndex();
	TUnit *parentUnit = getItem(parent);
	if (TUnit *unit = parentUnit->getChild(row))
		return createIndex(row, column, unit);
	return QModelIndex();
}

QModelIndex ModelUnits::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();
    if (TUnit *childUnit = getItem(index)){
		if (TUnit *parentUnit = childUnit->getParent()) {
			if (parentUnit == rootUnit)
				return QModelIndex();
			if (TUnit *grandParentUnit = parentUnit->getParent()){
				return createIndex(parentUnit->getNumRowInParent(), 0, parentUnit);
			}
		}
	}
	return QModelIndex();
}

QVariant ModelUnits::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    TUnit *item = static_cast<TUnit *>(index.internalPointer());
	switch (role){
		case Qt::DisplayRole:
			switch (index.column()){
				case C_NAME: return QVariant(item->getNameUnit().c_str());
				case C_PARENT: 
					TUnit *parentUnit = item->getParent();
					if (parentUnit)
						return QVariant(parentUnit->getIdUnit());
					else
						return QVariant();
			}
		case Qt::UserRole:
			return item->getIdUnit();
		default:
			return QVariant();
	}
	return QVariant();
}

/*bool ModelUnits::setData(const QModelIndex &index, const QVariant &value, int role)
{
}

bool ModelUnits::insertRows(int row, int column, const QModelIndex &parent)
{
}

bool ModelUnits::removeRows(int row, int column, const QModelIndex &parent)
{
}*/

int ModelUnits::columnCount(const QModelIndex &index) const 
{
	//return index.isValid() ? NUMBER_COLUMNS : 1;
	return NUMBER_COLUMNS;
}

int ModelUnits::rowCount(const QModelIndex &parent) const
{
//	return parent.isValid() ? 0 : _content.size();
	if (!parent.isValid()) // && parent.column() != 0)
		return rootUnit->childrenCount();
	TUnit *parentUnit = getItem(parent);
	return (parentUnit != rootUnit) ? parentUnit->childrenCount() : 0;
}

void ModelUnits::fillContent()
{
	MYSQL_RES *data_from_BD = nullptr;
	int mysql_status = 0;
	mysql_status = mysql_query(appParametrs.getDescriptorBD(), _SQL_Fill.c_str());
	if (mysql_status){
		std::cout << "Ошибка при выполнении запроса: " << _SQL_Fill << std::endl;
	}
	else {
		rootUnit = new TUnit();
		std::map<int, TUnit*> buffer; // временный кэш, необходимый для заполнения content
		data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
		MYSQL_ROW row;
		row = mysql_fetch_row(data_from_BD);
		while (row){
			if (boost::lexical_cast<int>(row[T_PARENT]) < 1){
				TUnit *unit = new TUnit(boost::lexical_cast<int>(row[T_IDUNIT]), rootUnit);
				buffer[boost::lexical_cast<int>(row[T_IDUNIT])] = unit;
				rootUnit->addChild(unit);
				_content.push_back(unit);
			}
			else {
				TUnit *parentUnit = buffer[boost::lexical_cast<int>(row[T_PARENT])];
				TUnit *unit = new TUnit(boost::lexical_cast<int>(row[T_IDUNIT]), parentUnit);
				buffer[boost::lexical_cast<int>(row[T_IDUNIT])] = unit;
				parentUnit->addChild(unit);
			}
			row = mysql_fetch_row(data_from_BD);
		}
	}
}
