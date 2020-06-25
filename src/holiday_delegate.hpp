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
	explicit HolidayDelegate(const int year_days, const int scale = 2, const bool back_ground = true, QObject *parent = nullptr)
					:QStyledItemDelegate(parent), _year_days(year_days),
					_scale(scale), _back_ground(back_ground) {}
	void paint(QPainter *painter, 
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
private:
	const static char *template_name_file_background; 
	const int _year_days;
	const int _scale;
	const bool _back_ground;
};

#endif //HOLIDAYDELEGATE_HPP

