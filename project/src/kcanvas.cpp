#include "kcanvas.h"
#include "kshapefactory.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QContextMenuEvent>

#include <QTimer>
#include <QInputMethodEvent>
constexpr int CURSOR_BLINK_INTERVAL_MS = 500;


KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_cursorBlinkTimer(new QTimer(this))
{
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	
	m_height = KGlobalData::getGlobalDataIntance().getCanvasHeight();
	m_width = KGlobalData::getGlobalDataIntance().getCanvasWidth();
	m_baseSize.setWidth(m_width); // ��������ߴ�
	m_baseSize.setHeight(m_height);
	m_color = KGlobalData::getGlobalDataIntance().getCanvasColor();
	QString canvas_color = "background-color:#" + KGlobalData::getGlobalDataIntance().getCanvasColor();
	setFixedSize(m_baseSize);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(canvas_color);

	// �������뷨֧�� ***
	setAttribute(Qt::WA_InputMethodEnabled);

	connect(m_cursorBlinkTimer, &QTimer::timeout, this, &KCanvas::blinkCursor);
	m_cursorBlinkTimer->start(CURSOR_BLINK_INTERVAL_MS);
}

KCanvas::~KCanvas()
{
}


void KCanvas::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.scale(m_zoomFactor, m_zoomFactor);

	// 1. Draw all shapes
	for (const auto& shape : m_shapes)
	{
		if (shape)
			shape->drawShape(&painter);
	}

	// 2. Draw selection controls for selected shapes
	if (!m_selectedShapes.empty())
	{
		for (const auto& shape : m_selectedShapes)
		{
			shape->drawSelectionControls(&painter);
			if (shape->getTextEdit())
			{
				if (m_cursorVisible)
					shape->drawCursor(&painter);
				if (m_isTextSelected)
					shape->drawTextSelected(&painter);
				if (shape->getPredText() != "")
					shape->drawPreText(&painter);
			}
		}
	}

	// 3. If multi-selecting, draw the selection rectangle
	if (m_state == State::Selecting)
	{
		// Use a temporary shape object to draw the selection rect
		KShape* tempRect = KShapeFactory::createShape(KGlobalData::KDrawFlag::RectDrawFlag).release();
		if (tempRect)
		{
			tempRect->setStartPoint(m_selectionRect.topLeft());
			tempRect->setEndPoint(m_selectionRect.bottomRight());
			tempRect->drawSelectingRect(&painter);
			delete tempRect;
		}
	}
}

void KCanvas::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
		return;

	// m_lastMousePos = event->pos();
	m_lastMousePos = event->pos() / m_zoomFactor;
	KGlobalData::KDrawFlag tool = KGlobalData::getGlobalDataIntance().getDrawFlag();

	if (tool == KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		KShape* shape = getAnSelected(m_lastMousePos);
		if (!shape)
		{
			deselectAll();
			shape = getShapeAt(m_lastMousePos);
		}
		if (shape)
		{
			// �ı�����
			if (shape->getShapeType() == KShapeType::TextShapeType)
			{
				if (event->type() == QEvent::MouseButtonDblClick)
				{
					deselectAll();
					shape->setTextEdit(true);
					m_editingText = shape;
					m_isTextSelected = true;
					shape->setCursorPos(shape->getText().length());
					shape->setLastCursorPos(0);
					setFocus(Qt::MouseFocusReason);
					m_cursorVisible = true;
					m_cursorBlinkTimer->start(CURSOR_BLINK_INTERVAL_MS);
				}
				else
				{
					shape->changeCursorPos(event->pos());
					if (m_isTextSelected)
						m_isTextSelected = false;
					shape->setLastCursorPos(shape->getCursorPos());
				}
			}

			m_currentHandle = shape->getHandleAt(m_lastMousePos);
			if (m_currentHandle != HandlePosition::None)
			{
				if (!shape->isSelected())
				{
					m_selectedShapes.push_back(shape);
					shape->setSelected(true);
					emit selectionChanged(this, m_selectedShapes);
				}
				m_state = State::Transforming;
				saveStateForUndo(m_selectedShapes, "Transform"); // Save state before transform
			}
		}
		else
		{ // Clicked on empty space
			deselectAll();
			m_state = State::Selecting;
			m_selectionRect.setTopLeft(m_lastMousePos);
			m_selectionRect.setBottomRight(m_lastMousePos);
		}
	}
	else
	{ // Drawing Tool (Rect, Line, etc.)
		deselectAll();
		m_state = State::Drawing;
		std::unique_ptr<KShape> newShape = KShapeFactory::createShape(tool);
		if (newShape)
		{
			m_drawingShape = newShape.get();
			m_drawingShape->setStartPoint(m_lastMousePos);
			m_drawingShape->setEndPoint(m_lastMousePos);
			m_shapes.push_back(std::move(newShape));
		}
	}
	update();
}

void KCanvas::mouseMoveEvent(QMouseEvent* event)
{
	// QPoint currentPos = event->pos();
	QPoint currentPos = event->pos() / m_zoomFactor;
	updateCursorStyle(currentPos); // Update cursor based on position

	if (event->buttons() != Qt::LeftButton) return;

	switch (m_state)
	{
	case State::Drawing:
		if (m_drawingShape)
		{
			if (m_drawingShape->getShapeType() == KShapeType::PenShapeType)
			{
				m_drawingShape->addPoint(currentPos);
			}
			else
			{
				m_drawingShape->setEndPoint(currentPos);
			}
		}
		break;
	case State::Selecting:
		m_selectionRect.setBottomRight(currentPos);
		break;
	case State::Transforming:
		if (m_editingText)
		{
			m_editingText->changeCursorPos(event->pos());
			m_isTextSelected = true;
		}
		else
		{
			handleTransformation(currentPos);
		}
		break;
	case State::Idle:
	default:
		break;
	}

	m_lastMousePos = currentPos;
	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton) return;

	switch (m_state)
	{
	case State::Drawing:
		if (m_drawingShape
			&& !m_drawingShape->isValid()
			&& !m_shapes.empty()
			&& m_drawingShape->getShapeType() != KShapeType::TextShapeType)
		{
			m_shapes.pop_back();
		}
		if (!m_shapes.empty() && m_shapes.back()->getShapeType() == KShapeType::TextShapeType)
		{
			deselectAll();
			KGlobalData::getGlobalDataIntance().setDrawFlag(KGlobalData::KDrawFlag::MouseDrawFlag);
			m_editingText = m_shapes.back().get();
			m_editingText->setTextEdit(true);
			m_isTextSelected = true;
			m_editingText->setCursorPos(m_editingText->getText().length());
			m_editingText->setLastCursorPos(0);
			setFocus(Qt::MouseFocusReason);
			m_cursorVisible = true;
			m_cursorBlinkTimer->start(CURSOR_BLINK_INTERVAL_MS);
			m_selectedShapes.push_back(m_editingText);
		}
		m_drawingShape = nullptr;
		break;
	case State::Selecting:
		m_selectionRect = m_selectionRect.normalized();
		for (const auto& shape : m_shapes)
		{
			if (m_selectionRect.intersects(shape->getShapeRect()))
			{
				m_selectedShapes.push_back(shape.get());
				shape->setSelected(true);
			}
		}
		emit selectionChanged(this, m_selectedShapes);
		break;
	case State::Transforming:
		// The undo command was already created on mouse press.
		// Now we finalize it with the 'after' state.
		// This part is simplified; the full implementation is in saveStateForUndo.
		saveState();
		break;
	case State::Idle:
	default:
		break;
	}

	m_state = State::Idle;
	update();
}

void KCanvas::keyPressEvent(QKeyEvent* event)
{
	if (m_editingText && m_editingText->isSelected())
		setFocus();
	// Text Edit
	if (m_editingText)
	{
		m_cursorVisible = true;
		m_cursorBlinkTimer->stop();
		m_cursorBlinkTimer->start(CURSOR_BLINK_INTERVAL_MS);
		if (event->key() == Qt::Key_Backspace)
		{
			if (m_editingText->getCursorPos() > 0)
			{
				if (m_isTextSelected)
				{
					int start = qMin(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
					int end = qMax(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
					m_editingText->setText(m_editingText->getText().remove(start, end - start));
					m_editingText->setCursorPos(start);
					m_isTextSelected = false;
				}
				else
				{
					m_editingText->setText(m_editingText->getText().remove(m_editingText->getCursorPos() - 1, 1));
					m_editingText->setCursorPos(m_editingText->getCursorPos() - 1);
				}
			}
		}
		else if (event->key() == Qt::Key_Delete)
		{
			if (m_isTextSelected)
			{
				int start = qMin(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				int end = qMax(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				m_editingText->setText(m_editingText->getText().remove(start, end - start));
				m_editingText->setCursorPos(start);
				m_isTextSelected = false;
			}
			else
			{
				if (m_editingText->getCursorPos() < m_editingText->getText().length())
					m_editingText->setText(m_editingText->getText().remove(m_editingText->getCursorPos(), 1));
			}
		}
		else if (event->key() == Qt::Key_Left)
		{
			if (m_isTextSelected)
			{
				int start = qMin(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				m_editingText->setCursorPos(start + 1);
				m_isTextSelected = false;
			}
			m_editingText->setCursorPos(qMax(0, m_editingText->getCursorPos() - 1));
		}
		else if (event->key() == Qt::Key_Right)
		{
			if (m_isTextSelected)
			{
				int end = qMax(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				m_editingText->setCursorPos(end - 1);
				m_isTextSelected = false;
			}
			m_editingText->setCursorPos(qMin(m_editingText->getText().length(), m_editingText->getCursorPos() + 1));
		}
		else if (event->key() == Qt::Key_Home)
		{
			m_editingText->setCursorPos(0);
		}
		else if (event->key() == Qt::Key_End)
		{
			m_editingText->setCursorPos(m_editingText->getText().length());
		}
		else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		{
			if (m_isTextSelected)
				m_isTextSelected = false;
			m_cursorBlinkTimer->stop();
			m_cursorVisible = false;
			clearFocus();
		}
		else if (event->text().length() > 0)
		{
			m_editingText->setText(m_editingText->getText().insert(m_editingText->getCursorPos(), event->text()));
			m_editingText->setCursorPos(m_editingText->getCursorPos() + 1);
		}
		update();
		return;
	}
	else
	{
		if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
		{
			deleteSelection();
			return;
		}
		if (event->matches(QKeySequence::Undo))
		{
			undo();
			return;
		}
		if (event->matches(QKeySequence::Copy))
		{
			copy();
			return;
		}
		if (event->matches(QKeySequence::Cut))
		{
			cut();
			return;
		}
		if (event->matches(QKeySequence::Paste))
		{
			paste();
			return;
		}
		if (event->matches(QKeySequence::SelectAll))
		{
			selectAll();
			return;
		}
	}
	QWidget::keyPressEvent(event);
}

void KCanvas::inputMethodEvent(QInputMethodEvent* event)
{
	if (m_editingText)
	{
		m_cursorVisible = true;
		m_cursorBlinkTimer->stop();
		m_cursorBlinkTimer->start(CURSOR_BLINK_INTERVAL_MS);

		// �����ύ�ı�
		if (!event->commitString().isEmpty() && m_editingText)
		{
			if (m_isTextSelected)
			{
				int start = qMin(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				int end = qMax(m_editingText->getCursorPos(), m_editingText->getLastCursorPos());
				m_editingText->setText(m_editingText->getText().remove(start, end - start));
				m_editingText->setCursorPos(start);
				m_isTextSelected = false;
			}
			m_editingText->setText(m_editingText->getText().insert(m_editingText->getCursorPos(), event->commitString()));
			m_editingText->setCursorPos(m_editingText->getCursorPos() + event->commitString().length());
			m_editingText->setPredText(""); // ���Ԥ�༭�ı�
		}

		// ����Ԥ�༭�ı�
		m_editingText->setPredText(event->preeditString());

		update(); // �����ػ�����ʾ�µ��ı���Ԥ�༭�ı�

		// �������뷨��������Ѿ�����������¼�
		event->accept();
	}
	else
	{
		// ����ı��򲻼�����������뷨�¼�
		event->ignore();
	}
}

// �����뷨����ṩ��Ϣ
QVariant KCanvas::inputMethodQuery(Qt::InputMethodQuery query) const
{
	if (!m_editingText)
		return QVariant(); // ����ı��򲻼�����ṩ��Ϣ

	switch (query)
	{
	case Qt::ImCursorRectangle:
	{
		// ���ع�����ڵľ�������
		int TEXT_PADDING = KGlobalData::getGlobalDataIntance().getTextPadding();
		QFontMetrics fm(this->font());
		int cursorX = m_editingText->getShapeRect().left() + TEXT_PADDING +
			fm.horizontalAdvance(m_editingText->getText().left(m_editingText->getCursorPos()));
		int cursorY = m_editingText->getShapeRect().top() + TEXT_PADDING;
		int cursorHeight = m_editingText->getShapeRect().height() - 2 * TEXT_PADDING;
		return QRect(cursorX, cursorY, 1, cursorHeight); // ���Ϊ1�Ĺ��
	}
	case Qt::ImFont:
		return this->font(); // ���ص�ǰʹ�õ�����
	case Qt::ImSurroundingText:
		// ���ع����Χ���ı����Ա����뷨�ṩ���õ�������
		return m_editingText->getText();
	case Qt::ImCursorPosition:
		// ���ع������Χ�ı��е�λ��
		return m_editingText->getCursorPos();
	case Qt::ImHints:
		// �����ṩ���뷨��ʾ�������Ƿ�Ϊ���롢�����ʼ���
		return QVariant::fromValue(Qt::ImhNone); // û��������ʾ
	default:
		break;
	}
	return QVariant();
}

// Text
void KCanvas::blinkCursor()
{
	m_cursorVisible = !m_cursorVisible;
	update();
}

void KCanvas::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu contextMenu(this);
	contextMenu.setStyleSheet(
		"QMenu {"
		"   background-color: #23272A;"
		"   color: #DCDDDE;"
		"   border: 1px solid #555;"
		"   padding: 5px;"
		"}"
		"QMenu::item {"
		"   padding: 5px 25px 5px 20px;"
		"}"
		"QMenu::item:selected {"
		"   background-color: #555;"
		"}"
		"QMenu::separator {"
		"   height: 1px;"
		"   background-color: #555;"
		"   margin-left: 10px;"
		"   margin-right: 10px;"
		"}"
	);

	// QPoint pos = event->pos();
	QPoint pos = event->pos() / m_zoomFactor;
	KShape* shapeUnderCursor = getShapeAt(pos);

	// ���ۺδ���������С�ȫѡ���͡�������
	QAction* selectAllAction = contextMenu.addAction(QStringLiteral("ȫѡ\tA"));
	QAction* undoAction = contextMenu.addAction(QStringLiteral("����(undo)\tZ"));
	connect(selectAllAction, &QAction::triggered, this, &KCanvas::selectAll);
	connect(undoAction, &QAction::triggered, this, &KCanvas::undo);
	undoAction->setEnabled(!m_history.empty()); // �����ʷ��¼Ϊ�գ�����ó���

	if (shapeUnderCursor)
	{
		// ����������ͼ�Σ�ѡ�������������δ��ѡ�еĻ���
		if (!shapeUnderCursor->isSelected())
		{
			deselectAll();
			shapeUnderCursor->setSelected(true);
			m_selectedShapes.push_back(shapeUnderCursor);
			update();
		}

		contextMenu.addSeparator();
		QAction* cutAction = contextMenu.addAction(QStringLiteral("����\tX"));
		QAction* copyAction = contextMenu.addAction(QStringLiteral("����\tC"));
		QAction* duplicateAction = contextMenu.addAction(QStringLiteral("��д\tD")); // ����д����Ӧ Duplicate
		QAction* deleteAction = contextMenu.addAction(QStringLiteral("ɾ��\tBackspace"));

		connect(cutAction, &QAction::triggered, this, &KCanvas::cut);
		connect(copyAction, &QAction::triggered, this, &KCanvas::copy);
		connect(duplicateAction, &QAction::triggered, this, &KCanvas::duplicate);
		connect(deleteAction, &QAction::triggered, this, &KCanvas::deleteSelection);

		contextMenu.addSeparator();
		QAction* bringForwardAction = contextMenu.addAction(QStringLiteral("����һ��\tUp"));
		QAction* sendBackwardAction = contextMenu.addAction(QStringLiteral("����һ��\tDown"));
		QAction* bringToFrontAction = contextMenu.addAction(QStringLiteral("����ǰ\tLeft"));
		QAction* sendToBackAction = contextMenu.addAction(QStringLiteral("�����\tRight"));

		connect(bringForwardAction, &QAction::triggered, this, &KCanvas::bringForward);
		connect(sendBackwardAction, &QAction::triggered, this, &KCanvas::sendBackward);
		connect(bringToFrontAction, &QAction::triggered, this, &KCanvas::bringToFront);
		connect(sendToBackAction, &QAction::triggered, this, &KCanvas::sendToBack);
	}

	contextMenu.exec(event->globalPos());
}

void KCanvas::paintToDevice(QPainter* painter)
{
	for (auto& shape : m_shapes)
	{
		shape->drawShape(painter);
	}
}

// --- Private Helper Methods ---

void KCanvas::deselectAll()
{
	for (auto& shape : m_selectedShapes)
	{
		shape->setSelected(false);
		if (m_editingText)
		{
			if (m_editingText->getShapeType() == KShapeType::TextShapeType && m_editingText->getText() == "")
			{
				for (auto it = m_shapes.begin(); it != m_shapes.end(); it++)
				{
					if (it->get() == m_editingText)
					{
						m_shapes.erase(it);
						break;
					}
				}
			}
			m_editingText = nullptr;
			shape->setTextEdit(false);
		}
	}
	m_selectedShapes.clear();
	emit selectionChanged(this, m_selectedShapes);
}

KShape* KCanvas::getShapeAt(const QPoint& pos)
{
	// Iterate backwards to find the top-most shape
	for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it)
	{
		// First check for a handle, as it has priority
		if ((*it)->isSelected() && (*it)->getHandleAt(pos) != HandlePosition::None)
		{
			return it->get();
		}
		// Then check if inside the shape
		if ((*it)->getHandleAt(pos) == HandlePosition::Inside)
		{
			return it->get();
		}
	}
	return nullptr;
}

KShape* KCanvas::getAnSelected(const QPoint& pos)
{
	for (auto& sshape : m_selectedShapes)
	{
		if (sshape->getHandleAt(pos) != HandlePosition::None)
		{
			return sshape;
		}
	}
	return nullptr;
}

void KCanvas::updateCursorStyle(const QPoint& pos)
{
	KGlobalData::KDrawFlag tool = KGlobalData::getGlobalDataIntance().getDrawFlag();
	if (tool != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		setCursor(Qt::CrossCursor);
		return;
	}

	HandlePosition handle = HandlePosition::None;
	KShape* shape = getShapeAt(pos);
	if (shape)
	{
		handle = shape->getHandleAt(pos);
	}

	switch (handle)
	{
	case HandlePosition::TopLeft:
	case HandlePosition::BottomRight:	setCursor(Qt::SizeFDiagCursor); break;
	case HandlePosition::TopRight:
	case HandlePosition::BottomLeft:	setCursor(Qt::SizeBDiagCursor); break;
	case HandlePosition::Top:
	case HandlePosition::Bottom:		setCursor(Qt::SizeVerCursor); break;
	case HandlePosition::Left:
	case HandlePosition::Right:			setCursor(Qt::SizeHorCursor); break;
	case HandlePosition::Inside:		setCursor(Qt::SizeAllCursor); break;
	case HandlePosition::Edit:			setCursor(Qt::IBeamCursor); break;
	default: unsetCursor(); break;
	}
}

void KCanvas::handleTransformation(const QPoint& pos)
{
	if (m_selectedShapes.empty())
		return;
	QPoint offset = pos - m_lastMousePos;

	for (auto& shape : m_selectedShapes)
	{
		switch (m_currentHandle)
		{
		case HandlePosition::Inside:      shape->move(offset); break;
		case HandlePosition::TopLeft:     shape->moveTopLeft(pos); break;
		case HandlePosition::Top:         shape->moveTop(pos); break;
		case HandlePosition::TopRight:    shape->moveTopRight(pos); break;
		case HandlePosition::Left:        shape->moveLeft(pos); break;
		case HandlePosition::Right:       shape->moveRight(pos); break;
		case HandlePosition::BottomLeft:  shape->moveBottomLeft(pos); break;
		case HandlePosition::Bottom:      shape->moveBottom(pos); break;
		case HandlePosition::BottomRight: shape->moveBottomRight(pos); break;
		default: break;
		}
	}
	
}

void KCanvas::deleteShapes(const std::vector<KShape*>& shapesToDelete)
{
	std::vector<std::unique_ptr<KShape>> newShapes;
	for (auto& shape : m_shapes)
	{
		bool shouldDelete = false;
		for (auto shapeToDel : shapesToDelete)
		{
			if (shape.get() == shapeToDel)
			{
				shouldDelete = true;
				break;
			}
		}
		if (!shouldDelete)
		{
			newShapes.push_back(std::move(shape));
		}
	}
	m_shapes = std::move(newShapes);
}

void KCanvas::saveStateForUndo(const std::vector<KShape*>& shapes, const QString& commandName)
{
	std::vector<std::unique_ptr<KShape>> beforeState;
	std::vector<std::unique_ptr<KShape>> afterState; // This would be captured on mouse release

	for (const auto& shape : shapes)
	{
		beforeState.push_back(shape->clone());
	}

	// On mouse release, you would capture the 'afterState' and create the command.
	// This is a placeholder for that logic.
	// auto cmd = std::make_unique<TransformCommand>(this, shapes, beforeState, afterState);
	// m_history.add(std::move(cmd));
}

void KCanvas::saveState()
{
	std::vector<std::unique_ptr<KShape>> statePoint;
	for (auto& shape : m_shapes)
	{
		statePoint.push_back(shape->clone());
	}
	m_history.push_back(std::move(statePoint));
}


// ----- command -----
void KCanvas::selectAll()
{
	deselectAll();
	for (const auto& shape : m_shapes)
	{
		shape->setSelected(true);
		m_selectedShapes.push_back(shape.get());
	}
	update();
}

void KCanvas::undo()
{
	if (m_history.size() > 0)
	{
		m_shapes = std::move(m_history.back());
		m_history.pop_back();
	}
	else
	{
		m_shapes.clear();
	}
	m_selectedShapes.clear();
	update();
}

void KCanvas::cut()
{
	saveState(); // ����״̬�Ա�����
	copy();
	deleteSelection();
}

void KCanvas::copy()
{
	m_clipboard.clear();
	for (const auto& shape : m_selectedShapes)
	{
		m_clipboard.push_back(shape->clone());
	}
}

void KCanvas::duplicate()
{
	if (m_selectedShapes.empty())
		return;
	saveState();
	copy();
	deselectAll();
	// ճ������΢ƫ��
	for (const auto& shape : m_clipboard)
	{
		auto newShape = shape->clone();
		qDebug() << newShape->getNumEdges();
		newShape->move(QPoint(10, 10)); // ƫ��һ�����
		newShape->setSelected(true);
		m_selectedShapes.push_back(newShape.get());
		m_shapes.push_back(std::move(newShape));
	}
	update();
}

void KCanvas::paste()
{
	if (m_clipboard.empty())
		return;
	saveState();
	deselectAll();
	// ճ������΢ƫ��
	for (const auto& shape : m_clipboard)
	{
		auto newShape = shape->clone();
		newShape->move(QPoint(10, 10)); // ƫ��һ�����
		newShape->setSelected(true);
		m_selectedShapes.push_back(newShape.get());
		m_shapes.push_back(std::move(newShape));
	}
	update();
}

void KCanvas::deleteSelection()
{
	if (m_selectedShapes.empty()) return;
	saveState();
	deleteShapes(m_selectedShapes);
	m_selectedShapes.clear();
	emit selectionChanged(this, m_selectedShapes);
	update();
}

// --- �㼶˳�������ʵ�� ---

void KCanvas::bringToFront()
{
	if (m_selectedShapes.empty()) return;
	saveState();
	std::vector<std::unique_ptr<KShape>> selected;
	m_shapes.erase(std::remove_if(m_shapes.begin(), m_shapes.end(),
		[&](std::unique_ptr<KShape>& shape)
		{
			if (shape->isSelected())
			{
				selected.push_back(std::move(shape));
				return true;
			}
			return false;
		}), m_shapes.end());

	for (auto& shape : selected)
	{
		m_shapes.push_back(std::move(shape));
	}
	update();
}

void KCanvas::sendToBack()
{
	if (m_selectedShapes.empty())
		return;
	saveState();
	std::vector<std::unique_ptr<KShape>> selected;
	m_shapes.erase(std::remove_if(m_shapes.begin(), m_shapes.end(),
		[&](std::unique_ptr<KShape>& shape)
		{
			if (shape->isSelected())
			{
				selected.push_back(std::move(shape));
				return true;
			}
			return false;
		}), m_shapes.end());

	m_shapes.insert(m_shapes.begin(), std::make_move_iterator(selected.begin()), std::make_move_iterator(selected.end()));
	update();
}

void KCanvas::bringForward()
{
	if (m_selectedShapes.size() != 1)
		return; // Ϊ�򻯣��˲�����֧�ֵ�ѡ
	saveState();
	auto it = std::find_if(m_shapes.begin(), m_shapes.end(),
		[&](const std::unique_ptr<KShape>& shape) { return shape.get() == m_selectedShapes[0]; });

	if (it != m_shapes.end() && std::next(it) != m_shapes.end())
	{
		auto next_it = std::next(it);
		std::iter_swap(it, next_it);
	}
	update();
}

void KCanvas::sendBackward()
{
	if (m_selectedShapes.size() != 1)
		return; // Ϊ�򻯣��˲�����֧�ֵ�ѡ
	saveState();
	auto it = std::find_if(m_shapes.begin(), m_shapes.end(),
		[&](const std::unique_ptr<KShape>& shape) { return shape.get() == m_selectedShapes[0]; });

	if (it != m_shapes.begin())
	{
		auto prev_it = std::prev(it);
		std::iter_swap(it, prev_it);
	}
	update();
}


void KCanvas::onSetBorderWidth(int width)
{
	if (m_selectedShapes.empty())
		return;
	saveState();
	for (auto& shape : m_selectedShapes)
	{
		shape->setBorderWidth(width);
	}
	update();
}

void KCanvas::onSetBorderStyle(int style)
{
	if (m_selectedShapes.empty())
		return;
	saveState();
	for (auto& shape : m_selectedShapes)
	{
		shape->setBorderStyle(static_cast<Qt::PenStyle>(style));
	}
	update();
}

void KCanvas::onSetBorderColor(const QColor& color)
{
	if (m_selectedShapes.empty()) return;
	saveState();
	for (auto& shape : m_selectedShapes)
	{
		shape->setBorderColor(color);
	}
	update();
}

void KCanvas::onSetFillColor(const QColor& color)
{
	if (m_selectedShapes.empty()) return;
	saveState();
	for (auto& shape : m_selectedShapes)
	{
		shape->setFillColor(color);
	}
	update();
}

void KCanvas::onSetCanvasColor(const QColor& color)
{
	m_color = color;
	setStyleSheet(QString("background-color: %1").arg(color.name()));
}

void KCanvas::onSetCanvasWidth(int width)
{
	if (m_baseSize.width() == width)
		return;
	m_baseSize.setWidth(width);
	setFixedSize(m_baseSize * m_zoomFactor);
	update();
}

void KCanvas::onSetCanvasHeight(int height)
{
	if (m_baseSize.height() == height)
		return;
	m_baseSize.setHeight(height);
	setFixedSize(m_baseSize * m_zoomFactor);
	update();
}

void KCanvas::setZoom(double factor)
{
	m_zoomFactor = factor;
	setFixedSize(m_baseSize * m_zoomFactor);
	update();
}

int KCanvas::getBaseWidth() const
{
	return m_baseSize.width();
}

int KCanvas::getBaseHeight() const
{
	return m_baseSize.height();
}

QColor KCanvas::getCanvasColor() const
{
	return m_color;
}

const std::vector<std::unique_ptr<KShape>>& KCanvas::getShapes() const
{
	return m_shapes;
}