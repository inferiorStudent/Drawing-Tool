#include "kshape.h"
#include <QDebug>
#include "kglobaldata.h"

constexpr int HANDLE_SIZE = 8;
constexpr int SELECTION_MARGIN = 5;

KShape::KShape(QObject* parent)
	: QObject(parent)
	, m_endPoint(QPoint(-1, -1))
	, m_currentText(QStringLiteral("�������ı�"))
	, m_cursorPosition(0)
	, m_lastCursorPos(0)
	, m_isSelected(false)
{
	m_cursorPosition = m_currentText.length();
}

KShape::~KShape()
{
}

void KShape::move(const QPoint& offset)
{
	m_startPoint += offset;
	m_endPoint += offset;
}

// --- Resize ---
void KShape::moveTop(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setTop(pos.y());
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveBottom(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setBottom(pos.y());
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveLeft(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setLeft(pos.x());
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveRight(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setRight(pos.x());
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveTopLeft(const QPoint& pos)
{
	m_startPoint = pos;
}

void KShape::moveTopRight(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setTopRight(pos);
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveBottomLeft(const QPoint& pos)
{
	QRect rect = normalizedRect();
	rect.setBottomLeft(pos);
	m_startPoint = rect.topLeft();
	m_endPoint = rect.bottomRight();
}

void KShape::moveBottomRight(const QPoint& pos)
{
	m_endPoint = pos;
}

KShapeType KShape::getShapeType() const
{
	return KShapeType::None;
}

QPoint KShape::getStartPoint() const
{
	return m_startPoint;
}

QPoint KShape::getEndPoint() const
{
	return m_endPoint;
}

void KShape::setStartPoint(const QPoint& point)
{
	m_startPoint = point;
}

void KShape::setEndPoint(const QPoint& point)
{
	m_endPoint = point;
}

void KShape::setSelected(bool selected)
{
	m_isSelected = selected;
}

bool KShape::isSelected() const
{
	return m_isSelected;
}


QRect KShape::getShapeRect() const
{
	if (getShapeType() == KShapeType::TextShapeType)
		return m_textRect;
	else
		return normalizedRect();
}

// ���㽻��ʱ ���� topleft ʼ�������Ͻǵ���һ��
QRect KShape::normalizedRect() const
{
	return QRect(m_startPoint, m_endPoint).normalized();
}


bool KShape::isValid() const
{
	return (m_startPoint.x() != m_endPoint.x() || m_startPoint.y() != m_endPoint.y());
}

void KShape::drawSelectionControls(QPainter* painter)
{
	QPen pen;
	pen.setColor(QColor(0, 120, 215));
	pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);

	QRect rect = getShapeRect();
	painter->drawRect(rect);

	painter->setPen(Qt::black);
	painter->setBrush(Qt::white);
	int handleSize = HANDLE_SIZE;

	painter->drawRect(rect.topLeft().x() - handleSize / 2, rect.topLeft().y() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.topRight().x() - handleSize / 2, rect.topRight().y() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.bottomLeft().x() - handleSize / 2, rect.bottomLeft().y() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.bottomRight().x() - handleSize / 2, rect.bottomRight().y() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.left() + rect.width() / 2 - handleSize / 2, rect.top() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.left() + rect.width() / 2 - handleSize / 2, rect.bottom() - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.left() - handleSize / 2, rect.top() + rect.height() / 2 - handleSize / 2, handleSize, handleSize);
	painter->drawRect(rect.right() - handleSize / 2, rect.top() + rect.height() / 2 - handleSize / 2, handleSize, handleSize);
}

// ����¼�����ʱ�����ֱ任
HandlePosition KShape::getHandleAt(const QPoint& pos) const
{
	// if (!m_isSelected) return HandlePosition::None;

	QRect rect = getShapeRect();
	int handleSize = HANDLE_SIZE;

	if (QRect(rect.topLeft() - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::TopLeft;
	if (QRect(rect.topRight() - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::TopRight;
	if (QRect(rect.bottomLeft() - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::BottomLeft;
	if (QRect(rect.bottomRight() - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::BottomRight;
	if (QRect(QPoint(rect.left() + rect.width() / 2, rect.top()) - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::Top;
	if (QRect(QPoint(rect.left() + rect.width() / 2, rect.bottom()) - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::Bottom;
	if (QRect(QPoint(rect.left(), rect.top() + rect.height() / 2) - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::Left;
	if (QRect(QPoint(rect.right(), rect.top() + rect.height() / 2) - QPoint(handleSize / 2, handleSize / 2), QSize(handleSize, handleSize)).contains(pos))
		return HandlePosition::Right;

	// ����һ�����
	if (rect.adjusted(-SELECTION_MARGIN, -SELECTION_MARGIN, SELECTION_MARGIN, SELECTION_MARGIN).contains(pos))
	{
		if (getShapeType() == KShapeType::TextShapeType && m_isTextEditing)
		{
			// ����ͨ��������ĸ�λ���������ı��Ƿ����ڱ༭
			// ֻ���� canvas ���洦��
			return HandlePosition::Edit;
		}
		return HandlePosition::Inside;
	}

	return HandlePosition::None;
}

void KShape::drawSelectingRect(QPainter* painter)
{
	QPen pen;
	pen.setColor(QColor(0, 120, 215));
	pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);
	QBrush backgroundBrush;
	backgroundBrush.setColor(QColor(0, 120, 215, 50)); // ͸��
	backgroundBrush.setStyle(Qt::SolidPattern);
	painter->setBrush(backgroundBrush);

	painter->drawRect(QRect(getStartPoint(), getEndPoint()));
}


QVector<QPoint>& KShape::getTrajectory()
{
	return m_trajectory;
}

void KShape::setTrajectory(const QVector<QPoint>& traj)
{
	m_trajectory = traj;
}

void KShape::addPoint(const QPoint& point)
{
	m_trajectory.append(point);
}

// --- �ı� ---
QString KShape::getText() const
{
	return m_currentText;
}

void KShape::setText(const QString& text)
{
	m_currentText = text;
}

QRect KShape::getTextRect() const
{
	return m_textRect;
}

void KShape::setTextRect(const QRect& rect)
{
	m_textRect = rect;
}

void KShape::drawCursor(QPainter* painter)
{
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(3);
	painter->setPen(pen);
	QFont font;
	QFontMetrics fm(font);
	QString textBeforeCursor = m_currentText.left(m_cursorPosition);
	int cursorX = m_textRect.left() + SELECTION_MARGIN + fm.horizontalAdvance(textBeforeCursor);
	painter->drawLine(cursorX, m_textRect.top() + SELECTION_MARGIN, cursorX, m_textRect.bottom() - SELECTION_MARGIN);
}

void KShape::drawPreText(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::TextAntialiasing);
	QFont font;
	QFontMetrics fm(font);
	int preeditX = m_textRect.left() + fm.horizontalAdvance(m_currentText.left(m_cursorPosition));
	painter->pen();
	painter->setPen(Qt::blue); // Ԥ�༭�ı�����ɫ
	painter->drawText(preeditX, m_textRect.top() + (m_textRect.height() - fm.height()) / 2 + fm.ascent(), m_preditText);

	// �����»���
	painter->setPen(QPen(Qt::blue, 1, Qt::DotLine)); // ���߻�ʵ��
	painter->drawLine(preeditX, m_textRect.top() + (m_textRect.height() - fm.height()) / 2 + fm.ascent() + 2,
		preeditX + fm.horizontalAdvance(m_preditText), m_textRect.top() + (m_textRect.height() - fm.height()) / 2 + fm.ascent() + 2);
}

QString KShape::getPredText() const
{
	return m_preditText;
}

void KShape::setPredText(const QString& predict)
{
	m_preditText = predict;
}

void KShape::drawTextSelected(QPainter* painter)
{
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setRenderHint(QPainter::TextAntialiasing);
	int TEXT_PADDING = KGlobalData::getGlobalDataIntance().getTextPadding();

	QFont font;
	painter->setFont(font);
	QRect textDrawRect = m_textRect.adjusted(TEXT_PADDING, TEXT_PADDING, -TEXT_PADDING, -TEXT_PADDING);
	QFontMetrics fm(font);

	int selectionStart = qMin(m_lastCursorPos, m_cursorPosition);
	int selectionEnd = qMax(m_lastCursorPos, m_cursorPosition);

	if (selectionStart == selectionEnd)
	{
		painter->setPen(Qt::black);
		painter->drawText(textDrawRect, Qt::AlignLeft | Qt::AlignVCenter, m_currentText);
	}
	else
	{
		// ��������
		// 1. ����ѡ������ǰ���ı�
		QString prefixText = m_currentText.left(selectionStart);
		painter->setPen(Qt::black); // ǰ׺������ɫ
		painter->drawText(textDrawRect, Qt::AlignLeft | Qt::AlignVCenter, prefixText);

		// ��ȡǰ׺�ı������ؿ��
		int prefixWidth = fm.horizontalAdvance(prefixText);

		// 2. ����ѡ������ (���װ���)
		QString selectedText = m_currentText.mid(selectionStart, selectionEnd - selectionStart);
		int selectedWidth = fm.horizontalAdvance(selectedText);

		// ����ѡ������Ļ��ƾ���
		QRect selectionRect = QRect(textDrawRect.left() + prefixWidth,
			textDrawRect.top(),
			selectedWidth,
			textDrawRect.height());

		// ������ɫ����
		painter->setBrush(QColor(65, 105, 225)); // ��ɫ����
		painter->setPen(Qt::NoPen);   // �����ޱ߿�
		painter->drawRect(selectionRect);

		// ���ư�ɫѡ������
		painter->setPen(Qt::white); // ѡ��������ɫ
		painter->drawText(selectionRect, Qt::AlignLeft | Qt::AlignVCenter, selectedText); // ֱ�ӻ��Ƶ�ѡ�о���

		// 3. ����ѡ���������ı� (��ɫ)
		QString suffixText = m_currentText.mid(selectionEnd);
		int suffixX = textDrawRect.left() + prefixWidth + selectedWidth; // ��׺�ı�����ʼX����

		painter->setPen(Qt::black); // ��׺������ɫ
		// ����׺�ı���������ȷ��ƫ��λ��
		painter->drawText(suffixX, textDrawRect.top(), textDrawRect.width() - suffixX + textDrawRect.left(), textDrawRect.height(),
			Qt::AlignLeft | Qt::AlignVCenter, suffixText);
	}
}

void KShape::setCursorPos(int pos)
{
	m_cursorPosition = pos;
}

int KShape::getCursorPos() const
{
	return m_cursorPosition;
}

int KShape::getLastCursorPos() const
{
	return m_lastCursorPos;
}

void KShape::setLastCursorPos(int pos)
{
	m_lastCursorPos = pos;
}

void KShape::changeCursorPos(const QPoint& pos)
{
	int TEXT_PADDING = KGlobalData::getGlobalDataIntance().getTextPadding();
	QFont font;
	QFontMetrics fm(font);
	// ����������� X ����������ı����ڲ��ı��������Ե��ƫ��
	int clickX = pos.x() - (m_textRect.left() + TEXT_PADDING);

	m_cursorPosition = 0;
	int currentTextWidth = 0;       // �ۼ��ַ����

	for (int i = 0; i < m_currentText.length(); ++i)
	{
		int charWidth = fm.horizontalAdvance(m_currentText.at(i));
		if (clickX <= currentTextWidth + charWidth / 2)
		{
			m_cursorPosition = i;
			break;
		}
		currentTextWidth += charWidth;
		m_cursorPosition = i + 1;
	}
}

void KShape::setTextEdit(bool edit)
{
	m_isTextEditing = edit;
}

bool KShape::getTextEdit() const
{
	return m_isTextEditing;
}

void KShape::setBorderWidth(int width)
{
	m_borderWidth = width;
}

void KShape::setBorderStyle(Qt::PenStyle style)
{
	m_borderStyle = style;
}

void KShape::setBorderColor(const QColor& color)
{
	m_borderColor = color;
}

void KShape::setFillColor(const QColor& color)
{
	m_fillColor = color;
}

int KShape::getBorderWidth() const
{
	return m_borderWidth;
}

Qt::PenStyle KShape::getBorderStyle() const
{
	return m_borderStyle;
}

QColor KShape::getBorderColor() const
{
	return m_borderColor;
}

QColor KShape::getFillColor() const
{
	return m_fillColor;
}


void KShape::setNumEdges(int num)
{
	m_numEdges = num;
}

int KShape::getNumEdges() const
{
	return m_numEdges;
}
