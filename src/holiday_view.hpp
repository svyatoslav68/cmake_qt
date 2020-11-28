/******************************************************
 *     Заголовочный файл для создания класса        ***
 *     табличного отображения. 						***
 *     Файл holiday_view.hpp							***
 *****************************************************/
#ifndef MYTABLEVIEW_HPP
#define MYTABLEVIEW_HPP

#include <QHeaderView>
#include <QTableView>

class HolidayHeader : public QHeaderView {
public:
	explicit HolidayHeader(QWidget *parent = nullptr);
private:
	void paintEvent(QPaintEvent *event);
	void paintHeader(QPainter *painter, const int RowHeight);
	std::array<int, 12> daysInMonth;
	std::array<std::string, 12> namesMonth = {"Янв.","Фев.","Мар.", "Апр.", "Май", "Июнь", "Июль", "Авг.", "Сен.", "Окт.", "Ноя.", "Дек."};
};

class HolidayView : public QTableView{
public:
	enum {WIDTH_NUMBER_COL=50, WIDTH_FIO_COL = 150};
	explicit HolidayView(const int year, QWidget *parent = nullptr, const int scale = 2);
	void setSizeColumns();
	const int getYear() {return _year;}
	const int getScale() {return _scale;}
private:
	HolidayHeader *horHeader;
	const int _year;
	const int _scale;
};

#endif //MYTABLEVIEW_HPP
