#ifndef __K_POLYGON_H__
#define __K_POLYGON_H__

#include "kshape.h"

class KPolygon : public KShape
{
	Q_OBJECT
public:
	explicit KPolygon(int numEdges = 3, QObject* parent = Q_NULLPTR);
	virtual ~KPolygon();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;

private:
	int m_numEdges = 3;
};


#endif //__K_POLYGON_H__