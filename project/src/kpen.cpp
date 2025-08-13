#include "kpen.h"
#include <QPainter>

KPen::KPen(QObject* parent)
	: KShape(parent)
	, m_maxX(0)
	, m_maxY(0)
	, m_minX(INT_MAX)
	, m_minY(INT_MAX)
{
}

KPen::~KPen()
{
}

void KPen::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter.setPen(pen);
	painter.setBrush(getFillColor());

	painter.drawPolyline(QPolygon(getTrajectory()));
	setStartEndPoint();
}

void KPen::drawShape(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);

	QPen pen;
	pen.setWidth(getBorderWidth());
	pen.setStyle(getBorderStyle());
	pen.setColor(getBorderColor());

	painter->setPen(pen);
	painter->setBrush(getFillColor());

	painter->drawPolyline(QPolygon(getTrajectory()));
	setStartEndPoint();
}

KShapeType KPen::getShapeType() const
{
	return KShapeType::PenShapeType;
}

void KPen::setStartEndPoint()
{
	m_maxX = m_maxY = 0;
	m_minX = m_minY = INT_MAX;
	QVector<QPoint>& points = getTrajectory();
	for (int i = 0; i < points.size(); i++)
	{
		m_maxX = m_maxX < points[i].x() ? points[i].x() : m_maxX;
		m_maxY = m_maxY < points[i].y() ? points[i].y() : m_maxY;
		m_minX = m_minX < points[i].x() ? m_minX : points[i].x();
		m_minY = m_minY < points[i].y() ? m_minY : points[i].y();
	}
	setStartPoint(QPoint(m_minX, m_minY));
	setEndPoint(QPoint(m_maxX, m_maxY));
}

void KPen::move(const QPoint& offset)
{
	QVector<QPoint>& points = getTrajectory();
	for (int i = 0; i < points.size(); i++)
	{
		points[i] += offset;
	}
	setStartEndPoint();
	setStartPoint(QPoint(m_minX, m_minY) + offset);
	setEndPoint(QPoint(m_maxX, m_maxY) + offset);
}

std::unique_ptr<KShape> KPen::clone()
{
	auto newPen = std::make_unique<KPen>();
	newPen->setStartPoint(this->getStartPoint());
	newPen->setEndPoint(this->getEndPoint());

	newPen->setBorderWidth(this->getBorderWidth());
	newPen->setBorderStyle(this->getBorderStyle());
	newPen->setBorderColor(this->getBorderColor());
	newPen->setFillColor(this->getFillColor());

	QVector<QPoint>& tmp = getTrajectory();
	newPen->setTrajectory(QVector<QPoint>(tmp.begin(), tmp.end()));
	return newPen;
}