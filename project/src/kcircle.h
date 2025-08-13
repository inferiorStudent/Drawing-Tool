#ifndef __K_CIRCLE_H__
#define __K_CIRCLE_H__

#include "kshape.h"

class KCircle : public KShape
{
	Q_OBJECT
public:
	explicit KCircle(QObject* parent = Q_NULLPTR);
	virtual ~KCircle();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;
};


#endif //__K_CIRCLE_H__