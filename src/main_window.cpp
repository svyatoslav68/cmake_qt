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
#include <QHeaderView>
#include <QGraphicsScene>
#include <vector>
#include <string>
#include "main_window.hpp"
#include "tholiday.hpp"
#include "holiday_table_model.hpp"
#include "holiday_delegate.hpp"
#include "holiday_view.hpp"
#include "dialog_conditions.hpp"

extern clParametrs appParametrs;

MainWindow::MainWindow(){
	//setAttribute(Qt::WA_DeleteOnClose); // Атрибут показывает, что после закрытия окна, его нужно удалить из памяти
	setObjectName("MainWindow");
	setWindowTitle("График отпусков");
	mainScene  =new QGraphicsScene();
	mainScene->addEllipse(QRectF(-100.0, -100.0, 75.0, 75.0));
	//mainView->setScene(mainScene);
	std::vector<std::string> v = {"Иванов", "Петров", "Сидоров"};
	//holidayModel = new HolidayTableModel(v);
	holidayModel = new HolidayTableModel();
	int year = appParametrs.getYear();
	mainView = new HolidayView(nullptr, 365 + static_cast<int>((year % 4 == 0 && year % 100 != 0)||(year % 400 == 0)));//QGraphicsView();
	mainView->setModel(holidayModel);
	mainView->setSizeColumns();
	HolidayDelegate *graphicDelegate = new HolidayDelegate();
	mainView->setItemDelegateForColumn(2, graphicDelegate);
	setCentralWidget(mainView);
	setGeometry(10, 30, 1100, 600);
	createActions();
	createMenu();
}

MainWindow::~MainWindow(){
	delete mainView;
	delete holidayModel;
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
	actConditions = new QAction("Условия");
	actConditions->setShortcut(QKeySequence("CTRL-U"));
	connect(actConditions, SIGNAL(triggered()), this, SLOT(dialogConditions()));
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
	menuSettings->addAction(actConditions);
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

void MainWindow::dialogConditions(){
	DialogConditions *dialog = new DialogConditions();
	if (dialog->exec() == QDialog::Accepted)
	{
		dialog->saveConfiguration();
	}
	delete dialog;
}
