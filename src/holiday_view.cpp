/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

void GraphicsWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, Qt::SolidLine));
	painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	painter.drawRect(10, 4, 250,20); 
}

HolidayView::HolidayView(QWidget *parent, const int days_in_year, const int scale):QTableView(parent), _year_days(days_in_year), _scale(scale)
{
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	setColumnWidth(HolidayTableModel::Holidays, _scale*_year_days);
}
