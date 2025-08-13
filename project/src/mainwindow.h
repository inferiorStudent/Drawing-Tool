#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ktab.h"
#include "ktoolbar.h"
#include "ktopmenu.h"
#include "kcanvas.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	KCanvas* getCurrentCanvas() const;

private slots:
	void addNewCanvas();
	void closeCanvas(int index);
	void onCurrentCanvasChanged(int index);
	void onSaveAs();
	void onImport();
	void onZoomChanged(int percentage);

private:
    Ui::MainWindow *ui;
	std::unique_ptr<KTab> m_tabBar;
	std::unique_ptr<KToolBar> m_toolbar;
	std::unique_ptr<KTopMenu> m_topmenu;
	std::unique_ptr<class KStylePanel> m_stylePanel;
	std::unique_ptr<QStackedWidget> m_canvasStack;
	std::unique_ptr<QWidget> m_bottomBorder;
	int m_canvasCounter;
};
#endif // MAINWINDOW_H