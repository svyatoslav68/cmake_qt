/**********************************************************
 ****   Файл реализации класса элемента редактирования *****
 ****        отпусков. файл graphic_widget.cpp          ****
 *********************************************************/
#include <iostream>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QKeyEvent>
#include "graphic_widget.hpp"

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
	painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
	painter.drawRect(_rectsHoliday[(_currentHoliday +1)%_rectsHoliday.size()]); 
}

void GraphicsWidget::keyPressEvent(QKeyEvent *event)
{
	//QRect rect = _rectsHoliday[_currentHoliday];
	switch (event->key()) {
		case Qt::Key_Left:
			if (event->modifiers() & Qt::ShiftModifier){
				_rectsHoliday[_currentHoliday].setWidth(_rectsHoliday[_currentHoliday].width()-_scale);
				_rectsHoliday[(_currentHoliday +1)%_rectsHoliday.size()].setWidth(_rectsHoliday[(_currentHoliday +1)%_rectsHoliday.size()].width()+_scale);
			}
			else
				_rectsHoliday[_currentHoliday].moveLeft(_rectsHoliday[_currentHoliday].left()-_scale);
			break;
		case Qt::Key_Right:
			if (event->modifiers() & Qt::ShiftModifier){
				_rectsHoliday[_currentHoliday].setWidth(_rectsHoliday[_currentHoliday].width()+_scale);
				_rectsHoliday[(_currentHoliday +1)%_rectsHoliday.size()].setWidth(_rectsHoliday[(_currentHoliday +1)%_rectsHoliday.size()].width()-_scale);
			}
			else
				_rectsHoliday[_currentHoliday].moveRight(_rectsHoliday[_currentHoliday].right()+_scale);
			break;
		case Qt::Key_Up:
			_currentHoliday = ++_currentHoliday%_rectsHoliday.size();
			std::cout << "current holiday: " << _currentHoliday << std::endl;
			break;
		/*case Qt::Key_Return:
			std::cout << "Press Return\n";
			break;*/
		default:
			QWidget::keyPressEvent(event);
	}
	update();
}

