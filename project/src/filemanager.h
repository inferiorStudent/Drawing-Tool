#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "kcanvas.h"
#include <QWidget>

class FileManager
{
public:
    /**
    * @brief 导出 QWidget (KCanvas) 的内容为图片文件.
    * @param parent 父窗口指针，用于QFileDialog的定位.
    * @param canvas 需要导出内容的 KCanvas 指针.
    * @return 成功返回 true, 失败或用户取消返回 false.
    */
    static bool exportCanvasAsImage(QWidget* parent, KCanvas* canvas);

	/**
    * @brief 导出 KCanvas 的内容为 SVG 矢量图文件.
    * @param parent 父窗口指针.
    * @param canvas 需要导出内容的 KCanvas 指针.
    * @return 成功返回 true, 失败或用户取消返回 false.
    */
	static bool exportCanvasAsSvg(QWidget * parent, KCanvas * canvas);
	
	/**
	* @brief 弹出文件对话框让用户选择一个 SVG 文件进行导入.
	* @param parent 父窗口指针.
	* @return 用户选择的 SVG 文件路径，如果取消则返回空字符串.
	*/
	static QString importSvg(QWidget * parent);
};

#endif // __FILE_MANAGER_H__