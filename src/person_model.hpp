/*******************************************
 * Заголовок класса модели списка личного  *
 *		   состава. PersonModel.           *
 *		   Файл: person_model.hpp          *
 ******************************************/
#include <QAbstractItemModel>
#pragma once
class TPerson;
class PersonModel : public QAbstractItemModel{
public:
	enum Column {ID, Position, FIO};
	enum MODE {SQL, TXT};
	PersonModel(QObject *parent = nullptr);
	PersonModel(MODE mode, QObject *parent = nullptr);
	virtual ~PersonModel();
	/*  Переопределяемые методы *****/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent ( const QModelIndex & ) const;
	/*********************************/
	void addPerson(int codPerson, std::string pos, std::string FIO);
	std::vector<std::tuple<int, std::string, std::string>> getContent() {return content;}
private:
	static const int COLUMN_COUNT = 3;
	const std::string Template_SQL_Fill;
protected:
	std::vector<std::tuple<int, std::string, std::string>> content;
};

