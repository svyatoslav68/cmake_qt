/*******************************************************************************
***** Файл реализации главного окна приложения *********************************
*****       main_window.cpp                    *********************************
*******************************************************************************/
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QMessageBox>
#include <Qt>
#include <QTableView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <vector>
#include <string>
#include "main_window.hpp"
#include "tholiday.hpp"
#include "holiday_table_model.hpp"

MainWindow::MainWindow(){
	//setAttribute(Qt::WA_DeleteOnClose); // Атрибут показывает, что после закрытия окна, его нужно удалить из памяти
	setObjectName("MainWindow");
	setWindowTitle("График отпусков");
	mainScene  =new QGraphicsScene();
	mainScene->addEllipse(QRectF(-100.0, -100.0, 75.0, 75.0));
	//mainView->setScene(mainScene);
	std::vector<std::string> v = {"Иванов", "Петров", "Сидоров"};
	holidayModel = new HolidayTableModel(v);
	mainView = new QTableView();//QGraphicsView();
	mainView->setModel(holidayModel);
	setCentralWidget(mainView);
	createActions();
	createMenu();
}

void MainWindow::createActions(){
	actExit = new QAction("Закрыть программу", this);
	actExit->setObjectName("actExit");
	actExit->setShortcut(QKeySequence::Quit);
	connect(actExit, SIGNAL(triggered()), this, SLOT(clickedOk()));
	grpView = new QActionGroup(this);
	actPlan = new QAction("Заявки на отпуск");
	actPlan->setCheckable(true);
	actResult = new QAction("План отпусков");
	actResult->setCheckable(true);
	grpView->addAction(actPlan);
	grpView->addAction(actResult);
	actResult->setChecked(true);
	actRefresh = new QAction("Обновить");
	actUnit = new QAction("Подразделение");
	actSetColors = new QAction("Цвета");
	actHelpApplication = new QAction("Помощь", this);
	actHelpApplication->setObjectName("actHelpApplication");
	actHelpApplication->setShortcut(Qt::Key_F1);
	connect(actHelpApplication, SIGNAL(triggered()), this, SLOT(viewHelp()));
	actAboutQt = new QAction("О Qt", this);
	actAboutQt->setObjectName("actAboutQt");
	connect(actAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MainWindow::createMenu(){
	menuFile = menuBar()->addMenu("&File");
	menuFile->addAction(actExit);
	menuView = menuBar()->addMenu("Вид");
	menuView->addAction(actPlan);
	menuView->addAction(actResult);
	menuView->addSeparator();
	menuView->addAction(actRefresh);
	menuSettings = menuBar()->addMenu("Настройки");
	menuSettings->addAction(actUnit);
	menuSettings->addAction(actSetColors);
	//menuSettings->addAction(act);
	menuHelp = menuBar()->addMenu("&Help");
	menuHelp->addAction(actHelpApplication);
	menuHelp->addSeparator();
	menuHelp->addAction(actAboutQt);
}

void MainWindow::clickedOk(){
	close();
}

void MainWindow::aboutQt(){
	QMessageBox::aboutQt(this, tr("О библиотеке Qt"));
}

void MainWindow::viewHelp(){
}
