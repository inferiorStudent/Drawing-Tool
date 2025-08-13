#include "ktopmenu.h"
#include <QHBoxLayout>
#include <QAction>
#include <QActionGroup>

KTopMenu::KTopMenu(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground, true);// 自定义控件样式表生效
	setStyleSheet("background-color:#2C2F33");// 设置样式表

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	fileButton = new QPushButton(QStringLiteral("文件"), this);
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
	fileButton->setFlat(true); // 看起来像菜单

	// 下拉菜单
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

	QAction* saveAction = new QAction(QStringLiteral("另存为"), this);
	QAction* importAction = new QAction(QStringLiteral("导入"), this);
	QAction* addCanvasAction = new QAction(QStringLiteral("新建画布"), this);

	fileMenu->addAction(saveAction);
	fileMenu->addAction(importAction);
	fileMenu->addAction(addCanvasAction);

	connect(saveAction, &QAction::triggered, this, &KTopMenu::exportTriggered);
	connect(importAction, &QAction::triggered, this, &KTopMenu::importTriggered);
	connect(addCanvasAction, &QAction::triggered, this, &KTopMenu::newCanvasTriggered);

	connect(fileButton, &QPushButton::clicked, this, &KTopMenu::showFileMenu);

	layout->addWidget(fileButton);

	// --- 查看菜单 ---
	viewButton = new QPushButton(QStringLiteral("查看"), this);
	viewButton->setStyleSheet(fileButton->styleSheet()); // 复用文件按钮样式
	viewButton->setFlat(true);
	
	viewMenu = new QMenu(this);
	viewMenu->setStyleSheet(fileMenu->styleSheet()); // 复用文件菜单样式
	
	QActionGroup * zoomGroup = new QActionGroup(this);
	zoomGroup->setExclusive(true);
	
	// 创建不同缩放比例的 Action
	QAction * zoom50 = new QAction("50%", this);
	zoom50->setData(50);
	zoom50->setCheckable(true);
	QAction * zoom100 = new QAction("100%", this);
	zoom100->setData(100);
	zoom100->setCheckable(true);
	zoom100->setChecked(true); // 默认 100%
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
	// 将菜单放到 button 的下面
	fileMenu->popup(fileButton->mapToGlobal(QPoint(0, fileButton->height())));
}

void KTopMenu::showViewMenu()
{
	viewMenu->popup(viewButton->mapToGlobal(QPoint(0, viewButton->height())));
}