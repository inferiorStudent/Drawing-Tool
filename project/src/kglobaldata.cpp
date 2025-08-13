#include "kglobaldata.h"
#include <Windows.h>

KGlobalData::KGlobalData(QObject* parent)
    : QObject(parent)
    , m_drawFlag(KDrawFlag::NoneDrawFlag)
    , m_canvasColor("FFFFFF")
    , m_canvasWidth(600)
    , m_canvasHeight(400)
    , m_textPadding(3)
{
}

KGlobalData::~KGlobalData()
{
}

KGlobalData& KGlobalData::getGlobalDataIntance()
{
    static KGlobalData s_instance;
    return s_instance;
}

void KGlobalData::setDrawFlag(KGlobalData::KDrawFlag drawflag)
{
    this->m_drawFlag = drawflag;
}

KGlobalData::KDrawFlag KGlobalData::getDrawFlag() const
{
    return m_drawFlag;
}

void KGlobalData::setCanvasWidth(const int width)
{
    this->m_canvasWidth = width;
}

void KGlobalData::setCanvasHeight(const int height)
{
    this->m_canvasHeight = height;
}

int KGlobalData::getCanvasWidth() const
{
    return this->m_canvasWidth;
}

int KGlobalData::getCanvasHeight() const
{
    return m_canvasHeight;
}

void KGlobalData::setCanvasColor(const QString& colorStr)
{
    this->m_canvasColor = colorStr;
}
QString KGlobalData::getCanvasColor() const
{
    return m_canvasColor;
}

int KGlobalData::getTextPadding() const
{
    return m_textPadding;
}