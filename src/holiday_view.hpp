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
private:
	const int _scale;
	std::vector<QRect> _rectsHoliday;
	int _currentHoliday;
protected:
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
};

class HolidayView : public QTableView{
public:
	explicit HolidayView(QWidget *parent = nullptr, const int days_in_year = 365, const int scale = 2);
	void setSizeColumns();
private:
	const int _year_days;
	const int _scale;
};

#endif //MYTABLEVIEW_HPP
