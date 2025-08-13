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
	// 当用户在面板中选择一个图形时，发射此信号
	// 参数：选择的图形类型、对应图标的文件名（不含路径）
	void shapeSelected(KGlobalData::KDrawFlag flag, const QString& iconName);

private:
	// 创建面板中的小按钮
	QPushButton* createToolButton(const QString& iconName);
};

#endif // __K_POLYGON_PANEL_H__