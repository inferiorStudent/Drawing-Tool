#include <QStyledItemDelegate>
#include <QPainter>
#include <QComboBox>

class LineStyleDelegate : public QStyledItemDelegate
{
public:
	explicit LineStyleDelegate(QObject* parent = nullptr)
		: QStyledItemDelegate(parent)
	{
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		// 调用默认绘制来处理背景、选择状态等，但稍后会覆盖文本部分
		QStyleOptionViewItem opt = option;
		// 清除文本和图标，这样QStyledItemDelegate::paint就不会画它们了
		opt.text.clear();
		opt.icon = QIcon();
		QStyledItemDelegate::paint(painter, opt, index);

		// 获取线型枚举值 (假设将Qt::PenStyle存储在UserRole中)
		QVariant data = index.data(Qt::UserRole);
		if (data.isValid() && data.canConvert<int>())
		{
			Qt::PenStyle lineStyle = static_cast<Qt::PenStyle>(data.toInt());

			// 绘制线型图案
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);

			QPen pen;
			pen.setStyle(lineStyle);
			pen.setColor(option.palette.color(QPalette::Text)); // 使用文本颜色
			pen.setWidth(2);
			painter->setPen(pen);

			// 绘制一条与项目同宽的水平线，上下各留一些边距
			qreal margin = 5.0;
			QPointF startPoint(option.rect.x() + margin, option.rect.center().y());
			QPointF endPoint(option.rect.x() + option.rect.width() - margin, option.rect.center().y());

			painter->drawLine(startPoint, endPoint);

			painter->restore();
		}
	}
};