#ifndef __K_PEN_H__
#define __K_PEN_H__

#include "kshape.h"

class KPen : public KShape
{
	Q_OBJECT
public:
	explicit KPen(QObject* parent = Q_NULLPTR);
	virtual ~KPen();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;
	void setStartEndPoint();
	void move(const QPoint& offset) override;

private:
	int m_maxX;
	int m_maxY;

	int m_minX;
	int m_minY;
};


#endif //__K_PEN_H__