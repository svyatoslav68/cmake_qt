/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

HolidayView::HolidayView(QWidget *parent, int days_in_year):QTableView(parent), year_days(days_in_year){
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	setColumnWidth(HolidayTableModel::Holidays, 2*year_days);
}
