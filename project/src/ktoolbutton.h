#ifndef __KTOOLBUTTON_H_
#define __KTOOLBUTTON_H_
#include <QRadioButton>
#include <QSvgRenderer>
#include <QPainter>
#include "kglobaldata.h"

class KToolButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit KToolButton(KGlobalData::KDrawFlag drawflag, QWidget* parent = Q_NULLPTR);
    ~KToolButton();
    void initToolButton();
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void enterEvent(QEvent* event) override; // 进入组件区域
    virtual void leaveEvent(QEvent* event) override; // 离开组件区域
    virtual void mouseReleaseEvent(QMouseEvent* e) override;

    void resetStatus();
    void setIcon(const QString& iconName);
    void setDrawFlag(const KGlobalData::KDrawFlag& flag);

private:
    void onToggledButton(bool checked);
private:
    QString m_iconName;//svg 图标名称
    QSvgRenderer* m_pSvgRenderer;//处理 svg 图标/图片的对象指针
    KGlobalData::KDrawFlag m_drawFlag;//记录绘图类型
};
#endif