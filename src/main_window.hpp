/*******************************************************************************
******** Заголовочный файл главного окна приложения ****************************
********             main_window.hpp                ****************************
*******************************************************************************/
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
/*class QMainWindow*/
class QAction;
class QActionGroup;
class QMenu;
class QWidget;
class QTableView;
//class QGraphicsView;
class QGraphicsScene;
class HolidayTableModel;

class MainWindow: public QMainWindow {
Q_OBJECT
public:
	MainWindow();
private:
	void createActions();
	void createMenu();
	QAction *actExit;
	QActionGroup *grpView;
	QAction *actPlan, *actResult, *actRefresh;
	QAction *actUnit, *actSetColors;
	QAction *actHelpApplication, *actAboutQt;
	QMenu *menuFile, *menuView, *menuSettings, *menuHelp;
	//QWidget *mainWidget;
	QGraphicsScene *mainScene;
	HolidayTableModel *holidayModel;
	QTableView *mainView;
	//QGraphicsView *mainView;
private slots:
	void clickedOk();
	void viewHelp();
	void aboutQt();
};

#endif // MAIN_WINDOW_HPP

