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
#include <QComboBox>
#include <QSpinBox>
#include <QStringList>
#include <QtDebug>
#include <tuple>
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
	conditionsModel = nullptr;
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
	delete viewConditions;
	delete spCondition;
	delete cbCondition;
	delete viewTablePersons;
	delete viewListContentGroup;
	delete laContentGroup;
	delete viewListGroups;
	delete buOk;
	delete buSave;
	delete personModel;
	delete membersModel;
	delete conditionsModel;
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
	connect(viewListContentGroup, SIGNAL(clicked(const QModelIndex &)), this, SLOT(changeIndexMember(const QModelIndex &)));
	QLabel *laCondition = new QLabel("Условия:"); 
	viewConditions = new QListView();
	viewConditions->setObjectName("viewConditions");
	QStringList lstConditions;
	lstConditions << "<" << "=" << ">";
	cbCondition = new QComboBox();
	cbCondition->setObjectName("cbCondition");
	cbCondition->addItems(lstConditions);
	spCondition = new QSpinBox();
	spCondition->setMinimum(0);
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
	buAppend->setEnabled(false);
	connect(buAppend, SIGNAL(clicked()), this, SLOT(addMemberSlot()));
	buDelete = new QPushButton("Удалить члена");
	buDelete->setEnabled(false);
	connect(buDelete, SIGNAL(clicked()), this, SLOT(deleteMemberSlot()));
	buAddCondition = new QPushButton("+");
	buAddCondition->setFlat(true);
	buAddCondition->setMaximumHeight(40);
	buAddCondition->setMaximumWidth(40);
	buAddCondition->setEnabled(false);
	connect(buAddCondition, &QPushButton::clicked, this, [this](){this->addCondition();});
	buDeleteCondition = new QPushButton("-");
	buDeleteCondition->setFlat(true);
	buDeleteCondition->setMaximumHeight(40);
	buDeleteCondition->setMaximumWidth(40);
	buDeleteCondition->setEnabled(false);
	connect(buDeleteCondition, &QPushButton::clicked, this, [this](){this->deleteCondition();});
	QHBoxLayout *editConditionLayout = new QHBoxLayout;
	editConditionLayout->setSpacing(0);
	editConditionLayout->addWidget(cbCondition);
	editConditionLayout->addWidget(spCondition);
	editConditionLayout->addStretch();
	editConditionLayout->addWidget(buAddCondition);
	editConditionLayout->addWidget(buDeleteCondition);
	QVBoxLayout *contentLayout = new QVBoxLayout();
	contentLayout->addWidget(laContentGroup, 0);
	contentLayout->addWidget(viewListContentGroup, 2);
	//QHBoxLayout *conditionLayout = new QHBoxLayout;
	contentLayout->addWidget(laCondition, 0);
	contentLayout->addWidget(viewConditions, 1);
	//contentLayout->addLayout(conditionLayout);
	contentLayout->addLayout(editConditionLayout);
	QVBoxLayout *controlLayout = new QVBoxLayout;
	controlLayout->addStretch();
	controlLayout->addWidget(buAppend);
	controlLayout->addWidget(buDelete);
	controlLayout->addStretch();
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(groupLayout);
	topLayout->addLayout(controlLayout);
	topLayout->addLayout(contentLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(topLayout, 1);
	mainLayout->addWidget(viewTablePersons, 3);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void DialogConditions::loadConfiguration(){
}

void DialogConditions::saveConfiguration(){
	ValuesFromXML vx = ValuesFromXML(appParametrs.getNameConfFile().c_str());
	/* Создаем вектор пар. Первое значение - имя группы, второе -
	вектор численных идентификаторов членов группы */
	std::vector<std::tuple<std::string, std::vector<int>, std::vector<std::pair<std::string, int>>>> vector_groups;
	for (auto group: groupsModel->getMembers()){
		//std::cout << str.toStdString() << std::endl;
		std::vector<int> vector_members;
		std::vector<std::pair<std::string, int>> vector_conditions;
		//vectorString.push_back(group.name.toStdString());
		for (auto member: *group.children){
			vector_members.push_back(member.idBD);
		}
		for (auto condition: *group.conditions){
			std::string word_condition;
			switch (condition.symbol_functor){
				case '>':word_condition = "great";break;
				case '<':word_condition = "less";break;
				case '=':word_condition = "equal";break;
				default: word_condition = "";
			}
			vector_conditions.push_back(std::make_pair(word_condition, condition.number_members));
		}
		vector_groups.push_back(std::tuple(group.name, vector_members, vector_conditions));
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
	buAppend->setEnabled(viewListGroups->currentIndex().isValid());
	buDelete->setEnabled(viewListGroups->currentIndex().isValid() and viewListContentGroup->currentIndex().isValid());
	buAddCondition->setEnabled(viewListGroups->currentIndex().isValid());
	buDeleteCondition->setEnabled(viewListGroups->currentIndex().isValid());
	laContentGroup->setText(QString("Члены группы %1:").arg(index.model()->data(index).toString()));
	if (membersModel)
		delete membersModel;
	membersModel = new ModelMembersGroup(this, static_cast<ItemOfGroup *>(index.internalPointer())->children);
	viewListContentGroup->setModel(membersModel);
	if (conditionsModel)
		delete conditionsModel;
	conditionsModel = new ModelConditionsGroup(this, static_cast<ItemOfGroup *>(index.internalPointer())->conditions);
	viewConditions->setModel(conditionsModel);
}

void DialogConditions::changeIndexMember(const QModelIndex &index){
	buDelete->setEnabled(viewListGroups->currentIndex().isValid() and viewListContentGroup->currentIndex().isValid());
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

void DialogConditions::addCondition()
{
	if (conditionsModel) {
		conditionsModel->insertRows(conditionsModel->rowCount(), 1);
		QList<QVariant> values = {QVariant(cbCondition->itemText(cbCondition->currentIndex()).at(0).unicode()), QVariant(static_cast<unsigned int>(spCondition->value()))};
		conditionsModel->setData(conditionsModel->index(conditionsModel->rowCount() - 1), QVariant(values));
	}
}

void DialogConditions::deleteCondition()
{ // FIXME, не рабоатает segmentation fault !!!
	if (conditionsModel) {
		conditionsModel->removeRows(viewConditions->currentIndex().row(), 1);
	}
}


