#ifndef __K_RECT_H__
#define __K_RECT_H__

#include "kshape.h"

class KRect : public KShape
{
	Q_OBJECT
public:
	explicit KRect(QObject* parent = Q_NULLPTR);
	virtual ~KRect();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;
};


#endif //__K_RECT_H__