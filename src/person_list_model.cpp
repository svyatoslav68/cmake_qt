/*****************************************************
*		  Реализация класс списковой модели			 *
*			   перечня личного состава				 *
*			 Файл: person_list_model.cpp
*****************************************************/
#include <iostream>
#include <QVariant>
#include <Qt>
#include "person_model.hpp"
#include "person_list_model.hpp"

PersonListModel::PersonListModel(PersonModel::MODE mode, QObject *parent):PersonModel(mode, parent)
{
}

QVariant PersonListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() < 0 || index.row() > content.size()
		|| index.column() < 0 || index.column() > 1)
		return QVariant();
	if (role == Qt::DisplayRole){
		auto [ id, position, fio ] = content[index.row()];
		//std::cout << content.size() << ":" << fio.c_str() << std::endl;
		return QString("%1:%2 %3").arg(id).arg(position.c_str()).arg(fio.c_str());
	}
	return QVariant();
}

QVariant PersonListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole){
	if (orientation == Qt::Vertical)
		return QVariant(section+1);
	if (orientation == Qt::Horizontal)
		return QVariant(QString("Ф И.О."));
	}
	return QVariant();
}
