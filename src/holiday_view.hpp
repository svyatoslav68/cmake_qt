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
	explicit HolidayView(QWidget *parent = nullptr);
private:
};

#endif //MYTABLEVIEW_HPP
