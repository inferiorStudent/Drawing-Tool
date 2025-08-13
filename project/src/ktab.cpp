#include "ktab.h"
#include <QStyle>

KTab::KTab(QWidget* parent)
	: QWidget(parent)
	, m_currentIndex(-1)
{
	setFixedHeight(40);
	setObjectName("KTab");
	setAttribute(Qt::WA_StyledBackground, true);

	this->setStyleSheet(
		"KTab#KTab {"
		"   background-color: #404040;"
		"}"
		"KTabItem#KTabItem {"
		"   background-color: #2D2D2D;"
		"   border: 1px solid #2D2D2D;"
		"   border-bottom: none;"
		"   border-top-right-radius: 8px;"
		"   padding: 0 5px;"
		"   margin-top: 0px;"
		"}"
		"KTabItem#KTabItem[selected='true'] {"
		"   background-color: #808080;"
		"}"
		"KTabItem#KTabItem:hover {"
		"   background-color: #A0A0A0;"
		"}"
		"KTabItem QLabel {"
		"   color: white;"
		"}"
		"QPushButton#KTabItemCloseButton {"
		"   border: none; background-color: transparent; border-radius: 6px;"
		"   min-width: 16px; max-width: 16px; min-height: 16px; max-height: 16px;"
		"   image: url(:/image/close.svg);"
		"}"
		"QPushButton#KTabItemCloseButton:hover { background-color: #555; }"
		"QPushButton#AddButton {"
		"   border: none; background-color: transparent; border-radius: 4px;"
		"   min-width: 24px; max-width: 24px; min-height: 24px; max-height: 24px;"
		"   image: url(:/image/add.svg);"
		"}"
		"QPushButton#AddButton:hover { background-color: #444; }"
	);

	m_layout = new QHBoxLayout(this);
	m_layout->setContentsMargins(5, 0, 0, 0);
	m_layout->setSpacing(2);
	m_layout->setAlignment(Qt::AlignLeft);

	m_addButton = new QPushButton(this);
	m_addButton->setObjectName("AddButton");
	m_addButton->setCursor(Qt::PointingHandCursor);
	connect(m_addButton, &QPushButton::clicked, this, &KTab::newTabRequested);
	m_layout->addWidget(m_addButton);

	//m_layout->addStretch();
	setLayout(m_layout);
}

int KTab::addTab(const QString& name)
{
	KTabItem* newTab = new KTabItem(name, this);
	newTab->setCursor(Qt::PointingHandCursor);

	connect(newTab, &KTabItem::clicked, this, &KTab::onTabClicked);
	connect(newTab, &KTabItem::closeRequested, this, &KTab::onTabCloseRequested);

	// 新标签插入到+右边
	int index = static_cast<int>(m_tabs.size());
	m_layout->insertWidget(index, newTab);
	m_tabs.push_back(newTab);

	// 只有一个tab
	if (m_tabs.size() == 1)
	{
		setCurrentTab(0);
	}
	return index;
}

void KTab::closeTab(int index)
{
	if (index < 0 || index >= m_tabs.size()) return;

	KTabItem* tabToRemove = m_tabs[index];
	m_tabs.erase(m_tabs.begin() + index);

	m_layout->removeWidget(tabToRemove);
	delete tabToRemove;

	if (m_tabs.empty())
	{
		m_currentIndex = -1;
		emit currentChanged(-1);
	}
	else if (m_currentIndex >= index)
	{
		m_currentIndex = std::max(0, m_currentIndex - 1);
		setCurrentTab(m_currentIndex);
	}
	updateTabStyles();
}

void KTab::setCurrentTab(int index)
{
	if (index < 0 || index >= m_tabs.size() || index == m_currentIndex)
		return;

	m_currentIndex = index;
	updateTabStyles();
	emit currentChanged(m_currentIndex);
}

void KTab::updateTabStyles()
{
	for (int i = 0; i < m_tabs.size(); ++i)
	{
		m_tabs[i]->setSelected(i == m_currentIndex);
	}
}

int KTab::getTabIndex(KTabItem* item)
{
	for (int i = 0; i < m_tabs.size(); ++i)
	{
		if (m_tabs[i] == item)
		{
			return i;
		}
	}
	return -1;
}

void KTab::onTabClicked(KTabItem* item)
{
	if (item)
	{
		int index = getTabIndex(item);
		setCurrentTab(index);
	}
}

void KTab::onTabCloseRequested(KTabItem* item)
{
	if (item)
	{
		int index = getTabIndex(item);
		emit tabCloseRequested(index);
	}
}