#include "kpropertybox.h"
#include <QVBoxLayout>
#include <QLabel>

KPropertyBox::KPropertyBox(const QString& title, QWidget* contentWidget, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground, true);
	// 设置每个属性盒的样式：背景色、边框、圆角等
	this->setStyleSheet(
		"KPropertyBox {"
		"   background-color: #555;"
		"   border-radius: 4px;"
		"}"
		"QLabel {"
		"   color: #BBB;"      // 标签文字颜色稍暗
		"	background-color: #555;"
		"   font-size: 15px;"
		"}"
	);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(5, 5, 5, 5);
	layout->setSpacing(4);

	QLabel* titleLabel = new QLabel(title, this);
	titleLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(titleLabel);
	layout->addWidget(contentWidget);

	// 确保内容控件样式正确
	contentWidget->setStyleSheet(
		"QSpinBox, QComboBox { background-color: #3C3C3C; border: 1px solid #666; padding: 2px; }"
		"QPushButton { border: 1px solid #666; }"
	);
}