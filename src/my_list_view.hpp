/***************************************************
	Заголовок класса отображения списка отпусков
			   файл my_list_view.hpp
***************************************************/
#ifndef MY_LIST_VIEW_HPP 
#define MY_LIST_VIEW_HPP

#include <QListView>

class QItemSelection;

class MyListView : public QListView {
Q_OBJECT
public:
	MyListView(QWidget *parent = nullptr):QListView(parent) {}
protected:
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
signals:
	void changeCurrentIndex();
};

#endif // MY_LIST_VIEW_HPP
