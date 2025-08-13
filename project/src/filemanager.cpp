#include "filemanager.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSvgGenerator> // 用于导出 SVG
#include <QPainter>

bool FileManager::exportCanvasAsImage(QWidget* parent, KCanvas* canvas)
{
	if (!canvas)
	{
		QMessageBox::warning(parent, QStringLiteral("导出错误"), QStringLiteral("没有可供导出的画布。"));
		return false;
	}

	QString filePath = QFileDialog::getSaveFileName(
		parent,
		QStringLiteral("另存为"),
		QStringLiteral("未命名.svg"), // 默认文件名
		QStringLiteral("SVG 矢量图 (*.svg);;PNG 图片 (*.png);;JPEG 图片 (*.jpg *.jpeg)")
	);

	// 如果用户取消了对话框，filePath会为空
	if (filePath.isEmpty())
	{
		return false;
	}
	if (filePath.endsWith("svg"))
	{
		// 1. 创建 SVG 生成器
		QSvgGenerator generator;
		generator.setFileName(filePath);
		generator.setSize(canvas->size());
		generator.setViewBox(canvas->rect()); // 设置视口
		generator.setTitle(QStringLiteral("来自 KDraw 的作品"));
		generator.setDescription(QStringLiteral("由 KDraw 绘图程序生成。"));

		QPainter painter;
		if (!painter.begin(&generator))
		{
			QMessageBox::critical(parent, QStringLiteral("导出失败"), QStringLiteral("无法创建 SVG 生成器。"));
			return false;
		}

		// 2. 将画布内容绘制到 SVG 生成器上
		canvas->paintToDevice(&painter);
		

		QMessageBox::information(
			parent,
			QStringLiteral("导出成功"),
			QStringLiteral("画布已成功导出到：\n%1").arg(filePath));
		return true;
	}
	else
	{
		// 创建一个和 KCanvas 等大的 QPixmap 对象作为绘制目标
		QPixmap pixmap(canvas->size());
		pixmap.fill(canvas->getCanvasColor());

		// 使用 QWidget::render() 函数将 KCanvas 的内容绘制到 QPixmap 上
		// 这是最可靠的截图方式，能捕捉到所有子控件和绘制的图形
		canvas->render(&pixmap);

		// 保存 QPixmap 到用户指定的文件路径
		// QPixmap::save() 会根据文件后缀名自动选择正确的图片格式
		if (pixmap.save(filePath))
		{
			QMessageBox::information(parent,
				QStringLiteral("导出成功"),
				QStringLiteral("画布已成功导出到：\n%1").arg(filePath));
			return true;
		}
		else
		{
			QMessageBox::critical(parent,
				QStringLiteral("导出失败"),
				QStringLiteral("无法保存图片，请检查路径或权限。"));
			return false;
		}
	}
	return false;
}

QString FileManager::importSvg(QWidget* parent)
{
	return QFileDialog::getOpenFileName(parent, QStringLiteral("导入 SVG"), "",
		QStringLiteral("SVG 矢量图 (*.svg)"));
}