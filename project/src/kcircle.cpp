#include "kcircle.h"
#include <QPainter>

KCircle::KCircle(QObject* parent)
	: KShape(parent)
{
}

KCircle::~KCircle()
{
}

void KCircle::drawShape(QPaintDevice* parent)
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

	painter.drawEllipse(QRect(getStartPoint(), getEndPoint()));
}

void KCircle::drawShape(QPainter* painter)
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

	painter->drawEllipse(QRect(getStartPoint(), getEndPoint()));
}

KShapeType KCircle::getShapeType() const
{
	return KShapeType::CircleShapeType;
}

std::unique_ptr<KShape> KCircle::clone()
{
	auto newCircle = std::make_unique<KCircle>();
	newCircle->setStartPoint(this->getStartPoint());
	newCircle->setEndPoint(this->getEndPoint());

	newCircle->setBorderWidth(this->getBorderWidth());
	newCircle->setBorderStyle(this->getBorderStyle());
	newCircle->setBorderColor(this->getBorderColor());
	newCircle->setFillColor(this->getFillColor());
	return newCircle;
}