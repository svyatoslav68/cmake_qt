/**********************************************************
 ****   Заголовок класса элемента редактирования      *****
 ****        отпусков. файл graphic_widget.cpp        *****
 *********************************************************/
#ifndef GRAPHIC_WIDGET_HPP
#define GRAPHIC_WIDGET_HPP

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
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
};

#endif //GRAPHIC_WIDGET_HPP
