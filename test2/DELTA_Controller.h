
// test2.h : main header file for the PROJECT_NAME application
//

#pragma once	//һ���ļ��ж�����ø�ͷ�ļ�ʱ�ú�ָʾ������ֻ����һ��

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif	//�ڰ������ļ�֮ǰ�Ȱ�����stdafx.h������PCH�ļ�

#include "resource.h"		// main symbols��������ԴID


// CDeltaControllerApp:
// See test2.cpp for the implementation of this class
//

class CDeltaControllerApp : public CWinApp	//�����û���
{
public:
	CDeltaControllerApp();	//�����޲ι��캯��

// Overrides
public:
	virtual BOOL InitInstance();//����CWinApp���InitInstance()����

// Implementation

	DECLARE_MESSAGE_MAP()	//��Ϣӳ��������
};

extern CDeltaControllerApp theApp;	//������һ��ȫ�ֵ��û��������