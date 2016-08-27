// Exhibiton.cpp : implementation file
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "Exhibiton.h"
#include "afxdialogex.h"
#include "Robot.h"

extern CXYPlatform* pXYPlatform;
// CExhibiton dialog

IMPLEMENT_DYNAMIC(CExhibiton, CDialogEx)

CExhibiton::CExhibiton(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExhibiton::IDD, pParent)
	, m_RunTime_Loop(0)
	, m_LineType(0)
{
	
}

CExhibiton::~CExhibiton()
{
}

void CExhibiton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINT1_X, m_Point1_X);
	DDX_Text(pDX, IDC_POINT1_Y, m_Point1_Y);
	DDX_Text(pDX, IDC_POINT1_Z, m_Point1_Z);
	DDV_MinMaxDouble(pDX, m_Point1_Z, 547.0, 747.0);
	DDX_Text(pDX, IDC_POINT2_X, m_Point2_X);
	DDX_Text(pDX, IDC_POINT2_Y, m_Point2_Y);
	DDX_Text(pDX, IDC_POINT2_Z, m_Point2_Z);
	DDV_MinMaxDouble(pDX, m_Point2_Z, 547.0, 747.0);
	DDX_Text(pDX, IDC_POINT3_X, m_Point3_X);
	DDX_Text(pDX, IDC_POINT3_Y, m_Point3_Y);
	DDX_Text(pDX, IDC_POINT3_Z, m_Point3_Z);
	DDV_MinMaxDouble(pDX, m_Point3_Z, 547.0, 747.0);
	DDX_Text(pDX, IDC_POINT4_X, m_Point4_X);
	DDX_Text(pDX, IDC_POINT4_Y, m_Point4_Y);
	DDX_Text(pDX, IDC_POINT4_Z, m_Point4_Z);
	DDV_MinMaxDouble(pDX, m_Point4_Z, 547.0, 747.0);
	DDX_Text(pDX, IDC_LOOP_TIMES, m_LoopTimes);
	//  DDX_Text(pDX, IDC_looptimes, m_looptimes_al);
	DDX_Text(pDX, IDC_RUN_TIME, m_RunTime_Loop);
	DDX_Radio(pDX, IDC_RADIO_LINE, m_LineType);
}


BEGIN_MESSAGE_MAP(CExhibiton, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CExhibiton::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CExhibiton::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_looptimes, &CExhibiton::OnEnChangelooptimes)
	ON_BN_CLICKED(IDC_BTN_ONETIME, &CExhibiton::OnBnClickedBtnOnetime)
END_MESSAGE_MAP()


// CExhibiton message handlers


void CExhibiton::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	//DestroyWindow();
}

void CExhibiton::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	extern int m_Int;
	UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡ�����ֵ
	if (m_Point1_X<-(RECTANGLESIZE_X + 150) || m_Point1_X>(RECTANGLESIZE_X + 150) ||
		m_Point2_X<-(RECTANGLESIZE_X + 150) || m_Point2_X>(RECTANGLESIZE_X + 150) ||
		m_Point3_X<-(RECTANGLESIZE_X + 150) || m_Point3_X>(RECTANGLESIZE_X + 150) ||
		m_Point4_X<-(RECTANGLESIZE_X + 150) || m_Point4_X>(RECTANGLESIZE_X + 150))
	{
		MessageBox(_T("������X��Χ��-315��315)"));
		return;
	}
	if (m_Point1_Y<-(RECTANGLESIZE_Y + 150) || m_Point1_Y>(RECTANGLESIZE_Y + 150) ||
		m_Point2_Y<-(RECTANGLESIZE_Y + 150) || m_Point2_Y>(RECTANGLESIZE_Y + 150) ||
		m_Point3_Y<-(RECTANGLESIZE_Y + 150) || m_Point3_Y>(RECTANGLESIZE_Y + 150) ||
		m_Point4_Y<-(RECTANGLESIZE_Y + 150) || m_Point4_Y>(RECTANGLESIZE_Y + 150))
	{
		MessageBox(_T("������Y��Χ��-370��370)"));
		return;
	}
	if (m_Point1_Z<547 || m_Point1_Z>747 ||
		m_Point2_Z<547 || m_Point2_Z>747 ||
		m_Point3_Z<547 || m_Point3_Z>747 ||
		m_Point4_Z<547 || m_Point4_Z>747)
	{
		MessageBox(_T("������Z��Χ��547��747)"));
		return;
	}
	if (m_Int > 85)
	{
		MessageBox(_T("�ٶȲ�Ҫ����85%"));
		return;
	}
	AfxBeginThread(_threadRobotAction, (void*)this);
}

UINT __cdecl CExhibiton::_threadRobotAction(LPVOID pParam)
{
	CExhibiton *exhibition = (CExhibiton*)pParam;

	exhibition->threadExhibitionRobotAction();
	//UpdateData(FALSE);
	return 0;
}

void CExhibiton::threadExhibitionRobotAction()
{
	short flag=1;
	double Point0[3],Point1[3],Point2[3],Point3[3],Point4[3]; //��ǰ����ĸ�Ŀ��������
	double P0[3], P0_turn[3], P1[3], P2[3], P3[3], P4[3];     //��¼��ʼ��
	extern double ovalue[];    //��������λ����������λ������
	extern int m_Int;
	bool XyActionFlag;

	//UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡ�����ֵ

	//gtsmotion.WaitMotor();    //�ȴ������λ
	gtsmotion.GetCurPos(Point0);

	P0[0] = ovalue[0];
	P0[1] = ovalue[1];
	P0[2] = ovalue[2];

	Point1[0] = m_Point1_X;
	Point1[1] = m_Point1_Y;
	Point1[2] = m_Point1_Z;

	Point2[0] = m_Point2_X;
	Point2[1] = m_Point2_Y;
	Point2[2] = m_Point2_Z;

	Point3[0] = m_Point3_X;
	Point3[1] = m_Point3_Y;
	Point3[2] = m_Point3_Z;

	Point4[0] = m_Point4_X;
	Point4[1] = m_Point4_Y;
	Point4[2] = m_Point4_Z;

	extern long T_gap;         //����ʱ����
	extern int n;                 //����

	XyActionFlag = pXYPlatform->actionScheme(Point1, P1);
	if (XyActionFlag==false)
	{
		MessageBox(_T("1.XYƽ̨�˶��У�2.���ڻ�δ�򿪣�"));
		return;
	}
	flag = gtsmotion.Pvt_DynamicPT(Point0,P1,1,m_Int);   //ֱ���˶�����һ��
	gtsmotion.WaitMotor();    //�ȴ������λ
	double t1;//����ο�ʼǰȡ��ϵͳ����ʱ��(ms)

	for (int i=0;i<m_LoopTimes;i++)
	{
		t1=GetTickCount();
		XyActionFlag = pXYPlatform->actionScheme(Point2, P2);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
		{
			MessageBox(_T("�˶��滮�д���������ֵ�����ԣ�"));
			return;
		}
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0, P2, (m_LineType + 1), m_Int);   //���ǰһ�߶�����û������Բ�ߵ���ڶ���

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = pXYPlatform->actionScheme(Point3, P3);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
		{
			MessageBox(_T("�˶��滮�д���������ֵ�����ԣ�"));
			return;
		}
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0, P3, (m_LineType + 1), m_Int);

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = pXYPlatform->actionScheme(Point4, P4);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
		{
			MessageBox(_T("�˶��滮�д���������ֵ�����ԣ�"));
			return;
		}
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0, P4, (m_LineType + 1), m_Int);   //���ǰһ�߶�����û������Բ�ߵ���ڶ���

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = pXYPlatform->actionScheme(Point1, P1);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
		{
			MessageBox(_T("�˶��滮�д���������ֵ�����ԣ�"));
			return;
		}
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0, P1, (m_LineType + 1), m_Int);   //���ǰһ�߶�����û������Բ�ߵ����һ��

		gtsmotion.WaitMotor();    //�ȴ������λ
		double t2=GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(ms)
		m_RunTime_Loop = (double)(t2-t1);
	}
	P0[2] += 247;
	XyActionFlag = pXYPlatform->actionScheme(P0, P0_turn);
	gtsmotion.GetCurPos(Point0);//��ȥ��ʼ��
	flag = gtsmotion.Pvt_DynamicPT(Point0,P0_turn,1,m_Int);
}

void CExhibiton::OnDestroy()
{
	//CDialogEx::OnDestroy();
	CDialogEx::PostNcDestroy();  
	// TODO: Add your message handler code here
	//delete this;
}


void CExhibiton::OnEnChangelooptimes()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


BOOL CExhibiton::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Point1_X = 160.0;
	m_Point1_Y = 220.0;
	m_Point1_Z = 647.0;
	m_Point2_X = -160.0;
	m_Point2_Y = 220.0;
	m_Point2_Z = 647.0;
	m_Point3_X = -160.0;
	m_Point3_Y = -220.0;
	m_Point3_Z = 647.0;
	m_Point4_X = 160.0;
	m_Point4_Y = -220.0;
	m_Point4_Z = 647.0;
	m_LoopTimes = 1;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CExhibiton::OnBnClickedBtnOnetime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
}
