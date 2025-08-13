#ifndef __K_GLOBAL_DATA__
#define __K_GLOBAL_DATA__
#include <QObject>
class KGlobalData : public QObject
{
    Q_OBJECT
public:
    enum class KDrawFlag
    {
        NoneDrawFlag = 0,
        MouseDrawFlag,
        PenDrawFlag,
        LineDrawFlag,
        RectDrawFlag,
        CircleDrawFlag,
        TextDrawFlag,
		// 多边形
		TriangleDrawFlag, // 三角形
		PentagonDrawFlag, // 五边形
		HexagonDrawFlag   // 六边形
    };
    explicit KGlobalData(QObject* parent = Q_NULLPTR);
    ~KGlobalData();
    static KGlobalData& getGlobalDataIntance();
    void setDrawFlag(KGlobalData::KDrawFlag drawflag);
    KDrawFlag getDrawFlag()const;
    void setCanvasWidth(const int width);
    void setCanvasHeight(const int height);
    int getCanvasWidth() const;
    int getCanvasHeight() const;
    void setCanvasColor(const QString& colorStr);
    QString getCanvasColor() const;

    int getTextPadding() const;
private:
    KGlobalData(const KGlobalData& other) = delete;
    KGlobalData(const KGlobalData&& other) = delete;
    void operator=(const KGlobalData) = delete;
private:
    KDrawFlag m_drawFlag;
    int m_canvasWidth;
    int m_canvasHeight;
    QString m_canvasColor;
    int m_textPadding;
};
#endif