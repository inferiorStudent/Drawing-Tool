#ifndef __K_STYLE_PANEL_H__
#define __K_STYLE_PANEL_H__

#include <QWidget>
#include <QColorDialog>
#include "linestyledelegate.h"
#include "linestylecombox.h"

// ǰ������
class QFormLayout;
class QSpinBox;
class QComboBox;
class QPushButton;
class KCanvas;
class KShape;

class KStylePanel : public QWidget
{
	Q_OBJECT
public:
	explicit KStylePanel(QWidget* parent = nullptr);

public slots:
	void onSelectionChanged(KCanvas* activeCanvas, const std::vector<KShape*>& selectedShapes);

signals:
	void canvasWidthChanged(int width);
	void canvasHeightChanged(int height);
	void canvasColorChanged(const QColor& color);

	void borderWidthChanged(int width);
	void borderStyleChanged(int style); // Qt::PenStyle enum
	void borderColorChanged(const QColor& color);
	void fillColorChanged(const QColor& color);

private slots:
	void openBorderColorDialog();
	void openFillColorDialog();
	void openCanvasColorDialog();
	void handleBorderStyleChange(int index);

private:
	void updateDisplay(KCanvas* canvas, KShape* shape);

	// ����
	QSpinBox* m_canvasWidthSpinBox;
	QSpinBox* m_canvasHeightSpinBox;
	QPushButton* m_canvasColorButton;

	// ��״
	QWidget* m_shapePropertiesWidget;
	QSpinBox* m_borderWidthSpinBox;
	QComboBox* m_borderStyleComboBox;
	QPushButton* m_borderColorButton;
	QPushButton* m_fillColorButton;

	QWidget* m_mainContentWidget; // ���ؼ�������������ʾ/����
	// ��ֹ�ź�ѭ��
	bool m_isUpdatingFromSignal = false;
};

#endif // __K_STYLE_PANEL_H__