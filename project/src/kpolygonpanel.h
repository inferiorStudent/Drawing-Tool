#ifndef __K_POLYGON_PANEL_H__
#define __K_POLYGON_PANEL_H__

#include <QWidget>
#include "kglobaldata.h"

class QPushButton;

class KPolygonPanel : public QWidget
{
	Q_OBJECT

public:
	explicit KPolygonPanel(QWidget* parent = nullptr);

signals:
	// ���û��������ѡ��һ��ͼ��ʱ��������ź�
	// ������ѡ���ͼ�����͡���Ӧͼ����ļ���������·����
	void shapeSelected(KGlobalData::KDrawFlag flag, const QString& iconName);

private:
	// ��������е�С��ť
	QPushButton* createToolButton(const QString& iconName);
};

#endif // __K_POLYGON_PANEL_H__