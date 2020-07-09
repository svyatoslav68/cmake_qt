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

std::vector<QRect> GraphicsWidget::getHolidays()
{
	return _rectsHoliday;
}

void GraphicsWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, Qt::SolidLine));
	painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	painter.drawRect(_rectsHoliday[_currentHoliday]); 
}

bool GraphicsWidget::event(QEvent *e)
{
	if (e->type() == QEvent::KeyPress){
		std::cout << "Press Tab\n";	
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
		if (keyEvent->key() == Qt::Key_Tab){
			std::cout << "Press Tab\n";	
			_currentHoliday = ++_currentHoliday%_rectsHoliday.size();
			return true;
		}
	}
	return QWidget::event(e);
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
		case Qt::Key_Up:
			_currentHoliday = ++_currentHoliday%_rectsHoliday.size();
			std::cout << "current holiday: " << _currentHoliday << std::endl;
			break;
		default:
			QWidget::keyPressEvent(event);
	}
	update();
}

HolidayView::HolidayView(const int year, QWidget *parent, const int scale):QTableView(parent), _year(year), _scale(scale)
{
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, 50);
	setColumnWidth(HolidayTableModel::FIO, 150);
	int year_days = 365 + static_cast<int>((_year % 4 == 0 && _year % 100 != 0)||(_year % 400 == 0));
	setColumnWidth(HolidayTableModel::Holidays, _scale*year_days);
}

/*bool HolidayView::event(QEvent *e)
{
	if (e->type() == QEvent::KeyPress){
		std::cout << "From HolidayView Press Tab\n";	
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
		if (keyEvent->key() == Qt::Key_Tab){
			std::cout << "Press Tab\n";	
			return true;
		}
	}
	return QWidget::event(e);
}*/

void HolidayView ::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		std::cout << "Press Up\n";	
		break;
	default:
		QTableView::keyPressEvent(event);
	}
}
