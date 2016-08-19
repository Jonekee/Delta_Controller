// XYPlatform.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "XYPlatform.h"
#include "afxdialogex.h"


// CXYPlatform �Ի���

IMPLEMENT_DYNAMIC(CXYPlatform, CDialogEx)

CXYPlatform::CXYPlatform(CWnd* pParent /*=NULL*/)
: CDialogEx(CXYPlatform::IDD, pParent)
, m_XyState(_T("")), READ(0), WRITE(1)
{

}

CXYPlatform::~CXYPlatform()
{
}

void CXYPlatform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM, m_mscomm);
	DDX_Text(pDX, IDC_XY_STATE, m_XyState);
	DDX_Control(pDX, IDC_SERIAL_PORT, m_SerialPort);
}


BEGIN_MESSAGE_MAP(CXYPlatform, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CXYPlatform::OnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_STOP, &CXYPlatform::OnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_LOOP, &CXYPlatform::OnClickedBtnLoop)
	ON_BN_CLICKED(IDC_BTN_XRZ, &CXYPlatform::OnClickedBtnXrz)
	ON_BN_CLICKED(IDC_BTN_XLZ, &CXYPlatform::OnClickedBtnXlz)
	ON_BN_CLICKED(IDC_BTN_YRZ, &CXYPlatform::OnClickedBtnYrz)
	ON_BN_CLICKED(IDC_BTN_YLZ, &CXYPlatform::OnClickedBtnYlz)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CXYPlatform ��Ϣ�������


void CXYPlatform::OnClickedBtnOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItemText(IDC_BTN_OPEN, str);
	CWnd* h_BtnOpen = GetDlgItem(IDC_BTN_OPEN);//ָ��ؼ���caption
	if (!m_mscomm.get_PortOpen())
	{
		m_mscomm.put_CommPort((m_SerialPort.GetCurSel() + 1));    //Ĭ��ѡ��com3
		m_mscomm.put_Settings(_T("9600,E,7,1"));
		m_mscomm.put_InputMode(1);
		m_mscomm.put_RThreshold(1);  //����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
		m_mscomm.put_InBufferSize(1024);
		m_mscomm.put_OutBufferSize(1024);
		m_mscomm.put_Handshaking(0); //�����ź�
		m_mscomm.put_InputLen(0);  //���úͷ���inputÿ�ζ������ֽ�������Ϊ0ʱ�������ջ������е�����
		m_mscomm.put_InBufferCount(0);//���úͷ��ؽ��ջ��������ֽ�������Ϊ0ʱ��ս��ջ�����
		m_mscomm.put_OutBufferCount(0);//���úͷ��ط��ͻ��������ֽ�������Ϊ0ʱ��շ��ͻ�����
		m_mscomm.put_PortOpen(TRUE);//�򿪴���
		SetTimer(1, 300, NULL);
		if (m_mscomm.get_PortOpen())
		{
			str = _T("�رմ���");
			UpdateData(true);
			h_BtnOpen->SetWindowTextW(str);//�ı䰴ť����Ϊ���رմ��ڡ�
		}
	}
	else
	{
		KillTimer(1);
		m_mscomm.put_PortOpen(false);
		str = _T("�򿪴���");
		UpdateData(true);
		h_BtnOpen->SetWindowTextW(str);
	}
}


void CXYPlatform::OnClickedBtnStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int address = 256, byteNum = 2, data = 0;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnLoop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strSend;
	CByteArray hexData, sendData;
	m_mscomm.put_InBufferCount(0);//���úͷ��ؽ��ջ��������ֽ�������Ϊ0ʱ��ջ�����
	m_mscomm.put_OutBufferCount(0);//���úͷ��ط��ͻ��������ֽ�������Ϊ0ʱ��ջ�����
	BYTE commStr;
	commStr = 0x05;
	sendData.Add(commStr);
	m_mscomm.put_Output(COleVariant(sendData));//����
	Sleep(100);

	VARIANT variantInp;
	COleSafeArray safeArrayInp;
	LONG len, k;
	BYTE rxData[512];//����BYTE����
	CString strtemp;
	variantInp = m_mscomm.get_Input();//��������
	safeArrayInp = variantInp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
	len = safeArrayInp.GetOneDimSize();//�õ���Ч���ݳ���
	for (k = 0; k < len; k++)
	{
		safeArrayInp.GetElement(&k, rxData + k);//ת��ΪBYTE������
	}
	for (k = 0; k < len; k++)//������ת��ΪCString�ͱ���
	{
		BYTE bt = *(char*)(rxData);//�ַ���
		strtemp.Format(_T("%02X"), bt);
	}
	if (strtemp == "06")//��Ӧ����
		MessageBox(_T("��PLCͨѶ����!", "��PLCͨѶ���"));
	else if (strtemp == "15")//��Ӧ����
		MessageBox(_T("��PLCͨѶ������!", "��PLCͨѶ���"));
	else
		MessageBox(_T("��PLCû������!", "��ʾ"));
}


void CXYPlatform::OnClickedBtnXrz()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int address = 256, byteNum = 2, data = 1;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnXlz()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int address = 256, byteNum = 2, data = 2;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnYrz()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int address = 256, byteNum = 2, data = 16;
	sendCommand(WRITE, address, byteNum, data);
}


void CXYPlatform::OnClickedBtnYlz()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int address = 256, byteNum = 2, data = 32;
	sendCommand(WRITE, address, byteNum, data);
}


BOOL CXYPlatform::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;
	int i;
	for (i = 0; i < 15; i++)
	{
		str.Format(_T("com %d"), i + 1);
		m_SerialPort.InsertString(i, str);
	}
	m_SerialPort.SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CXYPlatform::OnTimer(UINT_PTR nIDEvent)//�����Լ������˿�״̬
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData(true);
	CByteArray send_data;
	BYTE commstr[11];
	commstr[0] = 0x02;
	commstr[1] = 0x30;
	int m_addressH = 0x80;//��ʼ��ַʮ�����Ʊ�ʾ
	getAddress(&commstr[2], m_addressH);//����ʼ��ַת��ΪASCII�룬������
	getNum(&commstr[6], 32);//�ڶ�������Ϊ�ֽ�����ת��ΪASCII�룬������
	commstr[8] = 0x03;
	getSumChk(&commstr[9], &commstr[1], 8);
	for (int i = 0; i < 11; i++)
	{
		send_data.Add(commstr[i]);
	}
	m_mscomm.put_Output(COleVariant(send_data));
	Sleep(200);

	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[512];//����BYTE����
	CString strtemp, str;
	variant_inp = m_mscomm.get_Input();//��ȡ������
	safearray_inp = variant_inp;//VARIANT�ͱ���ת��ΪCOleSafeArray�ͱ���
	len = safearray_inp.GetOneDimSize();//�õ���Ч���ݳ���
	for (k = 0; k < len; k++)
	{
		safearray_inp.GetElement(&k, rxdata + k);
	}
	for (k = 2; k>1; k -= 2)//��ȡ���ֽ���Ϊ2
	{
		BYTE temp = *(char*)(rxdata + k);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CXYPlatform::getAddress(BYTE *data, int i)
{
	BYTE b = i & 0xf;//��λ�룬���������ֽ�
	data[3] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;//i����4λ���ٰ�λ�룬���������嵽�ڰ�λ
	data[2] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 8) & 0xf;
	data[1] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 12) & 0xf;
	data[0] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getNum(BYTE *data, int i)//iΪ�ֽ���
{
	BYTE b = i & 0xf;//��λ��
	data[1] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;
	data[0] = (b < 10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getData(BYTE *data, int i)
{
	BYTE b = i & 0xf;
	data[1] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 4) & 0xf;
	data[0] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 8) & 0xf;
	data[3] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
	b = (i >> 12) & 0xf;
	data[2] = (b<10) ? (b + 0x30) : (b + 0x41 - 0xa);
}

void CXYPlatform::getSumChk(BYTE *datades, BYTE *datascr, int len)//����У����
{
	int a = 0;
	for (int i = 0; i < len; i++)
	{
		a += (*(datascr + i));
	}
	getNum(datades, a);
}

void CXYPlatform::sendCommand(const short index, int address, int byteNum, int data)
{
	UpdateData(true);
	switch (index)
	{
	case 0:
		break;
	case 1:
		CByteArray send_data;
		BYTE commstr[15];
		commstr[0] = 0x02;//��ʼ��
		commstr[1] = 0x31;//д��������
		getAddress(&commstr[2], address);
		getNum(&commstr[6], byteNum);
		getData(&commstr[8], data);
		commstr[12] = 0x03;//������
		getSumChk(&commstr[13], &commstr[1], 12);
		for (int i = 0; i < 15; i++)
		{
			send_data.Add(commstr[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(100);
		break;
	}
}

void CXYPlatform::moveXLf()
{
	int address = 256, byteNum = 2, data = 8;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveXRf()
{
	int address = 256, byteNum = 2, data = 4;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveYLf()
{
	int address = 256, byteNum = 2, data = 128;
	sendCommand(WRITE, address, byteNum, data);
}

void CXYPlatform::moveYRf()
{
	int address = 256, byteNum = 2, data = 64;
	sendCommand(WRITE, address, byteNum, data);
}