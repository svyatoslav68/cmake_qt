/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include "holiday_view.hpp"

HolidayView::HolidayView(QWidget *parent):QTableView(parent){
	QHeaderView *ver_header = verticalHeader();
	ver_header->hide();
}
