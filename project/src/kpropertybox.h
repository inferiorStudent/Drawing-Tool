#ifndef __K_PROPERTY_BOX_H__
#define __K_PROPERTY_BOX_H__

#include <QWidget>

class QLabel;

class KPropertyBox : public QWidget
{
	Q_OBJECT
public:
	explicit KPropertyBox(const QString& title, QWidget* contentWidget, QWidget* parent = nullptr);
};

#endif // __K_PROPERTY_BOX_H__