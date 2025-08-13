#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "kcanvas.h"
#include <QWidget>

class FileManager
{
public:
    /**
    * @brief ���� QWidget (KCanvas) ������ΪͼƬ�ļ�.
    * @param parent ������ָ�룬����QFileDialog�Ķ�λ.
    * @param canvas ��Ҫ�������ݵ� KCanvas ָ��.
    * @return �ɹ����� true, ʧ�ܻ��û�ȡ������ false.
    */
    static bool exportCanvasAsImage(QWidget* parent, KCanvas* canvas);

	/**
    * @brief ���� KCanvas ������Ϊ SVG ʸ��ͼ�ļ�.
    * @param parent ������ָ��.
    * @param canvas ��Ҫ�������ݵ� KCanvas ָ��.
    * @return �ɹ����� true, ʧ�ܻ��û�ȡ������ false.
    */
	static bool exportCanvasAsSvg(QWidget * parent, KCanvas * canvas);
	
	/**
	* @brief �����ļ��Ի������û�ѡ��һ�� SVG �ļ����е���.
	* @param parent ������ָ��.
	* @return �û�ѡ��� SVG �ļ�·�������ȡ���򷵻ؿ��ַ���.
	*/
	static QString importSvg(QWidget * parent);
};

#endif // __FILE_MANAGER_H__