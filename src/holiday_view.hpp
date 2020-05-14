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
	explicit HolidayView(QWidget *parent = nullptr, int days_in_year = 365);
	void setSizeColumns();
private:
	int year_days;
};

#endif //MYTABLEVIEW_HPP
