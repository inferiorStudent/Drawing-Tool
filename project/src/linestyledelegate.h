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
		// ����Ĭ�ϻ�������������ѡ��״̬�ȣ����Ժ�Ḳ���ı�����
		QStyleOptionViewItem opt = option;
		// ����ı���ͼ�꣬����QStyledItemDelegate::paint�Ͳ��ử������
		opt.text.clear();
		opt.icon = QIcon();
		QStyledItemDelegate::paint(painter, opt, index);

		// ��ȡ����ö��ֵ (���轫Qt::PenStyle�洢��UserRole��)
		QVariant data = index.data(Qt::UserRole);
		if (data.isValid() && data.canConvert<int>())
		{
			Qt::PenStyle lineStyle = static_cast<Qt::PenStyle>(data.toInt());

			// ��������ͼ��
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);

			QPen pen;
			pen.setStyle(lineStyle);
			pen.setColor(option.palette.color(QPalette::Text)); // ʹ���ı���ɫ
			pen.setWidth(2);
			painter->setPen(pen);

			// ����һ������Ŀͬ���ˮƽ�ߣ����¸���һЩ�߾�
			qreal margin = 5.0;
			QPointF startPoint(option.rect.x() + margin, option.rect.center().y());
			QPointF endPoint(option.rect.x() + option.rect.width() - margin, option.rect.center().y());

			painter->drawLine(startPoint, endPoint);

			painter->restore();
		}
	}
};