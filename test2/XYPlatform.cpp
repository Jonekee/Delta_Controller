// XYPlatform.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "XYPlatform.h"
#include "afxdialogex.h"
#include "Robot.h"
#include <cmath>

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
	DDX_Control(pDX, IDC_XY_STATE, m_StateInform);
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
	ON_WM_CLOSE()
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
		CString strtemp;
		LoopTest(strtemp);
		//strtemp="27";
		if (m_mscomm.get_PortOpen())
		{
			if (strtemp == "06" || strtemp == "15")
			{
				timerInit = true;
				str = _T("�رմ���");
				UpdateData(true);
				h_BtnOpen->SetWindowTextW(str);//�ı䰴ť����Ϊ���رմ��ڡ�
				StateShow(_T(">>>���ڴ򿪳ɹ���"));
				SetTimer(1, 300, NULL);
				return;
			}
			str = _T("�رմ���");
			UpdateData(true);
			h_BtnOpen->SetWindowTextW(str);//�ı䰴ť����Ϊ���رմ��ڡ�
			StateShow(_T(">>>���ڴ�״̬����������رպ��ٴ򿪣�"));
			MessageBox(_T("���ڴ�״̬����������رպ��ٴ򿪣�"));
			return;
		}
	}
	else
	{
		KillTimer(1);
		OnClickedBtnStop();
		m_mscomm.put_PortOpen(false);
		str = _T("�򿪴���");
		UpdateData(true);
		h_BtnOpen->SetWindowTextW(str);
		StateShow(_T(">>>�����ѹرգ�"));
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
	CString str;
	LoopTest(str);
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


void CXYPlatform::OnClickedBtnYrz()//������е��������������������෴
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
	m_SerialPort.SetCurSel(0);

	//m_XyState.
	m_StateInform.SetReadOnly(TRUE);
	m_StateInform.SetWindowTextW(_T(">>>����XYƽ̨�Ƿ���㣡"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CXYPlatform::OnTimer(UINT_PTR nIDEvent)//�����Լ������˿�״̬
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData(true);
	CString strtemp;
	static bool flag[6] = { 0 };
	readState(strtemp);
	
	if (timerInit == false && strtemp != "06" && strtemp != "15")
	{
		if (strFlag[0] == '1' && flag[0] == false)//Y������λ״̬��ʾ
		{
			flag[0] = true;
			StateShow(_T(">>>Y�ᵽ������λ��"));
			MessageBox(_T("Y�ᵽ������λ��"));
		}
		else if (strFlag[0] == '0'&&flag[0] == true)
		{
			flag[0] = false;
			StateShow(_T(">>>Y���뿪����λ..."));
		}

		if (strFlag[2] == '1' && flag[2] == false)//Y������λ״̬��ʾ
		{
			flag[2] = true;
			StateShow(_T(">>>Y�ᵽ������λ��"));
			MessageBox(_T("Y�ᵽ������λ��"));
		}
		else if (strFlag[2] == '0'&&flag[2] == true)
		{
			flag[2] = false;
			StateShow(_T(">>>Y���뿪����λ..."));
		}

		if (strFlag[1] == '1' && flag[1] == false)//Y����λ״̬��ʾ
		{
			flag[1] = true;
			StateShow(_T(">>>Y��ص���λ��"));
		}
		else if (strFlag[1] == '0'&&flag[1] == true)
		{
			flag[1] = false;
			StateShow(_T(">>>Y���뿪��λ..."));
		}

		if (strFlag[3] == '1' && flag[3] == false)//X������λ״̬��ʾ
		{
			flag[3] = true;
			StateShow(_T(">>>X�ᵽ������λ��"));
			MessageBox(_T("X�ᵽ������λ��"));
		}
		else if (strFlag[3] == '0'&&flag[3] == true)
		{
			flag[3] = false;
			StateShow(_T(">>>X���뿪����λ..."));
		}

		if (strFlag[5] == '1' && flag[5] == false)//X������λ״̬��ʾ
		{
			flag[5] = true;
			StateShow(_T(">>>X�ᵽ������λ��"));
			MessageBox(_T("X�ᵽ������λ��"));
		}
		else if (strFlag[5] == '0'&&flag[5] == true)
		{
			flag[5] = false;
			StateShow(_T(">>>X���뿪����λ..."));
		}

		if (strFlag[4] == '1' && flag[4] == false)//X����λ״̬��ʾ
		{
			flag[4] = true;
			StateShow(_T(">>>X��ص���λ��"));
		}
		else if (strFlag[4] == '0'&&flag[4] == true)
		{
			flag[4] = false;
			StateShow(_T(">>>X���뿪��λ..."));
		}
	}
	else if (strtemp == "15")
	{
		MessageBox(_T("�źŷ���ʧ�ܣ������·��ͣ�"));
	}
	else
	{
		timerInit = false;
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
	//UpdateData(true);
	CByteArray send_data;
	switch (index)
	{
	case 0:
		BYTE commstr0[11];
		commstr0[0] = 0x02;
		commstr0[1] = 0x30;
		getAddress(&commstr0[2], address);//����ʼ��ַת��ΪASCII�룬������
		getNum(&commstr0[6], byteNum);//�ڶ�������Ϊ�ֽ�����ת��ΪASCII�룬������
		commstr0[8] = 0x03;
		getSumChk(&commstr0[9], &commstr0[1], 8);
		for (int i = 0; i < 11; i++)
		{
			send_data.Add(commstr0[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(50);
		break;
	case 1:
		BYTE commstr1[15];
		commstr1[0] = 0x02;//��ʼ��
		commstr1[1] = 0x31;//д��������
		getAddress(&commstr1[2], address);
		getNum(&commstr1[6], byteNum);
		getData(&commstr1[8], data);
		commstr1[12] = 0x03;//������
		getSumChk(&commstr1[13], &commstr1[1], 12);
		for (int i = 0; i < 15; i++)
		{
			send_data.Add(commstr1[i]);
		}
		m_mscomm.put_Output(COleVariant(send_data));
		Sleep(50);
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

void CXYPlatform::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}

void CXYPlatform::getReturn(BYTE* rxdata, long* len)
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG k;
	variant_inp = m_mscomm.get_Input(); //��������
	safearray_inp = variant_inp;  //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
	*len = safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
	for (k = 0; k<*len; k++)
	{
		safearray_inp.GetElement(&k, rxdata + k);
	}
}

void CXYPlatform::StateShow(CString str)
{
	m_StateInform.GetWindowTextW(m_XyState);
	m_XyState += _T("\r\n") + str;
	m_StateInform.SetWindowTextW(m_XyState);
}

void CXYPlatform::readState(CString& strtemp)
{
	int m_addressH, byteNum;
	LONG len, k;
	BYTE rxdata[512];//����BYTE����
	CString str;
	for (int i = 0; i<3; i++)
	{
		switch (i)
		{
		case 0:
			m_addressH = 0x80;//����˿�X0~X7��ʼ��ַʮ�����Ʊ�ʾ
			byteNum = 1;
			break;
		case 1:
			m_addressH = 0x100;//�м�̵���M0~M7��ʼ��ַʮ�����Ʊ�ʾ
			byteNum = 1;
			break;
		case 2:
			m_addressH = 0x102;//�м�̵���M16~M23��ʼ��ַʮ�����Ʊ�ʾ
			byteNum = 1;
			break;
		}

		sendCommand(READ, m_addressH, byteNum, NULL);
		getReturn(rxdata, &len);//��ȡ���ջ������ķ���ֵ

		for (k = 0; k < len; k++)//������ת��ΪCString�ͱ���
		{
			BYTE bt = *(char*)(rxdata);//�ַ���
			strtemp.Format(_T("%02X"), bt);
		}
		if (strtemp == "06")//��Ӧ����
			break;
		//MessageBox(_T("��PLCͨѶ����!", "��PLCͨѶ���"));

		for (k = 2; k>1; k -= 2)//��ȡ���ֽ���Ϊ2
		{
			BYTE temp = *(char*)(rxdata + k);
			BYTE bt1 = ((temp - 0x30)<10) ? (temp - 0x30) : (temp - 0x41 + 10);
			temp = *(char*)(rxdata + k - 1);
			BYTE bt2 = ((temp - 0x30)<10) ? (temp - 0x30) : (temp - 0x41 + 10);
			BYTE bt = bt1 + bt2 * 16;
			char p[10];
			strtemp = itoa(bt, p, 2);
			str.Format(_T("%08s,"), strtemp);
		}

		switch (i)
		{
		case 0:
			for (int i = 0, j = 7; i < 8; i++, j--)
				strFlag[j] = str[i];
			break;
		case 1:
			for (int i = 0, j = 7; i < 8; i++, j--)
				mFlag0[j] = str[i];
			break;
		case 2:
			for (int i = 0, j = 7; i < 8; i++, j--)
				mFlag16[j] = str[i];
			break;
		}
	}
}

void CXYPlatform::getXyState(int* state, double* pos)
{
		*state = 0;
		if (mFlag16[0] == '1' && mFlag16[2] == '1')//ƽ̨��λ��1
		{
			*state = 1;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[2] == '1'&&strFlag[4] == '1')
		{
			*state = 2;
			*pos = 0;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[1] == '1'&&mFlag16[2] == '1')
		{
			*state = 3;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = RECTANGLESIZE_Y;
		}
		if (mFlag16[0] == '1'&&strFlag[1] == '1')
		{
			*state = 4;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = 0;
		}
		if (strFlag[4] == '1'&&strFlag[1] == '1')
		{
			*state = 5;
			*pos = 0;
			*(pos + 1) = 0;
		}
		if (strFlag[1] == '1'&&mFlag16[1] == '1')
		{
			*state = 6;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = 0;
		}
		if (mFlag16[0] == '1'&&mFlag16[3] == '1')
		{
			*state = 7;
			*pos = -RECTANGLESIZE_X;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (strFlag[4] == '1'&&mFlag16[3] == '1')
		{
			*state = 8;
			*pos = 0;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (mFlag16[1] == '1' && mFlag16[3] == '1')
		{
			*state = 9;
			*pos = RECTANGLESIZE_X;
			*(pos + 1) = -RECTANGLESIZE_Y;
		}
		if (strFlag[0] == '1' || strFlag[2] == '1' || strFlag[3] == '1' || strFlag[5] == '1')
		{
			*state = 10;
			*pos = 220;
			*(pos + 1) = 220;
		}
}

void CXYPlatform::moveAutoZero()
{
	int stateFlag = 0, loopNum = 0;
	double xyPos[2];
	if (m_mscomm.get_PortOpen())
	{
		do
		{
			Sleep(1000);
			getXyState(&stateFlag, xyPos);
			loopNum++;
		} while (stateFlag != 0 || loopNum < 10);
	}

	int address = 256, byteNum = 2, data = 0;

	switch (stateFlag)
	{
	case 0:
		if (m_mscomm.get_PortOpen())
		{
			MessageBox(_T("XYƽ̨���쳣��"));
			Sleep(5000);
		}
		break;
	case 1:
		data = 17;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 2:
		OnClickedBtnYrz();
		break;
	case 3:
		data = 18;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 4:
		OnClickedBtnXrz();
		break;
	case 5:
		OnClickedBtnStop();
		break;
	case 6:
		OnClickedBtnXlz();
		break;
	case 7:
		data = 33;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 8:
		OnClickedBtnYlz();
		break;
	case 9:
		data = 34;
		sendCommand(WRITE, address, byteNum, data);
		break;
	case 10:
		OnClickedBtnStop();
		break;
	}
}

BOOL CXYPlatform::serialPortState()
{
	if (m_mscomm.get_PortOpen())
		return TRUE;
	else
		return FALSE;
}

void CXYPlatform::LoopTest(CString& str)
{
	CString strSend;
	CByteArray hexData, sendData;
	m_mscomm.put_InBufferCount(0);//���úͷ��ؽ��ջ��������ֽ�������Ϊ0ʱ��ջ�����
	m_mscomm.put_OutBufferCount(0);//���úͷ��ط��ͻ��������ֽ�������Ϊ0ʱ��ջ�����
	BYTE commStr;
	commStr = 0x05;
	sendData.Add(commStr);
	m_mscomm.put_Output(COleVariant(sendData));//����
	Sleep(100);

	BYTE rxData[512];//����BYTE����
	CString strtemp;
	long k, len;
	getReturn(rxData, &len);
	for (k = 0; k < len; k++)//������ת��ΪCString�ͱ���
	{
		BYTE bt = *(char*)(rxData);//�ַ���
		strtemp.Format(_T("%02X"), bt);
	}
	if (strtemp == "06")//��Ӧ����
		MessageBox(_T("��PLCͨѶ����!", "��PLCͨѶ���"));
	else if (strtemp == "15")//��Ӧ����
		MessageBox(_T("��PLCͨѶ��������������·���ԣ�", "��PLCͨѶ���"));
	else
		MessageBox(_T("��PLCû�����ӣ�����1��PLC�Ƿ��ϵ� 2��PC��PLCͨ���Ƿ������⣡", "��ʾ"));

	str = strtemp;
}

void CXYPlatform::moveXyPlatform(int data)
{
	int address = 256, byteNum = 2;
	/*OnClickedBtnStop();
	Sleep(100);*/
	sendCommand(WRITE, address, byteNum, data);
}

bool CXYPlatform::actionScheme(double* targetPos, double* deltaTarget)
{
	//XYƽ̨�����߼�
	int XyCurrentPosition, XyTargetPosition, XyData = 0;
	double pos[2], XyTarget_X, XyTarget_Y;//��¼XYƽ̨λ��
	getXyState(&XyCurrentPosition, pos);
	if (XyCurrentPosition > 0 && XyCurrentPosition < 10)
	{
		double distance = std::sqrt(std::powf(std::fabs(*(targetPos + 0) - pos[0]), 2) + std::powf(std::fabs(*(targetPos + 1) - pos[1]), 2));
		if (distance <= 150)
		{
			//��ȡdeltaĿ��ֵ
			*(deltaTarget+0) = *(targetPos + 1) - pos[1];
			*(deltaTarget + 1) = *(targetPos + 0) - pos[0];
			*(deltaTarget + 2) = *(targetPos + 2) - 247.0;
		}
		else
		{
			int X_Index = 0, Y_Index = 0;
			if (*(targetPos + 0) < -(RECTANGLESIZE_X / 2)) X_Index = 1;    //Ŀ��λ�����Ǹ�������
			else if (*(targetPos + 0)>(RECTANGLESIZE_X / 2)) X_Index = 3;
			else X_Index = 2;
			if (*(targetPos + 1) > (RECTANGLESIZE_Y / 2)) Y_Index = 1;
			else if (*(targetPos + 1) < -(RECTANGLESIZE_Y / 2)) Y_Index = 3;
			else Y_Index = 2;
			//XyTargetPosition = (Y_Index - 1) * 3 + X_Index;
			XyTarget_X = (X_Index - 2) * RECTANGLESIZE_X;
			XyTarget_Y = -(Y_Index - 2) * RECTANGLESIZE_Y;

			//��ȡdeltaĿ��ֵ
			*(deltaTarget + 0) = *(targetPos + 1) - XyTarget_Y;
			*(deltaTarget + 1) = *(targetPos + 0) - XyTarget_X;
			*(deltaTarget + 2) = *(targetPos + 2) - 247.0;

			//��ȡXYƽ̨��ָ������
			if (XyTarget_X > pos[0]) XyData = 4;
			else if (XyTarget_X == pos[0]) XyData = 0;
			else XyData = 8;

			if (XyTarget_X*pos[0]<0) XyData += 256;//M8��־λ��X���Ƿ��Խ��λ
			else XyData += 0;

			if (XyTarget_Y > pos[1]) XyData += 128;
			else if (XyTarget_Y == pos[1]) XyData += 0;
			else XyData += 64;

			if (XyTarget_Y*pos[1]<0) XyData += 512;//M9��־λ��Y���Ƿ��Խ��λ
			else XyData += 0;

			moveXyPlatform(XyData);
			motorAction();
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CXYPlatform::motorAction()
{
	Sleep(500);//��ʱ��ÿ��300ms��ȡһ��״̬����Ϊ��������󣬵ȴ�500ms��ȷ������������ȡ���״̬
	do {} while (mFlag0[0]!='0' || mFlag0[1]!='0' || mFlag0[2]!='0' || mFlag0[3]!='0' || //˫�ر���
		mFlag0[4]!='0' || mFlag0[5]!='0' || mFlag0[6]!='0' || mFlag0[7]!='0' ||          //ȷ���������λ�����
		mFlag16[6] != '0' || mFlag16[7] != '0');                                         //ȷ������Ѿ���λ
}