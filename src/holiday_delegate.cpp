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
#include "graphic_widget.hpp"
#include "365x1.xpm"

//const char *HolidayDelegate::template_name_file_background = "./BackGround_%d.png";		


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
	//QString tmp_holidays="";
	if (_back_ground) {
		/*std::stringstream ss;
		ss << boost::format(std::string(template_name_file_background))%year_days;
		//std::string name_file_background = ss.str();
		//QPixmap pixmap("/home/slava/Projects/cmake_qt/src/BackGround_366.png");
		QPixmap pixmap(ss.str().c_str());*/
		QPixmap *pixmap;
		int year_days = 365 + static_cast<int>((_year % 4 == 0 && _year % 100 != 0)||(_year % 400 == 0));
		if (year_days == 365){
			pixmap = new QPixmap(backGround_365_1);
		}
		else {
			pixmap = new QPixmap(backGround_366_1);
		}
		QRect source(0, 0, option.rect.width(), option.rect.height());
		painter->drawPixmap(option.rect, pixmap->scaled(option.rect.width(), option.rect.height()), source);
		delete pixmap;
	}
	QList<QVariant> list_period_fails = model->data(index, Qt::DecorationRole).toList();
	for (auto &elem :list_period_fails){
		 /*std::cout << "begin_day = " << elem.toMap()["begin_day"].toInt() << std::endl;
		 std::cout << "number_days = " << elem.toMap()["number_days"].toInt() << std::endl;*/
		 painter->setPen(Qt::darkRed);
		 painter->setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
		 painter->drawRect(option.rect.x()+(elem.toMap()["begin_day"].toInt() - 1)*_scale, option.rect.y()+2, (elem.toMap()["number_days"].toInt())*_scale, 24);
	}
	QList<QVariant> list_from_model = model->data(index, Qt::EditRole).toList();
	for (auto &elem : list_from_model){
		using namespace boost::gregorian;
		//tmp_holidays+=QString::number(elem.toMap()["begin"].toInt())+" "+QString::number(elem.toMap()["duration"].toInt())+ " "+QString::number(elem.toMap()["travel"].toInt())+":";
		//tmp_holidays = elem.second.toList();
		//painter->setBrush(Qt::LinearGradientPattern);
		/* Пример того, как отображать конфликты: ----------------------------------------*
		 * painter->setPen(Qt::darkRed);
		 * painter->setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
		 * painter->drawRect(option.rect.x()+(elem.toMap()["begin"].toInt() - 1)*_scale, option.rect.y()+2, (elem.toMap()["duration"].toInt() - 1)*_scale, 24);
		 * ------------------------------------------------------------------------------*/
		painter->setPen(Qt::darkGreen);
		painter->setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
		painter->drawRect(option.rect.x()+(elem.toMap()["begin"].toInt() - 1)*_scale, option.rect.y()+4, (elem.toMap()["duration"].toInt())*_scale, 20);
		if (elem.toMap()["travel"].toInt() > 0){
			/* тоже отображается при конфликтах -----------------------------------------*
			 * painter->setPen(Qt::darkRed);
			 * painter->setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
			 * painter->drawRect(option.rect.x()+(elem.toMap()["begin"].toInt() - 1)*_scale + (elem.toMap()["duration"].toInt() -1)*_scale, option.rect.y()+2, (elem.toMap()["travel"].toInt() - 1)*_scale, 24);
			 * -------------------------------------------------------------------------*/
			painter->setPen(Qt::blue);
			painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
			painter->drawRect(option.rect.x()+(elem.toMap()["begin"].toInt() - 1)*_scale+(elem.toMap()["duration"].toInt())*_scale, option.rect.y()+4, (elem.toMap()["travel"].toInt())*_scale, 20);
		}
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
			std::cout << "To Editor begin=" << elem.toMap()["begin"].toInt() - 1 << "; duration=" << elem.toMap()["duration"].toInt() << std::endl;
			QRect rect((elem.toMap()["begin"].toInt()-1)*_scale, 4, (elem.toMap()["duration"].toInt())*_scale/*+elem.toMap()["travel"].toInt()*_scale - 1*/, 20);
			holidays.push_back(rect);
		}
		GraphicsWidget *graphicEditor = static_cast<GraphicsWidget *>(editor);
		graphicEditor->setHolidays(holidays);
	}
}

void HolidayDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
									const QModelIndex &index) const
{
	GraphicsWidget *graphicEditor = static_cast<GraphicsWidget *>(editor);
	std::vector<QRect> vec = graphicEditor->getHolidays();
	QList<QVariant> listOfRectHolidays;
	for(auto elem : vec){
		QRect rec_without_scale(elem);
		rec_without_scale.setLeft(elem.left()/_scale);
		rec_without_scale.setRight((elem.right()+2)/_scale);
		std::cout << "To Model scale = " << _scale << "; begin=" << elem.left()/_scale << "; duration=" << (elem.right() - elem.left() + 2)/_scale << std::endl;
		listOfRectHolidays.append(rec_without_scale);//.left()/_scale - 1);
	}
	model->setData(index, QVariant(listOfRectHolidays), Qt::EditRole);
}

