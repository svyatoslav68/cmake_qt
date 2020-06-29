/******************************************************
*    Файл реализации делегата, реализующего   *********
*    отображение планируемых отпусков в виде  *********
*    полосок, длина которых пропорциональна   *********
*    количеству дней в каждом из отпусков     *********
*    файл - holiday_delegate.cpp               *********
******************************************************/
#include <iostream>
#include <string>
#include <QPainter>
#include <QModelIndex>
#include <QImage>
#include <QPixmap>
#include <QRect>
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "holiday_delegate.hpp"
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"
#include "365x1.xpm"

const char *HolidayDelegate::template_name_file_background = "./BackGround_%d.png";		


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
	//QString tmp_holidays="";
	if (_back_ground) {
		/*std::stringstream ss;
		ss << boost::format(std::string(template_name_file_background))%_year_days;
		//std::string name_file_background = ss.str();
		//QPixmap pixmap("/home/slava/Projects/cmake_qt/src/BackGround_366.png");
		QPixmap pixmap(ss.str().c_str());*/
		QPixmap *pixmap;
		if (_year_days == 365){
			pixmap = new QPixmap(backGround_365_1);
		}
		else {
			pixmap = new QPixmap(backGround_366_1);
		}
		QRect source(0, 0, option.rect.width(), option.rect.height());
		painter->drawPixmap(option.rect, pixmap->scaled(option.rect.width(), option.rect.height()), source);
		delete pixmap;
	}
	for (auto &elem : list_from_model){
		using namespace boost::gregorian;
		//tmp_holidays+=QString::number(elem.toMap()["begin"].toInt())+" "+QString::number(elem.toMap()["duration"].toInt())+ " "+QString::number(elem.toMap()["travel"].toInt())+":";
		//tmp_holidays = elem.second.toList();
		//painter->setBrush(Qt::LinearGradientPattern);
		painter->setPen(Qt::darkGreen);
		painter->setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
		painter->drawRect(option.rect.x()+elem.toMap()["begin"].toInt()*_scale, option.rect.y()+4, elem.toMap()["duration"].toInt()*_scale, 20);
		painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
		painter->drawRect(option.rect.x()+elem.toMap()["begin"].toInt()*_scale+elem.toMap()["duration"].toInt()*_scale, option.rect.y()+4, elem.toMap()["travel"].toInt()*_scale, 20);
	}
	//painter->drawText(option.rect, Qt::AlignCenter, tmp_holidays);
}

QWidget *HolidayDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//std::cout << "Create editor.\n";
	GraphicsWidget *widget = new GraphicsWidget(_scale, parent);
	return widget;
}

void HolidayDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void HolidayDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
	if (index.column() == HolidayTableModel::Holidays){
		std::vector<QRect> holidays;
		QList<QVariant> list_from_model = index.model()->data(index, Qt::EditRole).toList();
		for (auto &elem : list_from_model){
			QRect rect(elem.toMap()["begin"].toInt()*_scale, 4, elem.toMap()["duration"].toInt()*_scale+elem.toMap()["travel"].toInt()*_scale, 20);
			holidays.push_back(rect);
		}
		GraphicsWidget *graphicEditor = static_cast<GraphicsWidget *>(editor);
		graphicEditor->setHolidays(holidays);
	}
}
