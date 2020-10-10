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
#include <QSignalMapper>
#include <vector>
#include <string>
#include "main_window.hpp"
#include "tholiday.hpp"
#include "holiday_table_model.hpp"
#include "holiday_delegate.hpp"
#include "holiday_view.hpp"
#include "dialog_conditions.hpp"
#include "dialog_units.hpp"

extern clParametrs appParametrs;

MainWindow::MainWindow():holidayModel(nullptr), mainView(nullptr), graphicDelegate(nullptr), scale(2)
{
	//setAttribute(Qt::WA_DeleteOnClose); // Атрибут показывает, что после закрытия окна, его нужно удалить из памяти
	setObjectName("MainWindow");
	setWindowTitle("График отпусков");
	createActions();
	createMenu();
	createModelAndView();
}

MainWindow::~MainWindow()
{
	delete mainView;
	delete holidayModel;
	delete graphicDelegate;
}

void MainWindow::createModelAndView(){
	if (holidayModel)
		delete holidayModel;
	holidayModel = new HolidayTableModel();
	//std::vector<int> rows {};
	holidayModel->fillConflicts();
	//connect(holidayModel, SIGNAL(dataChanged(QModelIndex(), QModelIndex())), );
	int year = appParametrs.getYear();
	if (mainView)
		delete mainView;
	mainView = new HolidayView(year, this, scale);//QGraphicsView();
	mainView->setModel(holidayModel);
	mainView->setSizeColumns();
	if (graphicDelegate)
		delete graphicDelegate;
	graphicDelegate = new HolidayDelegate(year, scale, actBackGround->isChecked());
	mainView->setItemDelegateForColumn(2, graphicDelegate);
	setCentralWidget(mainView);
	int tableWidth = 70, tableHeight = 50;
	for (int i = 0; i < mainView->model()->columnCount(); ++i){
		tableWidth += mainView->columnWidth(i);
	}
	for (int i = 0; i < mainView->model()->rowCount(); ++i){
		tableHeight += mainView->rowHeight(i);
	}
	setGeometry(10, 30, tableWidth, 600);
}

void MainWindow::createActions()
{
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
	grpScale = new QActionGroup(this);
	//QSignalMapper *scaleMapper = new QSignalMapper();
	act1x = new QAction("x1");
	act1x->setCheckable("true");
	connect(act1x, &QAction::triggered, this, [this](){this->changeScale(1);});
	act2x = new QAction("x2");
	act2x->setCheckable("true");
	act2x->setChecked("true");
	connect(act2x, &QAction::triggered, this, [this](){this->changeScale(2);});
	act3x = new QAction("x3");
	act3x->setCheckable("true");
	connect(act3x, &QAction::triggered, this, [this](){this->changeScale(3);});
	grpScale->addAction(act1x);
	grpScale->addAction(act2x);
	grpScale->addAction(act3x);
	actRefresh = new QAction("Обновить");
	connect(actRefresh, SIGNAL(triggered()), this, SLOT(refreshSlot()));
	actBackGround = new QAction("Показать фон");
	actBackGround->setCheckable("true");
	actBackGround->setChecked("true");
	connect(actBackGround, SIGNAL(triggered()), this, SLOT(refreshSlot()));
	actConditions = new QAction("Условия");
	actConditions->setShortcut(QKeySequence("CTRL-U"));
	connect(actConditions, SIGNAL(triggered()), this, SLOT(dialogConditions()));
	actUnit = new QAction("Подразделение");
	connect(actUnit, SIGNAL(triggered()), this, SLOT(selectUnitSlot()));
	actSetColors = new QAction("Цвета");
	actHelpApplication = new QAction("Помощь", this);
	actHelpApplication->setObjectName("actHelpApplication");
	actHelpApplication->setShortcut(Qt::Key_F1);
	connect(actHelpApplication, SIGNAL(triggered()), this, SLOT(viewHelp()));
	actAboutQt = new QAction("О Qt", this);
	actAboutQt->setObjectName("actAboutQt");
	connect(actAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MainWindow::createMenu()
{
	menuFile = menuBar()->addMenu("&File");
	menuFile->addAction(actExit);
	menuView = menuBar()->addMenu("Вид");
	menuView->addAction(actPlan);
	menuView->addAction(actResult);
	menuView->addAction(actBackGround);
	menuView->addSeparator();
	menuScale = menuView->addMenu("Масштаб");
	menuScale->addAction(act1x);
	menuScale->addAction(act2x);
	menuScale->addAction(act3x);
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

void MainWindow::clickedOk()
{
	close();
}

void MainWindow::aboutQt()
{
	QMessageBox::aboutQt(this, tr("О библиотеке Qt"));
}

void MainWindow::viewHelp()
{
}

void MainWindow::dialogConditions()
{
	DialogConditions *dialog = new DialogConditions();
	if (dialog->exec() == QDialog::Accepted)
	{
		dialog->saveConfiguration();
	}
	delete dialog;
}

void MainWindow::selectUnitSlot()
{
	bool requireOverDraw = false;
	TDialogUnits dialog_units(appParametrs.getIdUnit(), appParametrs.getYear());
	if (dialog_units.exec() == QDialog::Accepted){
		int year = dialog_units.getYear();
		if (year > -1){
			requireOverDraw |= year != appParametrs.getYear();
			appParametrs.setYear(year);
		}
		int unit = dialog_units.getUnit();
		if (unit > 0 ) {
			requireOverDraw |= unit != appParametrs.getIdUnit();
			appParametrs.setIdUnit(unit);
		}
	}
	if (requireOverDraw) {
		std::cout << "OverDraw MainWindow\n";
		createModelAndView();
	}
	if (dialog_units.getSave())
		appParametrs.saveSettings();
}

void MainWindow::refreshSlot()
{
	createModelAndView();
}

void MainWindow::changeScale(int new_scale)
{
	scale = new_scale;
	createModelAndView();
}

