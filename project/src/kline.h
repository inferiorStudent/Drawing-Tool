#ifndef __K_LINE_H__
#define __K_LINE_H__

#include "kshape.h"

class KLine : public KShape
{
	Q_OBJECT
public:
	explicit KLine(QObject* parent = Q_NULLPTR);
	virtual ~KLine();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;
};


#endif //__K_LINE_H__