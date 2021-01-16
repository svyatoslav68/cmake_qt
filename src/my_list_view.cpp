#include <iostream>
#include <QItemSelection>
#include "my_list_view.hpp"

void MyListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	//static int number_emit = 0;
	//std::cout << "Execute selectionChanged(), number execute = "<< number_emit++ << std::endl;
	emit changeCurrentIndex();
}


