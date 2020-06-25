/*************************************************
**  Реализация класса DialogConditions          **
**  Файл: dialog_conditions.cpp                 **
*************************************************/

#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QTableView>
#include <QStringList>
#include <QVariant>
#include <QPushButton>
#include <QBoxLayout>
#include <QModelIndex>
#include <QStringListModel>
#include <QLineEdit>
#include <QStringList>
#include <QtDebug>
#include <QHeaderView>
#include "cl_parametrs.hpp"
#include "person_model.hpp"
#include "modelMembersGroup.hpp"
#include "dialog_conditions.hpp"
#include "str_from_file.hpp"

extern clParametrs appParametrs;

DialogConditions::DialogConditions(QWidget *parent):QDialog(parent){
	ValuesFromXML vx = ValuesFromXML(appParametrs.getNameConfFile().c_str());
	setWindowTitle("Условия для создания графика отпусков");
	personModel = new PersonModel(this);
	groupsModel = new ModelGroups(this);
	groupsModel->loadFromXML();
	membersModel = nullptr;
	createWidgets();
}

DialogConditions::~DialogConditions(){
	delete buAddGroup;
	delete buDeleteGroup;
	delete editGroup;
	delete buAppend;
	delete buDelete;
	delete buAddCondition;
	delete buDeleteCondition;
	delete viewTablePersons;
	delete viewListContentGroup;
	delete laContentGroup;
	delete viewListGroups;
	delete buOk;
	delete buSave;
	delete personModel;
	delete membersModel;
	delete groupsModel;
}

void DialogConditions::createWidgets(){
	buOk = new QPushButton("Ok");
	connect(buOk, SIGNAL(clicked()), this, SLOT(accept()));
	buSave = new QPushButton("Save");
	connect(buSave, SIGNAL(clicked()), this, SLOT(saveConfiguration()));
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(buSave);
	bottomLayout->addStretch();
	bottomLayout->addWidget(buOk);
	viewTablePersons = new QTableView();
	viewTablePersons->setModel(personModel);
	QHeaderView *header = viewTablePersons->horizontalHeader();
	header->resizeSection(0, 50);
	header->resizeSection(1,220);
	header->setStretchLastSection(true);
	laContentGroup = new QLabel("Члены группы:");
	viewListContentGroup = new QListView();
	viewListContentGroup->setObjectName("viewListContentGroup");
	QLabel *laGroups = new QLabel("Список групп");
	viewListGroups = new QListView();
	viewListGroups->setModel(groupsModel);
	connect(viewListGroups, SIGNAL(clicked(const QModelIndex &)), this, SLOT(changeIndexGroup(const QModelIndex &)));
	buAddGroup = new QPushButton("+");
	buAddGroup->setFlat(true);
	buAddGroup->setMaximumHeight(40);
	buAddGroup->setMaximumWidth(40);
	connect(buAddGroup, SIGNAL(clicked()), this, SLOT(addGroup()));
	buDeleteGroup = new QPushButton("-");
	buDeleteGroup->setFlat(true);
	buDeleteGroup->setMaximumHeight(40);
	buDeleteGroup->setMaximumWidth(40);
	connect(buDeleteGroup, SIGNAL(clicked()), this, SLOT(deleteGroup()));
	editGroup = new QLineEdit();
	QHBoxLayout *editGroupLayout = new QHBoxLayout;
	editGroupLayout->setSpacing(0);
	editGroupLayout->addWidget(buAddGroup, 0);
	editGroupLayout->addWidget(buDeleteGroup, 0);
	editGroupLayout->addWidget(editGroup);
	QVBoxLayout *groupLayout = new QVBoxLayout;
	groupLayout->setMargin(0);
	groupLayout->setSpacing(0);
	groupLayout->addWidget(laGroups);
	groupLayout->addWidget(viewListGroups);
	groupLayout->addLayout(editGroupLayout);
	buAppend = new QPushButton("Добавить члена");
	connect(buAppend, SIGNAL(clicked()), this, SLOT(addMemberSlot()));
	buDelete = new QPushButton("Удалить члена");
	connect(buDelete, SIGNAL(clicked()), this, SLOT(deleteMemberSlot()));
	buAddCondition = new QPushButton("Добавить условие");
	buDeleteCondition = new QPushButton("Убрать условие");
	QVBoxLayout *contentLayout = new QVBoxLayout();
	contentLayout->addWidget(laContentGroup, 0);
	contentLayout->addWidget(viewListContentGroup, 1);
	QVBoxLayout *controlLayout = new QVBoxLayout;
	controlLayout->addStretch();
	controlLayout->addWidget(buAppend);
	controlLayout->addWidget(buDelete);
	controlLayout->addWidget(buAddCondition);
	controlLayout->addWidget(buDeleteCondition);
	controlLayout->addStretch();
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(groupLayout);
	topLayout->addLayout(controlLayout);
	topLayout->addLayout(contentLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(viewTablePersons);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void DialogConditions::loadConfiguration(){
}

void DialogConditions::saveConfiguration(){
	ValuesFromXML vx = ValuesFromXML(appParametrs.getNameConfFile().c_str());
	/* Создаем вектор пар. Первое значение - имя группы, второе -
	вектор численных идентификаторов членов группы */
	std::vector<std::pair<std::string, std::vector<int>>> vector_groups;
	for (auto group: groupsModel->getMembers()){
		//std::cout << str.toStdString() << std::endl;
		std::vector<int> vector_members;
		//vectorString.push_back(group.name.toStdString());
		for (auto member: *group.children){
			vector_members.push_back(member.idBD);
		}
		vector_groups.push_back(std::pair(group.name, vector_members));
	}
	vx.saveGroups("FILE.Groups", vector_groups);
}

void DialogConditions::addGroup(){
	groupsModel->insertRows(groupsModel->rowCount(), 1);
	groupsModel->setData(groupsModel->index(groupsModel->rowCount()-1), editGroup->text());
	editGroup->clear();
}

void DialogConditions::deleteGroup(){
	groupsModel->removeRows(viewListGroups->currentIndex().row(), 1);
}

void DialogConditions::changeIndexGroup(const QModelIndex &index){
	//std::cout <<"Current group = " << index.model()->data(index).toString().toStdString() << std::endl;
	laContentGroup->setText(QString("Члены группы %1:").arg(index.model()->data(index).toString()));
	if (membersModel)
		delete membersModel;
	membersModel = new ModelMembersGroup(this, static_cast<ItemOfGroup *>(index.internalPointer())->children);
	viewListContentGroup->setModel(membersModel);
	//viewListContentGroup->setModel();
}

void DialogConditions::addMemberSlot(){
	if (membersModel){
		membersModel->insertRows(membersModel->rowCount(), 1);
		QStringList strlist = {personModel->data(personModel->index(viewTablePersons->currentIndex().row(), PersonModel::ID)).toString(), personModel->data(personModel->index(viewTablePersons->currentIndex().row(), PersonModel::FIO)).toString()};
		membersModel->setData(membersModel->index(membersModel->rowCount()-1), QVariant(strlist));
	}
}

void DialogConditions::deleteMemberSlot(){
	if (membersModel){
		membersModel->removeRows(viewListContentGroup->currentIndex().row(), 1);
	}
}


