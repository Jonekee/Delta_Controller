#pragma once
#include "mscomm.h"
#include "afxwin.h"


// CXYPlatform �Ի���

class CXYPlatform : public CDialogEx
{
	DECLARE_DYNAMIC(CXYPlatform)

public:
	CXYPlatform(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CXYPlatform();

// �Ի�������
	enum { IDD = IDD_XYDELTADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	const short WRITE, READ;
	void getAddress(BYTE *data, int i);//��ȡ��Ԫ����ʼ��ַ
	void getNum(BYTE *data, int i);//��ȡ�ֽ���
	void getData(BYTE *data, int i);//��ȡд�������
	void getSumChk(BYTE *datades, BYTE *datascr, int len);//��ȡ�����
	void sendCommand(const short index, int address, int byteNum, int data);
	void StateShow(CString str);
	void getReturn(BYTE* rxdata, long* len);
	virtual BOOL OnInitDialog();

	afx_msg void OnClickedBtnOpen();
	afx_msg void OnClickedBtnLoop();
	void LoopTest(CString* str);
	afx_msg void OnClickedBtnXrz();
	afx_msg void OnClickedBtnXlz();
	afx_msg void OnClickedBtnYrz();
	afx_msg void OnClickedBtnYlz();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	CMscomm m_mscomm;
	CString m_XyState;
	CComboBox m_SerialPort;
	CEdit m_StateInform;
	CString strFlag[8], mFlag0[8], mFlag16[8];
	bool timerInit;

public:
	// ����ͨѶ����
	afx_msg void OnClickedBtnStop();
	void moveXLf();
	void moveXRf();
	void moveYLf();
	void moveYRf();
	void getXyState(int* state, double* pos);
	void moveAutoZero();
	void moveXyPlatform(int data);
	BOOL serialPortState();
};
