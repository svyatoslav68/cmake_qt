/***********************************************************************
******** Файл реализации класса HolidayTableModel **********************
********     файл holiday_table_model.cpp         **********************
***********************************************************************/
#include <utility>
#include <map>
#include <vector>
#include <string>
#include <QVariant>
#include <QGraphicsScene>
#include "holiday_table_model.hpp"

using std::pair;
using std::vector;
using std::map;

HolidayTableModel::HolidayTableModel(vector<pair<int, std::string>> persons){

}

HolidayTableModel::HolidayTableModel(std::string SQL_persons){

}

Qt::ItemFlags HolidayTableModel::flags(const QModelIndex &index) const{

}

QVariant HolidayTableModel::data(const QModelIndex &index, int role) const{

}

QVariant HolidayTableModel::headerData(int section, Qt::Orientation, int role) const{

}

