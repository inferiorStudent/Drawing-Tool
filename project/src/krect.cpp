#include "krect.h"
#include <QPainter>

KRect::KRect(QObject* parent)
	: KShape(parent)
{
}

KRect::~KRect()
{
}

void KRect::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);
	if (getEndPoint().x() == -1 && getEndPoint().y() == -1)
		return;

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter.setPen(pen);
	painter.setBrush(getFillColor());

	painter.drawRect(QRect(getStartPoint(), getEndPoint()));
}

void KRect::drawShape(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (getEndPoint().x() == -1 && getEndPoint().y() == -1)
		return;

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter->setPen(pen);
	painter->setBrush(getFillColor());

	painter->drawRect(QRect(getStartPoint(), getEndPoint()));
}

KShapeType KRect::getShapeType() const
{
	return KShapeType::RectShapeType;
}

std::unique_ptr<KShape> KRect::clone()
{
	auto newRect = std::make_unique<KRect>();

	newRect->setStartPoint(this->getStartPoint());
	newRect->setEndPoint(this->getEndPoint());

	newRect->setBorderWidth(this->getBorderWidth());
	newRect->setBorderStyle(this->getBorderStyle());
	newRect->setBorderColor(this->getBorderColor());
	newRect->setFillColor(this->getFillColor());
	return newRect;
}