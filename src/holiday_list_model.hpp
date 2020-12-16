/************************************************************
******* Заголовочный файл класса, реализующего **************
******* модель, содержащую отпуска сотрудников.		*********
******         файл holiday_list_model.hpp         *********
************************************************************/
#pragma once

//#include "tholiday.hpp"
class THoliday;
class QModelIndex;

class HolidayListModel: public QAbstractItemModel {
	Q_OBJECT
public:
	enum MODE {SQL, TXT};
	/*HolidayTableModel(std::vector<std::string> persons);*/
	explicit HolidayListModel(int row, int cod_person, MODE mode = TXT, QObject *parent = nullptr);
	~HolidayListModel() {}
	/* Методы QAbstractItemModel, которые необходимо переопределять *********
	 * в классе-потомке            *****************************************/
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data(const QModelIndex &index, int role) const;
	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex&) const;
	QModelIndex index ( int row, int column, const QModelIndex &) const;
	QModelIndex parent ( const QModelIndex & ) const;
	/***********************************************************************/
public slots:
	void loadFromTxt();
	void loadFromBd();
private:
	int indexRow;
	static std::map<
		int,
		std::tuple<
			int, // idPerson
			std::vector<THoliday> // Вектор отпусков сотрудника
		>
	> content;
private slots:
	void clearContent();
};
