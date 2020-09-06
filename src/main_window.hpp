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
class HolidayView;
class HolidayDelegate;

class MainWindow: public QMainWindow {
Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
private:
	void createActions();
	void createMenu();
	void createModelAndView();
	QAction *actExit;
	QActionGroup *grpView;
	QAction *actPlan, *actResult, *actRefresh;
	QAction *actConditions, *actUnit, *actSetColors;
	QActionGroup *grpScale;
	QAction *act1x, *act2x, *act3x;
	QAction *actBackGround;
	QAction *actHelpApplication, *actAboutQt;
	QMenu *menuFile, *menuView, *menuSettings, *menuHelp;
	QMenu *menuScale;
	//QWidget *mainWidget;
	HolidayTableModel *holidayModel;
	HolidayView *mainView;
	HolidayDelegate *graphicDelegate;//QGraphicsView *mainView;
	int scale;
private slots:
	void changeScale(int new_scale);
	void dialogConditions();
	void selectUnitSlot();
	void refreshSlot();
	void clickedOk();
	void viewHelp();
	void aboutQt();
};

#endif // MAIN_WINDOW_HPP

