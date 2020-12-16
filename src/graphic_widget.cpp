/**********************************************************
 ****   Файл реализации класса элемента редактирования *****
 ****        отпусков. файл graphic_widget.cpp          ****
 *********************************************************/
#include <iostream>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QKeyEvent>
#include <QRect>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "tholiday.hpp"
#include "graphic_widget.hpp"
using bdd=boost::gregorian::date_duration;
using bt=boost::gregorian::date;

void GraphicsWidget::setHolidays(const std::vector<THoliday> &holidays)
{
	_holidays = holidays;
}

std::vector<THoliday> GraphicsWidget::getHolidays()
{
	return _holidays;
}

void GraphicsWidget::paintEvent(QPaintEvent *event)
{
	using namespace boost::gregorian;
	QPainter painter(this);
	painter.setPen(QPen(Qt::blue, Qt::SolidLine));
	painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	THoliday drawHoliday = _holidays[_currentHoliday];
	//drawHoliday.displayHoliday();
	QRect rect((drawHoliday.firstDay() - 1)*_scale, 4, (drawHoliday.numberDaysHoliday())*_scale + (drawHoliday.numberDaysTravel())*_scale, 20);
	painter.drawRect(rect);
	painter.setPen(QPen(Qt::black, Qt::SolidLine));
	QString textLengthHoliday = QString("%1%2").arg(drawHoliday.numberDaysHoliday()).arg(drawHoliday.numberDaysTravel()?QString("+%1").arg(drawHoliday.numberDaysTravel()):"");
	painter.drawText(rect, Qt::AlignCenter, textLengthHoliday); 
	painter.setPen(QPen(Qt::blue, Qt::SolidLine));
	painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
	drawHoliday = _holidays[(_currentHoliday+1)%_holidays.size()];
	rect = QRect((drawHoliday.firstDay() - 1)*_scale, 4, (drawHoliday.numberDaysHoliday())*_scale + (drawHoliday.numberDaysTravel())*_scale, 20);
	painter.drawRect(rect);
	//drawHoliday.displayHoliday();
	//painter.drawRect(_holidays[(_currentHoliday+1)%_holidays.size()]); 
}

void GraphicsWidget::keyPressEvent(QKeyEvent *event)
{
	//QRect rect = holidays[_currentHoliday];
	switch (event->key()) {
		case Qt::Key_Left:
			if (event->modifiers() & Qt::ShiftModifier){
				if (--_holidays[_currentHoliday] == true)
				//if (event->modifiers() & Qt::ControlModifier)
					++_holidays[(_currentHoliday+1)%_holidays.size()];
				//else	
				//	--_holidays[(_currentHoliday+1)%_holidays.size()];
			}
			else
				_holidays[_currentHoliday].setBeginDate(_holidays[_currentHoliday].beginDate()-bdd(1));
			break;
		case Qt::Key_Right:
			if (event->modifiers() & Qt::ShiftModifier){
				if (--_holidays[(_currentHoliday+1)%_holidays.size()] == true)
				++_holidays[_currentHoliday];
			}
			else
				_holidays[_currentHoliday].setBeginDate(_holidays[_currentHoliday].beginDate()+bdd(1));
			break;
		case Qt::Key_Up:
			_currentHoliday = ++_currentHoliday%_holidays.size();
			//std::cout << "current holiday: " << _currentHoliday << std::endl;
			break;
		/*case Qt::Key_Return:
			std::cout << "Press Return\n";
			break;*/
		case Qt::Key_Insert:
			std::cout << "Press Insert\n";
			break;
		default:
			QWidget::keyPressEvent(event);
	}
	update();
}

void GraphicsWidget::toTwo()
{

}

