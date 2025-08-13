#ifndef __K_TAB_H__
#define __K_TAB_H__

#include <vector>
#include "ktabitem.h"

class KTabItem;

class KTab : public QWidget
{
	Q_OBJECT

public:
	explicit KTab(QWidget* parent = nullptr);
	int addTab(const QString& name);
	void closeTab(int index);
	void setCurrentTab(int index);

signals:
	void currentChanged(int index);      // 活动标签改变时
	void tabCloseRequested(int index);   // 活动标签关闭时
	void newTabRequested();              // 新建画布

private slots:
	void onTabClicked(KTabItem* item);
	void onTabCloseRequested(KTabItem * item);

private:
	void updateTabStyles();
	int getTabIndex(KTabItem* item);

	QHBoxLayout* m_layout;
	QPushButton* m_addButton;
	std::vector<KTabItem*> m_tabs;
	int m_currentIndex;
};


#endif // __K_TAB_H__