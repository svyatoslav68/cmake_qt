/******************************************************
*    Файл реализации делегата, реализующего   *********
*    отображение планируемых отпусков в виде  *********
*    полосок, длина которых пропорциональна   *********
*    количеству дней в каждом из отпусков     *********
*    файл - holiday_delegate.cpp               *********
******************************************************/
#include <QPainter>
#include <QModelIndex>
#include "holiday_delegate.hpp"
#include "holiday_table_model.hpp"

void HolidayDelegate::paint(QPainter *painter, 
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const{
	const HolidayTableModel *model = 
			qobject_cast<const HolidayTableModel *>(index.model());
	int rect_width; //Width of rect cell
	int rect_height; //Height of rect cell
	rect_width = option.rect.width();	
	rect_height = option.rect.height();	
	//std::cout << rect_width << " ";
	painter->drawText(option.rect, Qt::AlignCenter, QString::number(rect_height)+QString::number(rect_width));
}

