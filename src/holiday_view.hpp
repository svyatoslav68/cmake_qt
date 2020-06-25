/******************************************************
 *     Заголовочный файл для создания класса        ***
 *     табличного отображения. 						***
 *     Файл holiday_view.hpp							***
 *****************************************************/
#ifndef MYTABLEVIEW_HPP
#define MYTABLEVIEW_HPP

#include <QTableView>
class QWidget;

class HolidayView : public QTableView{
public:
	explicit HolidayView(QWidget *parent = nullptr, const int days_in_year = 365, const int scale = 2);
	void setSizeColumns();
private:
	const int _year_days;
	const int _scale;
};

#endif //MYTABLEVIEW_HPP
