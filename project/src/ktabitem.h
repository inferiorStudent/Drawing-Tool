#ifndef __K_TABITEM_H__
#define __K_TABITEM_H__

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyle>
#include <QMouseEvent>

class KTabItem : public QWidget
{
	Q_OBJECT
public:
	KTabItem(const QString& text, QWidget* parent = nullptr);
	void setSelected(bool selected);

signals:
	void clicked(KTabItem* item);
	void closeRequested(KTabItem* item);

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	QLabel* m_label;
	QPushButton* m_closeButton;
	bool m_isSelected;
};


#endif //__K_TABITEM_H__