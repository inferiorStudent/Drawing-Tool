#include "ktoolbar.h"
#include "kglobaldata.h"
#include <QButtonGroup>
#include <QApplication>
#include <QMouseEvent>

KToolBar::KToolBar(QWidget* parent)
    : QWidget(parent)
    , m_pVLayout(Q_NULLPTR)
    , m_pBtnGroup(Q_NULLPTR)
    , m_pMouseBtn(Q_NULLPTR)
    , m_pPenBtn(Q_NULLPTR)
    , m_pLineBtn(Q_NULLPTR)
    , m_pRectBtn(Q_NULLPTR)
    , m_pCircleBtn(Q_NULLPTR)
    , m_pTextBtn(Q_NULLPTR)
    , m_pPolygonBtn(Q_NULLPTR)
{
    setMaximumWidth(100);
    setAttribute(Qt::WA_StyledBackground, true);// �Զ���ؼ���ʽ����Ч
    setStyleSheet("background-color:#23272A");// ������ʽ��

    //��ֱ���ֶ���
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setAlignment(Qt::AlignVCenter | Qt::AlignTop);// ���ö��뷽ʽ
    m_pVLayout->setSpacing(25);// ���ò������е�����ļ��

    m_pBtnGroup = new QButtonGroup(this);
    m_pBtnGroup->setExclusive(true);// ���û��⣬ֻ����һ����ť���� checked ״̬

    // ���� �Զ��尴ť
    m_pMouseBtn = new KToolButton(KGlobalData::KDrawFlag::MouseDrawFlag, this);
    m_pPenBtn = new KToolButton(KGlobalData::KDrawFlag::PenDrawFlag, this);
    m_pLineBtn = new KToolButton(KGlobalData::KDrawFlag::LineDrawFlag, this);
    m_pRectBtn = new KToolButton(KGlobalData::KDrawFlag::RectDrawFlag, this);
    m_pCircleBtn = new KToolButton(KGlobalData::KDrawFlag::CircleDrawFlag, this);
    m_pTextBtn = new KToolButton(KGlobalData::KDrawFlag::TextDrawFlag, this);
	m_pPolygonBtn = new KToolButton(KGlobalData::KDrawFlag::TriangleDrawFlag, this);
	m_pPolygonBtn->setIcon("triangle.svg");

    // ����ť��ӵ� ��ť����
    m_pBtnGroup->addButton(m_pMouseBtn);
    m_pBtnGroup->addButton(m_pPenBtn);
    m_pBtnGroup->addButton(m_pLineBtn);
    m_pBtnGroup->addButton(m_pRectBtn);
    m_pBtnGroup->addButton(m_pCircleBtn);
    m_pBtnGroup->addButton(m_pTextBtn);
    m_pBtnGroup->addButton(m_pPolygonBtn);

    // ���Զ��尴ť��ӵ���������
    m_pVLayout->addWidget(m_pMouseBtn);
    m_pVLayout->addWidget(m_pPenBtn);
    m_pVLayout->addWidget(m_pLineBtn);
    m_pVLayout->addWidget(m_pRectBtn);
    m_pVLayout->addWidget(m_pCircleBtn);
    m_pVLayout->addWidget(m_pTextBtn);
    m_pVLayout->addWidget(m_pPolygonBtn);

    // ����������ӵ� qwidget ��
    setLayout(m_pVLayout);

	// --- ����������������� ---
	m_pPolygonPanel = new KPolygonPanel(this);
	m_pPolygonPanel->hide(); // ��ʼ����
	
	connect(m_pPolygonBtn, &KToolButton::clicked, this, &KToolBar::onPolygonButtonClicked);
	connect(m_pPolygonPanel, &KPolygonPanel::shapeSelected, this, &KToolBar::onPolygonSelected);

    // QApplication::instance()->installEventFilter(this);
}

KToolBar::~KToolBar()
{
}

void KToolBar::resetButtonState()
{
    m_pMouseBtn->resetStatus();
    m_pPenBtn->resetStatus();
    m_pRectBtn->resetStatus();
    m_pTextBtn->resetStatus();
    m_pCircleBtn->resetStatus();
    m_pLineBtn->resetStatus();
    m_pPolygonBtn->resetStatus();
}

void KToolBar::onPolygonButtonClicked()
{
	if (m_pPolygonPanel->isVisible())
	{
		m_pPolygonPanel->hide();
	}
	else
	{
		// ����������ʾλ�ã��ڰ�ť���Ҳ�
		QPoint pos = m_pPolygonBtn->mapToGlobal(QPoint(m_pPolygonBtn->width(), 0));
		m_pPolygonPanel->move(pos + QPoint(16, 0));
		m_pPolygonPanel->show();
	}
}

void KToolBar::onPolygonSelected(KGlobalData::KDrawFlag flag, const QString & iconName)
{
	m_pPolygonBtn->setDrawFlag(flag);
	m_pPolygonBtn->setIcon(iconName);
    KGlobalData::getGlobalDataIntance().setDrawFlag(flag);
	m_pPolygonBtn->setChecked(true);
}