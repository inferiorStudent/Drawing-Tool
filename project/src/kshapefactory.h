#ifndef __K_SHAPE_FACTORY_H__
#define __K_SHAPE_FACTORY_H__

#include <QObject>
#include "kglobaldata.h"
#include "krect.h"
#include "kcircle.h"
#include "kline.h"
#include "ktext.h"
#include "kpen.h"
#include "kpolygon.h"

class KShapeFactory : public QObject
{
	Q_OBJECT
public:
	explicit KShapeFactory(QObject* parent = Q_NULLPTR);
	~KShapeFactory();
	static std::unique_ptr<KShape> createShape(KGlobalData::KDrawFlag drawflag);
};



#endif //__K_SHAPE_FACTORY_H__