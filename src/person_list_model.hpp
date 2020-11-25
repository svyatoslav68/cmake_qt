/*******************************************
 *    Заголовок класса списковой модели    *
 * списка личного состава. PersonListModel *
 *		Файл: person_list_model.hpp        *
 ******************************************/
#pragma once
#include "person_model.hpp"
class QVariant;

class PersonListModel : public PersonModel{
public:
	PersonListModel(PersonModel::MODE mode, QObject *parent = nullptr);
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation, int role) const;
private:
};
