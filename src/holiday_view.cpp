/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

HolidayView::HolidayView(QWidget *parent, const int days_in_year, const int scale):QTableView(parent), _year_days(days_in_year), _scale(scale){
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	setColumnWidth(HolidayTableModel::Holidays, _scale*_year_days);
}
