/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <QHeaderView>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QKeyEvent>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

void GraphicsWidget::setHolidays(const std::vector<QRect> &holidays)
{
	_rectsHoliday = holidays;
}

void GraphicsWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, Qt::SolidLine));
	painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	painter.drawRect(_rectsHoliday[_currentHoliday]); 
}

void GraphicsWidget::keyPressEvent(QKeyEvent *event)
{
	//QRect rect = _rectsHoliday[_currentHoliday];
	switch (event->key()) {
		case Qt::Key_Left:
			_rectsHoliday[_currentHoliday].moveLeft(_rectsHoliday[_currentHoliday].left()-_scale);
			break;
		case Qt::Key_Right:
			_rectsHoliday[_currentHoliday].moveRight(_rectsHoliday[_currentHoliday].right()+_scale);
			break;
		default:
			QWidget::keyPressEvent(event);
	}
	update();
}

HolidayView::HolidayView(QWidget *parent, const int days_in_year, const int scale):QTableView(parent), _year_days(days_in_year), _scale(scale)
{
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	setColumnWidth(HolidayTableModel::Holidays, _scale*_year_days);
}

