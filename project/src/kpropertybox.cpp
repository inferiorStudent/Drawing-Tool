#include "kpropertybox.h"
#include <QVBoxLayout>
#include <QLabel>

KPropertyBox::KPropertyBox(const QString& title, QWidget* contentWidget, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground, true);
	// ����ÿ�����Ժе���ʽ������ɫ���߿�Բ�ǵ�
	this->setStyleSheet(
		"KPropertyBox {"
		"   background-color: #555;"
		"   border-radius: 4px;"
		"}"
		"QLabel {"
		"   color: #BBB;"      // ��ǩ������ɫ�԰�
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

	// ȷ�����ݿؼ���ʽ��ȷ
	contentWidget->setStyleSheet(
		"QSpinBox, QComboBox { background-color: #3C3C3C; border: 1px solid #666; padding: 2px; }"
		"QPushButton { border: 1px solid #666; }"
	);
}