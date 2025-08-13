#include "filemanager.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSvgGenerator> // ���ڵ��� SVG
#include <QPainter>

bool FileManager::exportCanvasAsImage(QWidget* parent, KCanvas* canvas)
{
	if (!canvas)
	{
		QMessageBox::warning(parent, QStringLiteral("��������"), QStringLiteral("û�пɹ������Ļ�����"));
		return false;
	}

	QString filePath = QFileDialog::getSaveFileName(
		parent,
		QStringLiteral("���Ϊ"),
		QStringLiteral("δ����.svg"), // Ĭ���ļ���
		QStringLiteral("SVG ʸ��ͼ (*.svg);;PNG ͼƬ (*.png);;JPEG ͼƬ (*.jpg *.jpeg)")
	);

	// ����û�ȡ���˶Ի���filePath��Ϊ��
	if (filePath.isEmpty())
	{
		return false;
	}
	if (filePath.endsWith("svg"))
	{
		// 1. ���� SVG ������
		QSvgGenerator generator;
		generator.setFileName(filePath);
		generator.setSize(canvas->size());
		generator.setViewBox(canvas->rect()); // �����ӿ�
		generator.setTitle(QStringLiteral("���� KDraw ����Ʒ"));
		generator.setDescription(QStringLiteral("�� KDraw ��ͼ�������ɡ�"));

		QPainter painter;
		if (!painter.begin(&generator))
		{
			QMessageBox::critical(parent, QStringLiteral("����ʧ��"), QStringLiteral("�޷����� SVG ��������"));
			return false;
		}

		// 2. ���������ݻ��Ƶ� SVG ��������
		canvas->paintToDevice(&painter);
		

		QMessageBox::information(
			parent,
			QStringLiteral("�����ɹ�"),
			QStringLiteral("�����ѳɹ���������\n%1").arg(filePath));
		return true;
	}
	else
	{
		// ����һ���� KCanvas �ȴ�� QPixmap ������Ϊ����Ŀ��
		QPixmap pixmap(canvas->size());
		pixmap.fill(canvas->getCanvasColor());

		// ʹ�� QWidget::render() ������ KCanvas �����ݻ��Ƶ� QPixmap ��
		// ������ɿ��Ľ�ͼ��ʽ���ܲ�׽�������ӿؼ��ͻ��Ƶ�ͼ��
		canvas->render(&pixmap);

		// ���� QPixmap ���û�ָ�����ļ�·��
		// QPixmap::save() ������ļ���׺���Զ�ѡ����ȷ��ͼƬ��ʽ
		if (pixmap.save(filePath))
		{
			QMessageBox::information(parent,
				QStringLiteral("�����ɹ�"),
				QStringLiteral("�����ѳɹ���������\n%1").arg(filePath));
			return true;
		}
		else
		{
			QMessageBox::critical(parent,
				QStringLiteral("����ʧ��"),
				QStringLiteral("�޷�����ͼƬ������·����Ȩ�ޡ�"));
			return false;
		}
	}
	return false;
}

QString FileManager::importSvg(QWidget* parent)
{
	return QFileDialog::getOpenFileName(parent, QStringLiteral("���� SVG"), "",
		QStringLiteral("SVG ʸ��ͼ (*.svg)"));
}