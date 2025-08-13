#include "kpolygon.h"
#include "kglobaldata.h"
#include <QPainter>
#include <QtMath>

KPolygon::KPolygon(int numEdges, QObject* parent)
	: KShape(parent)
	, m_numEdges(numEdges)
{
	setNumEdges(m_numEdges);
}

KPolygon::~KPolygon()
{
}

void KPolygon::drawShape(QPaintDevice* parent)
{
}

void KPolygon::drawShape(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (getEndPoint().x() == -1 && getEndPoint().y() == -1)
		return;

	m_numEdges = getNumEdges();
	if (m_numEdges < 3)
		return;

	QPointF startPoint(getStartPoint().x(), getStartPoint().y());
	QPointF endPoint(getEndPoint().x(), getEndPoint().y());

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter->setPen(pen);
	painter->setBrush(getFillColor());

	QPointF center = (startPoint + endPoint) / 2.0;
	qreal radius = qMin(qAbs(endPoint.x() - startPoint.x()), qAbs(endPoint.y() - startPoint.y())) / 2.0;

	QPolygonF polygon;
	for (int i = 0; i < m_numEdges; ++i)
	{
		// 可以根据需要调整初始角度，例如 M_PI / 2.0 可以让多边形一个顶点朝上
		qreal angle = 2 * M_PI * i / m_numEdges;
		qreal x = center.x() + radius * qCos(angle);
		qreal y = center.y() - radius * qSin(angle);
		polygon << QPointF(x, y);
	}

	painter->drawPolygon(polygon);
}

KShapeType KPolygon::getShapeType() const
{
	if (m_numEdges == 3)
		return KShapeType::TriangleType;
	else if (m_numEdges == 5)
		return KShapeType::PentagonType;
	else if (m_numEdges == 6)
		return KShapeType::HexagonType;
	else
		return KShapeType::None;
}

std::unique_ptr<KShape> KPolygon::clone()
{
	auto newRect = std::make_unique<KPolygon>();

	newRect->setStartPoint(this->getStartPoint());
	newRect->setEndPoint(this->getEndPoint());

	newRect->setBorderWidth(this->getBorderWidth());
	newRect->setBorderStyle(this->getBorderStyle());
	newRect->setBorderColor(this->getBorderColor());
	newRect->setFillColor(this->getFillColor());
	
	newRect->setNumEdges(getNumEdges());
	return newRect;
}