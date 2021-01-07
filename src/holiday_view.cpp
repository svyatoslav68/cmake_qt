/**********************************************************
 ****   Файл реализации класса табличного отображения *****
 ****        файл holiday_view .cpp                     *****
 *********************************************************/
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <QHeaderView>
#include <QPainter>
#include <QString>
#include <QPen>
#include <QPointF>
#include "holiday_table_model.hpp"
#include "holiday_view.hpp"

HolidayHeader::HolidayHeader(QWidget *parent):QHeaderView(Qt::Horizontal, parent) 
{
	using namespace boost::gregorian;
	HolidayView *viewer = dynamic_cast<HolidayView *>(parent);	
	for (int i = 0; i < 12; ++i){
		std::stringstream ss;
		ss << boost::format("%d/%d/%d")%(viewer->getYear())%(i+1)%(1) << std::flush;
		std::string dateStr = ss.str();
		date d(from_string(dateStr));
		daysInMonth[i] = static_cast<int>(d.end_of_month().day()) * viewer->getScale();
		//std::cout << "From HolidayHeader(). Date = " << d <<  "; last day = " << daysInMonth[i] << std::endl;
	}
}

void HolidayHeader::paintEvent(QPaintEvent *)
{
	//std::cout << "Executing paintEvent()" << std::endl;
	QPainter painter(this);
	painter.begin(viewport());
	QRect columnRect(0,0, HolidayView::WIDTH_NUMBER_COL, height());
	painter.setPen(palette().buttonText().color());
	painter.drawText(columnRect, QString("№ п/п"), QTextOption(Qt::AlignCenter));
	columnRect = QRect(HolidayView::WIDTH_NUMBER_COL, 0, HolidayView::WIDTH_FIO_COL, height());
	painter.drawText(columnRect, QString("Ф И.О."), QTextOption(Qt::AlignCenter));
	painter.setPen(QPen(palette().mid().color(), 2));
	painter.drawLine(QPointF(HolidayView::WIDTH_NUMBER_COL - 1, 0), QPointF(HolidayView::WIDTH_NUMBER_COL -1, height()));
	painter.drawLine(QPointF(HolidayView::WIDTH_NUMBER_COL+HolidayView::WIDTH_FIO_COL - 1, 0), QPointF(HolidayView::WIDTH_NUMBER_COL+HolidayView::WIDTH_FIO_COL -1, height()));
	for (int i = 0, leftX = HolidayView::WIDTH_NUMBER_COL+HolidayView::WIDTH_FIO_COL; i < 12; ++i){
		columnRect = QRect(leftX, 0, daysInMonth[i], height());
		painter.setPen(palette().buttonText().color());
		painter.drawText(columnRect, namesMonth[i].c_str(), QTextOption(Qt::AlignCenter));
		//std::cout << "From paintEvent(): leftX = " << leftX << std::endl;
		leftX+=daysInMonth[i];
		painter.setPen(QPen(palette().mid().color(), 2));
		painter.drawLine(QPointF(leftX - 1, 0), QPointF(leftX-1, height()));
		}
	//painter.setPen(QPen(palette().button().color().darker(), 0.5));
	//painter.drawRect(0, 0, width(), height());
	painter.end();
}

HolidayView::HolidayView(const int year, QWidget *parent, const int scale):QTableView(parent), _year(year), _scale(scale)
{
	//horizontalHeader()->hide();
	horHeader = new HolidayHeader(this);
	setHorizontalHeader(horHeader);
	//horHeader->update();
}

void HolidayView::setSizeColumns(){
	setColumnWidth(HolidayTableModel::Number, WIDTH_NUMBER_COL);
	setColumnWidth(HolidayTableModel::FIO, WIDTH_FIO_COL);
	int year_days = 365 + static_cast<int>((_year % 4 == 0 && _year % 100 != 0)||(_year % 400 == 0));
	setColumnWidth(HolidayTableModel::Holidays, _scale*year_days);
}

