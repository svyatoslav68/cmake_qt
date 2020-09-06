/******************************************************
 *     Заголовочный файл для создания класса        ***
 *     табличного отображения. 						***
 *     Файл holiday_view.hpp							***
 *****************************************************/
#ifndef MYTABLEVIEW_HPP
#define MYTABLEVIEW_HPP

#include <QTableView>

class HolidayView : public QTableView{
public:
	explicit HolidayView(const int year, QWidget *parent = nullptr, const int scale = 2);
	void setSizeColumns();
private:
	const int _year;
	const int _scale;
};

#endif //MYTABLEVIEW_HPP
