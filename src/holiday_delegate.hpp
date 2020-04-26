/******************************************************
*    Заголовочный файл делегата, реализующего *********
*    отображение планируемых отпусков в виде  *********
*    полосок, длина которых пропорциональна   *********
*    количеству дней в каждом из отпусков     *********
*    файл - holiday_delegate.hpp               *********
******************************************************/
#ifndef HOLIDAYDELEGATE_HPP
#define HOLIDAYDELEGATE_HPP

#include <QStyledItemDelegate>

class QStyledOptionViewItem;
class QModelIndex;
class QPainter;
class QObject;

class HolidayDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	explicit HolidayDelegate(QObject *parent = nullptr)
					:QStyledItemDelegate(parent) {}
	void paint(QPainter *painter, 
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
private:
};

#endif //HOLIDAYDELEGATE_HPP

