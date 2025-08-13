#include "kpolygonpanel.h"
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>

KPolygonPanel::KPolygonPanel(QWidget* parent)
	: QWidget(parent, Qt::Popup)
{
	this->setStyleSheet(
		"QWidget {"
		"   background-color: #23272A;"
		"   border-radius: 5px;"
		"   padding: 8px;"
		"}"
	);

	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(10); // ����ͼ����

	// ��������Ӹ��ֶ���ΰ�ť
	QPushButton* triangleBtn = createToolButton("triangle.svg");
	QPushButton* pentagonBtn = createToolButton("pentagon.svg");
	QPushButton* hexagonBtn = createToolButton("hexagon.svg");

	gridLayout->addWidget(triangleBtn, 0, 0);
	gridLayout->addWidget(pentagonBtn, 0, 1);
	gridLayout->addWidget(hexagonBtn, 1, 0);

	// ���Ӱ�ť����¼����źŷ���
	connect(triangleBtn, &QPushButton::clicked, this, [this]() {
		emit shapeSelected(KGlobalData::KDrawFlag::TriangleDrawFlag, "triangle.svg");
		this->hide(); // ѡ����Զ�����
		});
	connect(pentagonBtn, &QPushButton::clicked, this, [this]() {
		emit shapeSelected(KGlobalData::KDrawFlag::PentagonDrawFlag, "pentagon.svg");
		this->hide();
		});
	connect(hexagonBtn, &QPushButton::clicked, this, [this]() {
		emit shapeSelected(KGlobalData::KDrawFlag::HexagonDrawFlag, "hexagon.svg");
		this->hide();
		});
}

QPushButton* KPolygonPanel::createToolButton(const QString& iconName)
{
	QPushButton* button = new QPushButton(this);
	button->setFixedSize(32, 32);
	button->setIconSize(QSize(32, 32));
	button->setIcon(QIcon(":/image/normal/" + iconName));
	button->setFlat(true);
	button->setStyleSheet(
		"QPushButton { background-color: transparent; border: none; }"
		"QPushButton:hover { background-color: #555; border-radius: 4px; }"
	);
	return button;
}