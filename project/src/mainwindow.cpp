#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filemanager.h"
#include "kstylepanel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSvgWidget>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowTitle(QStringLiteral("SVG Editor"));
    QIcon windowIcon(":/image/icons8-ruler-64.png");
    this->setWindowIcon(windowIcon);
    this->setStyleSheet("QMainWindow { background-color: 36393F; }");
    if (statusBar())
        statusBar()->hide();

	m_canvasCounter = 0;
	QWidget* mainCentralWidget = new QWidget(this);
	QHBoxLayout* mainHorizontalLayout = new QHBoxLayout(mainCentralWidget);
	mainHorizontalLayout->setContentsMargins(0, 0, 0, 0);
	mainHorizontalLayout->setSpacing(0);

	m_toolbar = std::make_unique<KToolBar>(this);
	m_toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	mainHorizontalLayout->addWidget(m_toolbar.get());

	QWidget* rightContentContainer = new QWidget(this);
	QVBoxLayout* rightVerticalLayout = new QVBoxLayout(rightContentContainer);
	rightVerticalLayout->setContentsMargins(0, 0, 0, 0);
	rightVerticalLayout->setSpacing(0);

	m_topmenu = std::make_unique<KTopMenu>(this);
	m_topmenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	rightVerticalLayout->addWidget(m_topmenu.get());

	m_tabBar = std::make_unique<KTab>(this);
	rightVerticalLayout->addWidget(m_tabBar.get());

	m_canvasStack = std::make_unique<QStackedWidget>(this);

	QWidget* canvasCenterContainer = new QWidget(this);
	// 关键：让这个容器可以垂直和水平扩展，以填充所有可用空间
	canvasCenterContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QHBoxLayout* centerLayout = new QHBoxLayout(canvasCenterContainer);
	centerLayout->setContentsMargins(0, 0, 0, 0);
	//centerLayout->addStretch();
	centerLayout->addWidget(m_canvasStack.get());
	//centerLayout->addStretch();

	// 关键：将可扩展的画布容器添加到布局中，不再使用 addStretch()
	rightVerticalLayout->addWidget(canvasCenterContainer, 1); // 第二个参数 1 表示拉伸因子

	m_bottomBorder = std::make_unique<QWidget>(this);
	m_bottomBorder->setStyleSheet("background-color:#2C2F33");
	m_bottomBorder->setFixedHeight(40);
	rightVerticalLayout->addWidget(m_bottomBorder.get());

	mainHorizontalLayout->addWidget(rightContentContainer);

	m_stylePanel = std::make_unique<KStylePanel>(this);
	mainHorizontalLayout->addWidget(m_stylePanel.get());

	setCentralWidget(mainCentralWidget);

	connect(m_topmenu.get(), &KTopMenu::newCanvasTriggered, this, &MainWindow::addNewCanvas);
	connect(m_tabBar.get(), &KTab::newTabRequested, this, &MainWindow::addNewCanvas);
	connect(m_tabBar.get(), &KTab::tabCloseRequested, this, &MainWindow::closeCanvas);
	// connect(m_tabBar.get(), &KTab::currentChanged, m_canvasStack.get(), &QStackedWidget::setCurrentIndex);
	connect(m_canvasStack.get(), &QStackedWidget::currentChanged, this, &MainWindow::onCurrentCanvasChanged);
	connect(m_tabBar.get(), &KTab::currentChanged, m_canvasStack.get(), &QStackedWidget::setCurrentIndex);

	connect(m_topmenu.get(), &KTopMenu::exportTriggered, this, &MainWindow::onSaveAs);
	connect(m_topmenu.get(), &KTopMenu::importTriggered, this, &MainWindow::onImport);
	connect(m_topmenu.get(), &KTopMenu::zoomChanged, this, &MainWindow::onZoomChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

KCanvas* MainWindow::getCurrentCanvas() const
{
	// 从 StackedWidget 获取当前的 QScrollArea
	QScrollArea* scrollArea = qobject_cast<QScrollArea*>(m_canvasStack->currentWidget());
	if (scrollArea)
	{
		// 从 QScrollArea 获取其包含的 widget，并尝试转换为 KCanvas
		return qobject_cast<KCanvas*>(scrollArea->widget());
	}
	return nullptr; // 如果当前没有或不是 KCanvas，返回 nullptr
}

void MainWindow::addNewCanvas()
{
	m_canvasCounter++;
	QString tabName = QString(QStringLiteral("未命名画布%1")).arg(m_canvasCounter);

	KCanvas* newCanvas = new KCanvas(this);

	// 关键：将 KCanvas 放入 QScrollArea
	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(false); // 必须是 false，让滚动区域听从 KCanvas 的 setFixedSize
	scrollArea->setWidget(newCanvas);
	scrollArea->setAlignment(Qt::AlignCenter);

	// 将 QScrollArea 添加到堆栈中
	int canvasIndex = m_canvasStack->addWidget(scrollArea);

	m_tabBar->addTab(tabName);
	m_tabBar->setCurrentTab(canvasIndex);
}

void MainWindow::closeCanvas(int index)
{
	if (index < 0 || index >= m_canvasStack->count())
		return;

	QWidget* canvas = m_canvasStack->widget(index);
	m_canvasStack->removeWidget(canvas);
	canvas->deleteLater(); // Safely delete the widget
	
	m_tabBar->closeTab(index);
}

void MainWindow::onCurrentCanvasChanged(int index)
{
	if (index < 0)
	{
		if (m_stylePanel)
			m_stylePanel->onSelectionChanged(nullptr, {});
		return;
	}
	// 关键：使用新的辅助函数获取 KCanvas
	KCanvas* canvas = getCurrentCanvas();
	
	if (canvas && m_stylePanel)
	{
		// 断开旧的连接 (如果需要)
			
		// 连接 Canvas -> StylePanel
		connect(canvas, &KCanvas::selectionChanged, m_stylePanel.get(), &KStylePanel::onSelectionChanged);
		
		// 连接 StylePanel -> Canvas
		connect(m_stylePanel.get(), &KStylePanel::borderWidthChanged, canvas, &KCanvas::onSetBorderWidth);
		connect(m_stylePanel.get(), &KStylePanel::borderStyleChanged, canvas, &KCanvas::onSetBorderStyle);
		connect(m_stylePanel.get(), &KStylePanel::borderColorChanged, canvas, &KCanvas::onSetBorderColor);
		connect(m_stylePanel.get(), &KStylePanel::fillColorChanged, canvas, &KCanvas::onSetFillColor);
		connect(m_stylePanel.get(), &KStylePanel::canvasColorChanged, canvas, &KCanvas::onSetCanvasColor);
		connect(m_stylePanel.get(), &KStylePanel::canvasWidthChanged, canvas, &KCanvas::onSetCanvasWidth);
		connect(m_stylePanel.get(), &KStylePanel::canvasHeightChanged, canvas, &KCanvas::onSetCanvasHeight);

		// 手动触发一次更新
		m_stylePanel->onSelectionChanged(canvas, {}); // 初始为无选中状态
	}
	else if (m_stylePanel)
	{
		// 如果当前是 SVG 视图等非 KCanvas 控件，则清空属性面板
		m_stylePanel->onSelectionChanged(nullptr, {});
	}
}

void MainWindow::onSaveAs()
{
	KCanvas* currentCanvas = getCurrentCanvas();
	if (!currentCanvas)
	{
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("当前没有活动的画布可以导出。"));
		return;
	}

	FileManager::exportCanvasAsImage(this, currentCanvas);
}

void MainWindow::onImport()
{
	QString filePath = FileManager::importSvg(this);
	if (filePath.isEmpty())
		return;

	QWidget* svgContainer = new QWidget(this);
	QHBoxLayout * svgLayout = new QHBoxLayout(svgContainer);
	svgLayout->setContentsMargins(0, 0, 0, 0);
	svgLayout->addStretch();
	// 创建 QSvgWidget 控件并加载 SVG 文件
	QSvgWidget* svgWidget = new QSvgWidget(filePath, this);
	svgWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(true); // 对于 SVG，我们希望它能自适应调整大小
	scrollArea->setWidget(svgWidget);
	scrollArea->setAlignment(Qt::AlignCenter);

	int svgIndex = m_canvasStack->addWidget(scrollArea);

	QFileInfo fileInfo(filePath);
	QString tabName = fileInfo.fileName();
	m_tabBar->addTab(tabName);
	m_tabBar->setCurrentTab(svgIndex);

	if (m_stylePanel)
		m_stylePanel->onSelectionChanged(nullptr, {});
}

void MainWindow::onZoomChanged(int percentage)
{
	KCanvas* canvas = getCurrentCanvas();
	if (canvas)
	{
		canvas->setZoom(percentage / 100.0);
	}
}