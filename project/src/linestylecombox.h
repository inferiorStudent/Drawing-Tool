#include <QComboBox>
#include <QPainter>
#include <QPen>

class LineStyleComboBox : public QComboBox
{
public:
	explicit LineStyleComboBox(QWidget* parent = nullptr) : QComboBox(parent) {}

protected:
	void paintEvent(QPaintEvent* event) override
	{
		QStyleOptionComboBox opt;
		initStyleOption(&opt);

		// �����ø�����Ƴ��ı�֮������ж��� (�߿򡢼�ͷ��)
		QPainter painter(this);
		style()->drawComplexControl(QStyle::CC_ComboBox, &opt, &painter, this);

		// ��ȡ��ǰѡ�������������
		QVariant data = itemData(currentIndex(), Qt::UserRole);
		if (data.isValid() && data.canConvert<int>())
		{
			Qt::PenStyle lineStyle = static_cast<Qt::PenStyle>(data.toInt());

			// ��������ͼ��
			painter.save();
			painter.setRenderHint(QPainter::Antialiasing);

			QPen pen;
			pen.setStyle(lineStyle);
			pen.setColor(palette().color(QPalette::Text));
			pen.setWidth(2);
			painter.setPen(pen);

			// �����������ͨ������������
			qreal margin = 5.0;
			QRect rect = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);

			QPointF startPoint(rect.x() + margin, rect.center().y());
			QPointF endPoint(rect.x() + rect.width() - margin, rect.center().y());

			painter.drawLine(startPoint, endPoint);

			painter.restore();
		}
	}
};