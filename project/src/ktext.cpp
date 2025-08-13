#include "ktext.h"
#include <QFontMetrics>
#include <QFont>
#include <QDebug>
#include "kglobaldata.h"

KText::KText(QObject* parent)
	: KShape(parent)
{
}

KText::~KText()
{
}

void KText::drawShape(QPaintDevice* parent)
{
	int TEXT_PADDING = KGlobalData::getGlobalDataIntance().getTextPadding();
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	QFont font;
	QFontMetrics fm(font);
	QPoint off(fm.horizontalAdvance(getText()), fm.height());
	setTextRect(QRect(getStartPoint() - QPoint(0, off.y() / 2) - QPoint(TEXT_PADDING, TEXT_PADDING),
		getStartPoint() + off + QPoint(TEXT_PADDING, TEXT_PADDING) - QPoint(0, off.y() / 2)));
	// painter.drawText(getStartPoint() + QPoint(0, off.y() / 2), getText());
	painter.drawText(getTextRect(), Qt::AlignCenter, getText());
}

void KText::drawShape(QPainter* painter)
{
	int TEXT_PADDING = KGlobalData::getGlobalDataIntance().getTextPadding();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::TextAntialiasing);

	QFont font;
	QFontMetrics fm(font);
	QPoint off(fm.horizontalAdvance(getText()), fm.height());
	setTextRect(QRect(getStartPoint() - QPoint(0, off.y() / 2) - QPoint(TEXT_PADDING, TEXT_PADDING),
		getStartPoint() + off + QPoint(TEXT_PADDING, TEXT_PADDING) - QPoint(0, off.y() / 2)));
	// painter.drawText(getStartPoint() + QPoint(0, off.y() / 2), getText());
	painter->drawText(getTextRect(), Qt::AlignCenter, getText());
}

KShapeType KText::getShapeType() const
{
	return KShapeType::TextShapeType;
}

std::unique_ptr<KShape> KText::clone()
{
	// ����һ���µ� KText ʵ��
	auto newText = std::make_unique<KText>();

	// �ӻ��� KShape ��������
	newText->setStartPoint(this->getStartPoint());
	newText->setEndPoint(this->getEndPoint());

	// ���� KText ���е�����
	newText->setText(this->getText());
	newText->setTextRect(this->getTextRect());
	newText->setCursorPos(this->getCursorPos());
	newText->setLastCursorPos(this->getLastCursorPos());

	return newText;
}