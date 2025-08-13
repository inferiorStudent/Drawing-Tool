#include "ktopmenu.h"
#include <QHBoxLayout>
#include <QAction>
#include <QActionGroup>

KTopMenu::KTopMenu(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground, true);// �Զ���ؼ���ʽ����Ч
	setStyleSheet("background-color:#2C2F33");// ������ʽ��

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	fileButton = new QPushButton(QStringLiteral("�ļ�"), this);
	fileButton->setStyleSheet(
		"QPushButton {"
		"   background-color: transparent;"
		"   color: white;"
		"   border: none;"
		"   padding: 5px 10px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #333;"
		"}"
	);
	fileButton->setFlat(true); // ��������˵�

	// �����˵�
	fileMenu = new QMenu(this);
	fileMenu->setStyleSheet(
		"QMenu {"
		"   background-color: #23272A;"
		"   color: white;"
		"   border: 1px solid #333;"
		"}"
		"QMenu::item:selected {"
		"   background-color: #555;"
		"}"
	);

	QAction* saveAction = new QAction(QStringLiteral("���Ϊ"), this);
	QAction* importAction = new QAction(QStringLiteral("����"), this);
	QAction* addCanvasAction = new QAction(QStringLiteral("�½�����"), this);

	fileMenu->addAction(saveAction);
	fileMenu->addAction(importAction);
	fileMenu->addAction(addCanvasAction);

	connect(saveAction, &QAction::triggered, this, &KTopMenu::exportTriggered);
	connect(importAction, &QAction::triggered, this, &KTopMenu::importTriggered);
	connect(addCanvasAction, &QAction::triggered, this, &KTopMenu::newCanvasTriggered);

	connect(fileButton, &QPushButton::clicked, this, &KTopMenu::showFileMenu);

	layout->addWidget(fileButton);

	// --- �鿴�˵� ---
	viewButton = new QPushButton(QStringLiteral("�鿴"), this);
	viewButton->setStyleSheet(fileButton->styleSheet()); // �����ļ���ť��ʽ
	viewButton->setFlat(true);
	
	viewMenu = new QMenu(this);
	viewMenu->setStyleSheet(fileMenu->styleSheet()); // �����ļ��˵���ʽ
	
	QActionGroup * zoomGroup = new QActionGroup(this);
	zoomGroup->setExclusive(true);
	
	// ������ͬ���ű����� Action
	QAction * zoom50 = new QAction("50%", this);
	zoom50->setData(50);
	zoom50->setCheckable(true);
	QAction * zoom100 = new QAction("100%", this);
	zoom100->setData(100);
	zoom100->setCheckable(true);
	zoom100->setChecked(true); // Ĭ�� 100%
	QAction * zoom150 = new QAction("150%", this);
	zoom150->setData(150);
	zoom150->setCheckable(true);
	QAction * zoom200 = new QAction("200%", this);
	zoom200->setData(200);
	zoom200->setCheckable(true);
	
	zoomGroup->addAction(zoom50);
	zoomGroup->addAction(zoom100);
	zoomGroup->addAction(zoom150);
	zoomGroup->addAction(zoom200);
	
	viewMenu->addActions(zoomGroup->actions());
	
	connect(zoomGroup, &QActionGroup::triggered, this, [this](QAction* action) {
		emit zoomChanged(action->data().toInt());
		});
	connect(viewButton, &QPushButton::clicked, this, &KTopMenu::showViewMenu);
	
	layout->addWidget(viewButton);

	layout->addStretch();
}

void KTopMenu::showFileMenu()
{
	// ���˵��ŵ� button ������
	fileMenu->popup(fileButton->mapToGlobal(QPoint(0, fileButton->height())));
}

void KTopMenu::showViewMenu()
{
	viewMenu->popup(viewButton->mapToGlobal(QPoint(0, viewButton->height())));
}