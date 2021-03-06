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
	explicit HolidayDelegate(const int year, const int scale = 2, const bool back_ground = true, QObject *parent = nullptr)
					:QStyledItemDelegate(parent), _year(year),
					_scale(scale), _back_ground(back_ground) {}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, 
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
						const QModelIndex &index) const;
private:
	const static char *template_name_file_background; 
	const int _year;
	const int _scale;
	const bool _back_ground;
};

#endif //HOLIDAYDELEGATE_HPP

