#include "kstylepanel.h"
#include "kcanvas.h"
#include "kshape.h"
#include "kpropertybox.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>

KStylePanel::KStylePanel(QWidget* parent)
	: QWidget(parent)
{
	setFixedWidth(200);
	setObjectName("KStylePanel");
	setStyleSheet("background-color: #2C2F33; color: white;");
	setAttribute(Qt::WA_StyledBackground, true);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setAlignment(Qt::AlignTop);

	m_mainContentWidget = new QWidget(this); // ���������ݿؼ�
	mainLayout->addWidget(m_mainContentWidget);
	QVBoxLayout * contentLayout = new QVBoxLayout(m_mainContentWidget);
	contentLayout->setContentsMargins(0, 0, 0, 0);

	// --- ���� Group ---
	QGroupBox* canvasGroup = new QGroupBox(QStringLiteral("��������"), this);
	QGridLayout* canvasLayout = new QGridLayout(canvasGroup);

	m_canvasWidthSpinBox = new QSpinBox(this);
	m_canvasWidthSpinBox->setRange(100, 5000);
	m_canvasHeightSpinBox = new QSpinBox(this);
	m_canvasHeightSpinBox->setRange(100, 5000);
	m_canvasColorButton = new QPushButton("", this);

	canvasLayout->addWidget(new KPropertyBox(QStringLiteral("�������"), m_canvasWidthSpinBox, this), 0, 0);
	canvasLayout->addWidget(new KPropertyBox(QStringLiteral("�����߶�"), m_canvasHeightSpinBox, this), 0, 1);
	canvasLayout->addWidget(new KPropertyBox(QStringLiteral("������ɫ"), m_canvasColorButton, this), 1, 1);

	// --- ��״ Group (��ʼ����) ---
	QGroupBox* shapeGroup = new QGroupBox(QStringLiteral("ͼ������"), this);
	QGridLayout* shapeLayout = new QGridLayout(shapeGroup);

	m_borderWidthSpinBox = new QSpinBox(this);
	m_borderWidthSpinBox->setRange(1, 100);
	m_borderColorButton = new QPushButton("", this);
	m_fillColorButton = new QPushButton("", this);
	m_borderStyleComboBox = new LineStyleComboBox(this);

	m_borderStyleComboBox->setItemDelegate(new LineStyleDelegate(m_borderStyleComboBox));

	m_borderStyleComboBox->addItem("", QVariant::fromValue(static_cast<int>(Qt::SolidLine)));
	m_borderStyleComboBox->addItem("", QVariant::fromValue(static_cast<int>(Qt::DashLine)));
	m_borderStyleComboBox->addItem("", QVariant::fromValue(static_cast<int>(Qt::DotLine)));
	m_borderStyleComboBox->addItem("", QVariant::fromValue(static_cast<int>(Qt::DashDotLine)));

	shapeLayout->addWidget(new KPropertyBox(QStringLiteral("�߿���"), m_borderWidthSpinBox, this), 0, 0);
	shapeLayout->addWidget(new KPropertyBox(QStringLiteral("�߿���ʽ"), m_borderStyleComboBox, this), 0, 1);
	shapeLayout->addWidget(new KPropertyBox(QStringLiteral("�߿���ɫ"), m_borderColorButton, this), 1, 0);
	shapeLayout->addWidget(new KPropertyBox(QStringLiteral("�����ɫ"), m_fillColorButton, this), 1, 1);
	m_shapePropertiesWidget = shapeGroup; // �� GroupBox ��Ϊ����

	contentLayout->addWidget(canvasGroup);
	contentLayout->addWidget(m_shapePropertiesWidget);
	m_mainContentWidget->setVisible(false); // ��ʼ״̬��������������

	
	connect(m_canvasWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &KStylePanel::canvasWidthChanged);
	connect(m_canvasHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &KStylePanel::canvasHeightChanged);
	connect(m_canvasColorButton, &QPushButton::clicked, this, &KStylePanel::openCanvasColorDialog);

	connect(m_borderWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &KStylePanel::borderWidthChanged);
	connect(m_borderColorButton, &QPushButton::clicked, this, &KStylePanel::openBorderColorDialog);
	connect(m_fillColorButton, &QPushButton::clicked, this, &KStylePanel::openFillColorDialog);
	connect(m_borderStyleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KStylePanel::handleBorderStyleChange);
}

void KStylePanel::handleBorderStyleChange(int index)
{
	if (m_isUpdatingFromSignal)
		return;
	emit borderStyleChanged(m_borderStyleComboBox->itemData(index).toInt());
}

void KStylePanel::onSelectionChanged(KCanvas* activeCanvas, const std::vector<KShape*>& selectedShapes)
{
	m_isUpdatingFromSignal = true; // ������ UI ʱ��ֹ�����ź�

	// ���û�л�Ļ�������������������
	if (!activeCanvas)
	{
		m_mainContentWidget->setVisible(false);
		return;
	}
	m_mainContentWidget->setVisible(true);

	if (selectedShapes.empty())
	{
		updateDisplay(activeCanvas, nullptr);
	}
	else
	{
		// ֻ��ʾ��һ��ѡ��ͼ�ε�����
		updateDisplay(activeCanvas, selectedShapes[0]);
	}

	m_isUpdatingFromSignal = false;
}

void KStylePanel::updateDisplay(KCanvas* canvas, KShape* shape)
{
	if (!canvas)
		return;
	m_canvasWidthSpinBox->setValue(canvas->getBaseWidth());
	m_canvasHeightSpinBox->setValue(canvas->getBaseHeight());
	// ���»�����ɫ��ť�ı���ɫ
	m_canvasColorButton->setStyleSheet(QString("background-color: %1").arg(canvas->palette().window().color().name()));

	if (shape && shape->getShapeType() != KShapeType::TextShapeType)
	{
		m_shapePropertiesWidget->setVisible(true);
		m_borderWidthSpinBox->setValue(shape->getBorderWidth());
		m_borderColorButton->setStyleSheet(QString("background-color: %1").arg(shape->getBorderColor().name()));
		m_fillColorButton->setStyleSheet(QString("background-color: %1").arg(shape->getFillColor().name()));

		int index = m_borderStyleComboBox->findData(QVariant::fromValue(static_cast<int>(shape->getBorderStyle())));
		m_borderStyleComboBox->setCurrentIndex(index);
	}
	else
	{
		m_shapePropertiesWidget->setVisible(false);
	}
}

void KStylePanel::openBorderColorDialog()
{
	QColor color = QColorDialog::getColor(Qt::black, this, QStringLiteral("ѡ��߿���ɫ"));
	if (color.isValid())
	{
		m_borderColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
		if (!m_isUpdatingFromSignal) emit borderColorChanged(color);
	}
}
void KStylePanel::openFillColorDialog()
{
	QColor color = QColorDialog::getColor(Qt::white, this, QStringLiteral("ѡ�������ɫ"));
	if (color.isValid())
	{
		m_fillColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
		if (!m_isUpdatingFromSignal) emit fillColorChanged(color);
	}
}
void KStylePanel::openCanvasColorDialog()
{
	QColor color = QColorDialog::getColor(Qt::white, this, QStringLiteral("ѡ�񻭲���ɫ"));
	if (color.isValid())
	{
		m_canvasColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
		if (!m_isUpdatingFromSignal) emit canvasColorChanged(color);
	}
}