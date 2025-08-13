#include "kshapefactory.h"

KShapeFactory::KShapeFactory(QObject* parent)
	: QObject(parent)
{

}
KShapeFactory::~KShapeFactory()
{
	//
}

std::unique_ptr<KShape> KShapeFactory::createShape(KGlobalData::KDrawFlag drawflag)
{
	switch (drawflag)
	{
	case KGlobalData::KDrawFlag::RectDrawFlag:
		return std::make_unique<KRect>();
		break;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		return std::make_unique<KCircle>();
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		return std::make_unique<KLine>();
		break;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		return std::make_unique<KPen>();
		break;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		return std::make_unique<KText>();
		break;
	case KGlobalData::KDrawFlag::PentagonDrawFlag:
		return std::make_unique<KPolygon>(5);
		break;
	case KGlobalData::KDrawFlag::TriangleDrawFlag:
		return std::make_unique<KPolygon>(3);
		break;
	case KGlobalData::KDrawFlag::HexagonDrawFlag:
		return std::make_unique<KPolygon>(6);
		break;
	default:
		break;
	}
	return Q_NULLPTR;	// 有返回类型的函数一定要有返回值\o/\o/\o/
}