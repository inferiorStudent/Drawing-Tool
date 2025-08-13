#ifndef __K_SHAPE_H__
#define __K_SHAPE_H__

#include <QObject>
#include <QPoint>
#include <QPaintDevice>
#include <QPainter>
#include <QVector>
#include <QBrush>
#include <memory>

// Enum for the type of shape
enum class KShapeType
{
	None = 0,
	PenShapeType,
	LineShapeType,
	CircleShapeType,
	RectShapeType,
	TextShapeType,
	TriangleType,
	PentagonType,
	HexagonType
};

// ����ѡ��ͼ�εĲ���
enum class HandlePosition
{
	None,
	TopLeft,
	Top,
	TopRight,
	Left,
	Inside,
	Right,
	BottomLeft,
	Bottom,
	BottomRight,
	Edit // ����û�õ����������ã�
};


class KShape : public QObject
{
	Q_OBJECT
public:
	explicit KShape(QObject* parent = Q_NULLPTR);
	virtual ~KShape();

	// �������ûɶ�ã�������д��һ�����ݻ��ʵĺ���
	virtual void drawShape(QPaintDevice* parent = nullptr) = 0;
	// ���һ��ͼ��
	virtual std::unique_ptr<KShape> clone() = 0;

	virtual void drawShape(QPainter* painter) = 0;

	virtual void move(const QPoint& offset);
	virtual void moveTop(const QPoint& pos);
	virtual void moveBottom(const QPoint& pos);
	virtual void moveLeft(const QPoint& pos);
	virtual void moveRight(const QPoint& pos);
	virtual void moveTopLeft(const QPoint& pos);
	virtual void moveTopRight(const QPoint& pos);
	virtual void moveBottomLeft(const QPoint& pos);
	virtual void moveBottomRight(const QPoint& pos);

	virtual KShapeType getShapeType() const;
	QPoint getStartPoint() const;
	QPoint getEndPoint() const;
	void setStartPoint(const QPoint& point);
	void setEndPoint(const QPoint& point);
	QRect getShapeRect() const;
	bool isValid() const;

	void setBorderWidth(int width);
	void setBorderStyle(Qt::PenStyle style);
	void setBorderColor(const QColor & color);
	void setFillColor(const QColor & color);
	int getBorderWidth() const;
	Qt::PenStyle getBorderStyle() const;
	QColor getBorderColor() const;
	QColor getFillColor() const;

	void drawSelectionControls(QPainter* painter);
	void setSelected(bool selected);
	bool isSelected() const;
	virtual HandlePosition getHandleAt(const QPoint& pos) const;

	// ����
	QVector<QPoint>& getTrajectory();
	void setTrajectory(const QVector<QPoint>& traj);
	void addPoint(const QPoint& point);

	// �ı�
	QString getText() const;
	void setText(const QString& text);
	QRect getTextRect() const;
	void setTextRect(const QRect& rect);
	void drawCursor(QPainter* painter);
	void drawPreText(QPainter* painter);
	void drawTextSelected(QPainter* painter);
	void setCursorPos(int pos);
	int getCursorPos() const;
	int getLastCursorPos() const;
	void setLastCursorPos(int pos);
	void changeCursorPos(const QPoint& pos);
	void setTextEdit(bool edit);
	bool getTextEdit() const;
	QString getPredText() const;
	void setPredText(const QString& predict);

	// �����
	void setNumEdges(int num);
	int getNumEdges() const;
	
	// ��������ѡ���
	void drawSelectingRect(QPainter* painter);

protected:
	
	QRect normalizedRect() const;

private:
	QPoint m_startPoint;
	QPoint m_endPoint;
	bool m_isSelected = false;

	// ���ʹ켣
	QVector<QPoint> m_trajectory;

	// ��ʽ
	int m_borderWidth = 2;
	Qt::PenStyle m_borderStyle = Qt::SolidLine;
	QColor m_borderColor = Qt::black;
	QColor m_fillColor = Qt::white;

	// �ı�
	QString m_currentText;
	QString m_preditText = "";
	QRect m_textRect;
	int m_cursorPosition;
	int m_lastCursorPos;
	bool m_isTextEditing = false;

	// �����
	int m_numEdges = 3;
	
};


#endif //__K_SHAPE_H__
