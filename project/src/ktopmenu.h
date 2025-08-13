#ifndef __K_TOPMENU_H__
#define __K_TOPMENU_H__

#include <QWidget>
#include <QPushButton>
#include <QMenu>

class KTopMenu : public QWidget
{
	Q_OBJECT
public:
	explicit KTopMenu(QWidget* parent = Q_NULLPTR);

signals:
	void exportTriggered();
	void importTriggered();
	void newCanvasTriggered();
	void zoomChanged(int percentage);	// Ëõ·ÅÐÅºÅ

private slots:
	void showFileMenu();
	void showViewMenu();

private:
	QPushButton* fileButton;
	QMenu* fileMenu;
	QPushButton* viewButton;
	QMenu * viewMenu;
};


#endif //__K_TOPMENU_H__