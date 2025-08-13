#include "kline.h"
#include <QPainter>

KLine::KLine(QObject* parent)
	: KShape(parent)
{
}

KLine::~KLine()
{
}

void KLine::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);
	// 防止鼠标未移动导致 endPoint定位到 (0, 0)
	if (getEndPoint().x() == -1 && getEndPoint().y() == -1)
		return;

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter.setPen(pen);
	painter.setBrush(getFillColor());

	painter.drawLine(getStartPoint(), getEndPoint());
}

void KLine::drawShape(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);
	// 防止鼠标未移动导致 endPoint定位到 (0, 0)
	if (getEndPoint().x() == -1 && getEndPoint().y() == -1)
		return;

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter->setPen(pen);
	painter->setBrush(getFillColor());

	painter->drawLine(getStartPoint(), getEndPoint());
}

KShapeType KLine::getShapeType() const
{
	return KShapeType::LineShapeType;
}

std::unique_ptr<KShape> KLine::clone()
{
	auto newLine = std::make_unique<KLine>();
	newLine->setStartPoint(this->getStartPoint());
	newLine->setEndPoint(this->getEndPoint());

	newLine->setBorderWidth(this->getBorderWidth());
	newLine->setBorderStyle(this->getBorderStyle());
	newLine->setBorderColor(this->getBorderColor());
	newLine->setFillColor(this->getFillColor());
	return newLine;
}