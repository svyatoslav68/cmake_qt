/******************************************************
 *     Заголовочный файл для создания класса        ***
 *     табличного отображения. 						***
 *     Файл holiday_view.hpp							***
 *****************************************************/
#ifndef MYTABLEVIEW_HPP
#define MYTABLEVIEW_HPP

#include <QTableView>
#include <QWidget>
class QRect;

class GraphicsWidget : public QWidget {
Q_OBJECT
public:
	GraphicsWidget(int scale = 2, QWidget *parent = nullptr):QWidget(parent), _scale(scale), _currentHoliday(0){}
	void setHolidays(const std::vector<QRect> &holidays);
	std::vector<QRect> getHolidays();
private:
	const int _scale;
	std::vector<QRect> _rectsHoliday;
	int _currentHoliday;
protected:
	bool event(QEvent *e);
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
};

class HolidayView : public QTableView{
public:
	explicit HolidayView(const int year, QWidget *parent = nullptr, const int scale = 2);
	void setSizeColumns();
private:
	const int _year;
	const int _scale;
protected:
	//bool event(QEvent *e);
	void keyPressEvent(QKeyEvent *event);
};

#endif //MYTABLEVIEW_HPP
