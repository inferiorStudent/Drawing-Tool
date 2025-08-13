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

		// 首先让父类绘制除文本之外的所有东西 (边框、箭头等)
		QPainter painter(this);
		style()->drawComplexControl(QStyle::CC_ComboBox, &opt, &painter, this);

		// 获取当前选中项的线型数据
		QVariant data = itemData(currentIndex(), Qt::UserRole);
		if (data.isValid() && data.canConvert<int>())
		{
			Qt::PenStyle lineStyle = static_cast<Qt::PenStyle>(data.toInt());

			// 绘制线型图案
			painter.save();
			painter.setRenderHint(QPainter::Antialiasing);

			QPen pen;
			pen.setStyle(lineStyle);
			pen.setColor(palette().color(QPalette::Text));
			pen.setWidth(2);
			painter.setPen(pen);

			// 定义绘制区域，通常在左侧或中央
			qreal margin = 5.0;
			QRect rect = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);

			QPointF startPoint(rect.x() + margin, rect.center().y());
			QPointF endPoint(rect.x() + rect.width() - margin, rect.center().y());

			painter.drawLine(startPoint, endPoint);

			painter.restore();
		}
	}
};