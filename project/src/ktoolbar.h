#ifndef __K_TOOLBAR_H__
#define __K_TOOLBAR_H__

#include "ktoolbutton.h"
#include "kpolygonpanel.h"
#include <QVBoxLayout>

class KToolBar : public QWidget
{
    Q_OBJECT

public:
    KToolBar(QWidget* parent);
    ~KToolBar();

public slots:
    void resetButtonState();

private slots:
	void onPolygonButtonClicked();
	void onPolygonSelected(KGlobalData::KDrawFlag flag, const QString & iconName);

private:
    QVBoxLayout* m_pVLayout;            // ��ֱ����
    QButtonGroup* m_pBtnGroup;          //��ť��
    KToolButton* m_pMouseBtn;
    KToolButton* m_pPenBtn;
    KToolButton* m_pLineBtn;
    KToolButton* m_pRectBtn;
    KToolButton* m_pCircleBtn;
    KToolButton* m_pTextBtn;

	KToolButton* m_pPolygonBtn;         // ����ΰ�ť
	KPolygonPanel * m_pPolygonPanel;    // �������
};

#endif //__K_TOLLBAR_H__