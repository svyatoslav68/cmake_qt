/************************************************
*	   Заголовок модели дерева организаций      *
*			   Файл modelUnits.hpp              *
************************************************/
#ifndef MODELUNITS_HPP
#define MODELUNITS_HPP

#include <QAbstractItemModel>
#include <QTreeView>

class QModelIndex;
class QVariant;
class QWidget;

class UnitsTreeView: public QTreeView {
public:
	UnitsTreeView(QWidget *parent = nullptr);
	void setSizeColumns();
private:
};

class ModelUnits: public QAbstractItemModel
{
public:
	enum {
		C_NAME = 0,
		C_PARENT =1
	} columns; // Заголовки представления
	ModelUnits();
	~ModelUnits();
	/* Методы QAbstractItemModel, которые необходимо переопределять 
	 * в классе-потомке*/
	QVariant headerData(int section, Qt::Orientation, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	/*bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool insertRows(int row, int column, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int column, const QModelIndex &parent = QModelIndex());*/
	int columnCount(const QModelIndex &index=QModelIndex()) const override;
	int rowCount(const QModelIndex &index=QModelIndex()) const override;
private:
	const int NUMBER_COLUMNS = 2;
	class TUnit;
	TUnit *rootUnit;
	typedef std::vector<TUnit *> TVectorUnits;
	class TUnit
	/* Класс описывает члена содержимого модели подразделений.
	Так как, все подразделения описываются в виде леса, то 
	класс должен содержать ссылки на родительское подразделение*/
	{
	public:
		explicit TUnit(int idUnit = -1, TUnit *parentUnit = nullptr);
		int childrenCount() {return _children.size();}
		std::string getNameUnit() const;
		std::string getFullNameUnit() const;
		int getIdUnit() const {return _idUnit;}
		TUnit *getParent() const;
		int getNumRowInParent() const;
		bool operator==(const TUnit &arg) const;
		void addChild(TUnit *child);
		TUnit *getChild(int row) const;
	private:
		int _idUnit;
		std::string _name;
		std::string _full_name;
		int _parent_int;
		int _level;
		TUnit *_parentUnit;
		TVectorUnits _children;
	};
	TUnit *getItem(const QModelIndex &index) const;
	enum {
			T_IDUNIT = 0,
			T_NAME = 1,
			T_FULLNAME = 2,
			T_LEVEL = 3, 
			T_PARENT = 4,
			T_NUMBER = 5,
			T_TYPE = 6,
			T_ORDER = 7
	} fields; // Перечисление полей запроса
	const std::string _SQL_Fill;
	TVectorUnits _content;
	void fillContent();
};
#endif //MODELUNITS_HPP
