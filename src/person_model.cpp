/*******************************************
 * Реализация класса PersonModel           *
 * Файл person_model.cpp				   *
 * ****************************************/
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <QModelIndex>
#include <QVariant>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "str_from_file.hpp"
#include "cl_parametrs.hpp"
#include "tperson.hpp"
#include "person_model.hpp"

extern clParametrs appParametrs;

PersonModel::PersonModel(QObject *parent):QAbstractItemModel(parent),Template_SQL_Fill(ValuesFromXML(appParametrs.getNameConfFile().c_str()).
	getStrSQL("FILE.SQL", "ListPerson", "getMilitaryPersonsUnit")){
}

PersonModel::PersonModel(MODE mode, QObject *parent):PersonModel(parent) {
	if (mode == SQL) {
		MYSQL_RES *data_from_BD = nullptr;
		std::stringstream ss;
		ss << boost::format(Template_SQL_Fill)%appParametrs.getIdUnit() << std::flush;
		std::string SQL = ss.str();
		int mysql_status = 0;
		mysql_status = mysql_query(appParametrs.getDescriptorBD(), SQL.c_str());
		std::cout << "SQL = " << SQL.c_str() << std::endl;
		if (mysql_status){
			std::cout << "Ошибка при выполнении запроса: " << SQL << std::endl;
		}
		else {
			data_from_BD = mysql_store_result(appParametrs.getDescriptorBD());
			MYSQL_ROW row;
			row = mysql_fetch_row(data_from_BD);
			while (row){
				content.push_back(std::make_tuple(boost::lexical_cast<int>(row[0]), row[1], row[2]));
				row = mysql_fetch_row(data_from_BD);
			}
			delete data_from_BD;
		}
	}
	else if (mode == TXT) {
		//ValuesFromXML PersonsFile("holidays.xml");			
	}
	else {
		std::cerr << "Constructor PersonModel executed with incorrect parametrs\n";
	}
}

PersonModel::~PersonModel(){

}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const{
	Qt::ItemFlags theFlags;
	if (index.isValid()){
		theFlags |= Qt::ItemIsEnabled|Qt::ItemIsSelectable;
	}
	return theFlags;
}

QVariant PersonModel::data(const QModelIndex &index, int role) const{
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > COLUMN_COUNT)
		return QVariant();
	if (role == Qt::DisplayRole){
		auto [ id, position, fio ] = content[index.row()];
		switch (index.column()){
			case ID: return id;
			case Position: return position.c_str();
			case FIO: return fio.c_str();;
			default: return QVariant();
		}
	}
	return QVariant();
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const{
	if (role == Qt::DisplayRole){
	if (orientation == Qt::Vertical)
		return QVariant(section+1);
	if (orientation == Qt::Horizontal){
		switch (section){
			case ID: return "№ п/п";
			case Position: return "Должность";
			case FIO: return "ФИО";
			default: return QVariant();
		}
	}
	}
	return QVariant();
}

int PersonModel::rowCount(const QModelIndex& index) const{
	return index.isValid() ? 0 : content.size();
}

int PersonModel::columnCount(const QModelIndex &index) const{
	return index.isValid() ? 0 : COLUMN_COUNT;
}

QModelIndex PersonModel::index ( int row, int column, const QModelIndex &parent) const{
	return createIndex(row, column);
}


QModelIndex PersonModel::parent ( const QModelIndex & ) const{
	return QModelIndex();
}

bool PersonModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row+count - 1);
	auto it = content.cbegin();
	for (int i = row; i < row+count; ++i){
		content.erase(it+i);
	}
	endRemoveRows();
	return true;
}

void PersonModel::addPerson(int codPerson, std::string pos, std::string FIO){
	std::tuple<int, std::string, std::string>	person {codPerson, pos, FIO};
	content.push_back(person);
	emit dataChanged(createIndex(content.size() - 1, 0), createIndex(content.size() - 1, 0));
}
