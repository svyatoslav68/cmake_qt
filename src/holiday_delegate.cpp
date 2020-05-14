/******************************************************
*    Файл реализации делегата, реализующего   *********
*    отображение планируемых отпусков в виде  *********
*    полосок, длина которых пропорциональна   *********
*    количеству дней в каждом из отпусков     *********
*    файл - holiday_delegate.cpp               *********
******************************************************/
#include <QPainter>
#include <QModelIndex>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "holiday_delegate.hpp"
#include "holiday_table_model.hpp"

void HolidayDelegate::paint(QPainter *painter, 
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const{
	//using namespace boost::gregorian;
	const HolidayTableModel *model = 
			qobject_cast<const HolidayTableModel *>(index.model());
	int rect_width; //Width of rect cell
	int rect_height; //Height of rect cell
	rect_width = option.rect.width();	
	rect_height = option.rect.height();	
	//std::cout << rect_width << " ";
	//painter->drawText(option.rect, Qt::AlignCenter, QString::number(rect_height)+QString::number(rect_width));
	QList<QVariant> list_from_model = model->data(index, Qt::EditRole).toList();
	QString tmp_holidays="";
	for (auto &elem : list_from_model){
		using namespace boost::gregorian;
		tmp_holidays+=QString::number(elem.toMap()["begin"].toInt())+" "+QString::number(elem.toMap()["duration"].toInt())+ " "+QString::number(elem.toMap()["travel"].toInt())+":";
		//tmp_holidays = elem.second.toList();
		//painter->setBrush(Qt::LinearGradientPattern);
		painter->setPen(Qt::darkGreen);
		painter->setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
		painter->drawRect(option.rect.x()+elem.toMap()["begin"].toInt()*2, option.rect.y()+4, elem.toMap()["duration"].toInt()*2, 20);
		painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
		painter->drawRect(option.rect.x()+elem.toMap()["begin"].toInt()*2+elem.toMap()["duration"].toInt()*2, option.rect.y()+4, elem.toMap()["travel"].toInt()*2, 20);
	}
	//painter->drawText(option.rect, Qt::AlignCenter, tmp_holidays);
}

