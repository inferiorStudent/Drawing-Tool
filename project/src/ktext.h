#ifndef __K_TEXT_H__
#define __K_TEXT_H__
#include "kshape.h"

class KText : public KShape
{
	Q_OBJECT
public:
	explicit KText(QObject* parent = Q_NULLPTR);
	virtual ~KText();

	std::unique_ptr<KShape> clone() override;
protected:
	void drawShape(QPaintDevice* parent);
	void drawShape(QPainter* painter);
	KShapeType getShapeType() const;

	
};

#endif //__K_TEXT_H__