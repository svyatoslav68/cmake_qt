/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

HolidayView::HolidayView(const int year, QWidget *parent, const int scale):QTableView(parent), _year(year), _scale(scale)
{
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	int year_days = 365 + static_cast<int>((_year % 4 == 0 && _year % 100 != 0)||(_year % 400 == 0));
	setColumnWidth(HolidayTableModel::Holidays, _scale*year_days);
}

