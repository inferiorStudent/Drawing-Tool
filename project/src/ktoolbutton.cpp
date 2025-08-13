#include "ktoolbutton.h"

KToolButton::KToolButton(KGlobalData::KDrawFlag drawflag, QWidget * parent)
    : QRadioButton(parent)
    , m_pSvgRenderer(nullptr)
    , m_drawFlag(drawflag)
{
    initToolButton();
}

KToolButton::~KToolButton()
{
}

void KToolButton::initToolButton()
{
    switch (m_drawFlag)
    {
    case KGlobalData::KDrawFlag::MouseDrawFlag:
        m_iconName = "arrow.svg";
        break;
    case KGlobalData::KDrawFlag::PenDrawFlag:
        m_iconName = "edit.svg";
        break;
    case KGlobalData::KDrawFlag::LineDrawFlag:
        m_iconName = "line.svg";
        break;
    case KGlobalData::KDrawFlag::RectDrawFlag:
        m_iconName = "rect.svg";
        break;
    case KGlobalData::KDrawFlag::CircleDrawFlag:
        m_iconName = "circle.svg";
        break;
    case KGlobalData::KDrawFlag::TextDrawFlag:
        m_iconName = "text.svg";
        break;
    default:
        break;
    }
    m_pSvgRenderer = new QSvgRenderer(":/image/normal/" + m_iconName);
    this->setMinimumSize(32, 32);

    connect(this, &KToolButton::toggled, this, &KToolButton::onToggledButton);
}


void KToolButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    m_pSvgRenderer->render(&painter, QRectF(0, 0, 32, 32));
}

void KToolButton::enterEvent(QEvent* event)
{
    if (!isChecked())
        m_pSvgRenderer->load(":/image/hover/" + m_iconName);
    setCursor(Qt::PointingHandCursor);
    update();
}

void KToolButton::leaveEvent(QEvent* event)
{
    if (!isChecked())
        m_pSvgRenderer->load(":/image/normal/" + m_iconName);

    setCursor(Qt::ArrowCursor);
    update();
}


void KToolButton::mouseReleaseEvent(QMouseEvent* e)
{
    if (m_drawFlag == KGlobalData::KDrawFlag::HexagonDrawFlag
        || m_drawFlag == KGlobalData::KDrawFlag::TriangleDrawFlag
        || m_drawFlag == KGlobalData::KDrawFlag::PentagonDrawFlag)
        QRadioButton::mouseReleaseEvent(e);
    else
        setChecked(true);
}

void KToolButton::onToggledButton(bool checked)
{
    if (isChecked())
    {
        m_pSvgRenderer->load(":/image/selected/" + m_iconName);
        KGlobalData::getGlobalDataIntance().setDrawFlag(m_drawFlag);
    }
    else
        m_pSvgRenderer->load(":/image/normal/" + m_iconName);
    update();
}

void KToolButton::resetStatus()
{
	switch (m_drawFlag)
	{
	case KGlobalData::KDrawFlag::MouseDrawFlag:
		m_iconName = "arrow.svg";
		break;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		m_iconName = "edit.svg";
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		m_iconName = "line.svg";
		break;
	case KGlobalData::KDrawFlag::RectDrawFlag:
		m_iconName = "rect.svg";
		break;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		m_iconName = "circle.svg";
		break;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		m_iconName = "text.svg";
		break;
	default:
		break;
	}
    if (KGlobalData::getGlobalDataIntance().getDrawFlag() == m_drawFlag)
        m_pSvgRenderer->load(":/image/selected/" + m_iconName);
    else
        m_pSvgRenderer->load(":/image/normal/" + m_iconName);

    update();
}

void KToolButton::setIcon(const QString& iconName)
{
	m_iconName = iconName;
	// 根据当前是否选中，加载正确的图标状态
	if (isChecked())
    {
		m_pSvgRenderer->load(":/image/selected/" + m_iconName);
	}
	else
    {
		m_pSvgRenderer->load(":/image/normal/" + m_iconName);
	}
	update();
}

void KToolButton::setDrawFlag(const KGlobalData::KDrawFlag& flag)
{
    m_drawFlag = flag;
}