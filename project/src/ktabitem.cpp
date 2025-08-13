#include "ktabitem.h"

KTabItem::KTabItem(const QString& text, QWidget* parent)
	: QWidget(parent)
	, m_isSelected(false)
{
	setObjectName("KTabItem");
	setAttribute(Qt::WA_StyledBackground, true);	// 一定要加这句话, 使配置的样式生效
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(10, 5, 5, 5);
	layout->setSpacing(5);

	m_label = new QLabel(text, this);
	m_closeButton = new QPushButton(this);
	m_closeButton->setObjectName("KTabItemCloseButton");

	layout->addWidget(m_label);
	layout->addWidget(m_closeButton);
	setLayout(layout);

	connect(m_closeButton, &QPushButton::clicked, this, [this]() {
		emit closeRequested(this);
		});
}

void KTabItem::setSelected(bool selected)
{
	m_isSelected = selected;
	setProperty("selected", selected);
	style()->unpolish(this);
	style()->polish(this);
}

void KTabItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked(this);
	}
}