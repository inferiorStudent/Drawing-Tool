#ifndef __K_CANVAS_H__
#define __K_CANVAS_H__

#include "kglobaldata.h"
#include "kshape.h"
#include <QWidget>
#include <QKeyEvent>
#include <QSvgGenerator>
#include <vector>
#include <memory>
#include <stack>

class KCanvas : public QWidget
{
	Q_OBJECT

public:
	explicit KCanvas(QWidget* parent = Q_NULLPTR);
	~KCanvas();

	void deleteShapes(const std::vector<KShape*>& shapesToDelete);
	void paintToDevice(QPainter* painter);
	int getBaseWidth() const;
	int getBaseHeight() const;
	QColor getCanvasColor() const;
	// FOR TEST
	const std::vector<std::unique_ptr<KShape>>& getShapes() const;

signals:
	// 图形选择后通知右侧属性面板 改变其值显示
	void selectionChanged(KCanvas* activeCanvas, const std::vector<KShape*>&selectedShapes);

public slots:
	// 右键 / 快捷键命令
	void selectAll();
	void undo();
	void cut();
	void copy();
	void duplicate();
	void paste();
	void deleteSelection();
	void bringForward();
	void sendBackward();
	void bringToFront();
	void sendToBack();

	void setZoom(double factor);

	void onSetBorderWidth(int width);
	void onSetBorderStyle(int style);
	void onSetBorderColor(const QColor & color);
	void onSetFillColor(const QColor & color);
	void onSetCanvasColor(const QColor & color);
	void onSetCanvasWidth(int width);
	void onSetCanvasHeight(int height);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
	void inputMethodEvent(QInputMethodEvent* event) override;	// 输入法中文支持
	QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private:
	void updateCursorStyle(const QPoint& pos);
	void deselectAll();
	KShape* getShapeAt(const QPoint& pos);
	KShape* getAnSelected(const QPoint& pos);
	void handleTransformation(const QPoint& pos);
	void saveStateForUndo(const std::vector<KShape*>& shapes, const QString& commandName);
	void blinkCursor();
	void saveState();

private:
	enum class State
	{
		Idle,
		Drawing,
		Selecting,
		Transforming    // 拉伸、编辑
	};
	State m_state = State::Idle;

	int m_width;
	int m_height;
	QColor m_color;

	std::vector<std::unique_ptr<KShape>> m_shapes;
	std::vector<KShape*> m_selectedShapes;
	std::vector<std::vector<std::unique_ptr<KShape>>> m_history;
	std::vector<std::unique_ptr<KShape>> m_clipboard;

	KShape* m_drawingShape = nullptr; // 正在绘制的图
	QRect m_selectionRect;            // 鼠标选择框
	HandlePosition m_currentHandle = HandlePosition::None;
	QPoint m_lastMousePos;            // 上一个鼠标的位置

	// 缩放相关
	double m_zoomFactor = 1.0;
	QSize m_baseSize;

	// Text
	bool m_cursorVisible = false;
	bool m_isTextSelected = false;
	KShape* m_editingText = nullptr;		// 若不为空 则文本框处于编辑状态
	QTimer* m_cursorBlinkTimer;

};

#endif //__K_CANVAS_H__
