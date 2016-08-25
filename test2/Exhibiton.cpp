// Exhibiton.cpp : implementation file
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "Exhibiton.h"
#include "afxdialogex.h"
#include "Robot.h"


// CExhibiton dialog

IMPLEMENT_DYNAMIC(CExhibiton, CDialogEx)

CExhibiton::CExhibiton(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExhibiton::IDD, pParent)
	, m_RunTime_Loop(0)
{
	m_Point1_X = 125.0;
	m_Point1_Y = 125.0;
	m_Point1_Z = 647.0;
	m_Point2_X = -125.0;
	m_Point2_Y = 125.0;
	m_Point2_Z = 647.0;
	m_Point3_X = -125.0;
	m_Point3_Y = -125.0;
	m_Point3_Z = 647.0;
	m_Point4_X = 125.0;
	m_Point4_Y = -125.0;
	m_Point4_Z = 647.0;
	m_LoopTimes = 1;
	m_looptimes_al = 0;
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
	DDX_Text(pDX, IDC_looptimes, m_looptimes_al);
	DDX_Text(pDX, IDC_RUN_TIME, m_RunTime_Loop);
}


BEGIN_MESSAGE_MAP(CExhibiton, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CExhibiton::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CExhibiton::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_looptimes, &CExhibiton::OnEnChangelooptimes)
END_MESSAGE_MAP()


// CExhibiton message handlers


void CExhibiton::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	DestroyWindow();
}

void CExhibiton::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡ�����ֵ
	if (m_Point1_X<-(rectangleSize + 100) || m_Point1_X>(rectangleSize + 100) ||
		m_Point2_X<-(rectangleSize + 100) || m_Point2_X>(rectangleSize + 100) ||
		m_Point3_X<-(rectangleSize + 100) || m_Point3_X>(rectangleSize + 100) ||
		m_Point4_X<-(rectangleSize + 100) || m_Point4_X>(rectangleSize + 100))
	{
		MessageBox(_T("������X��Χ��-300��300)"));
		return;
	}
	if (m_Point1_Y<-(rectangleSize + 100) || m_Point1_Y>(rectangleSize + 100) ||
		m_Point2_Y<-(rectangleSize + 100) || m_Point2_Y>(rectangleSize + 100) ||
		m_Point3_Y<-(rectangleSize + 100) || m_Point3_Y>(rectangleSize + 100) ||
		m_Point4_Y<-(rectangleSize + 100) || m_Point4_Y>(rectangleSize + 100))
	{
		MessageBox(_T("������Y��Χ��-300��300)"));
		return;
	}
	if (m_Point1_Z<-547 || m_Point1_Z>747 ||
		m_Point2_Z<-547 || m_Point2_Z>747 ||
		m_Point3_Z<-547 || m_Point3_Z>747 ||
		m_Point4_Z<-547 || m_Point4_Z>747)
	{
		MessageBox(_T("������Z��Χ��547��747)"));
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

	XyActionFlag = m_XyPlatform.actionScheme(Point1, P1);
	if (XyActionFlag==false)
	{
		MessageBox(_T("XYƽ̨�˶��У�"));
		return;
	}
	flag = gtsmotion.Pvt_DynamicPT(Point0,P1,1,m_Int);   //ֱ���˶�����һ��
	gtsmotion.WaitMotor();    //�ȴ������λ
	double t1=GetTickCount();//����ο�ʼǰȡ��ϵͳ����ʱ��(ms)

	for (int i=0;i<m_LoopTimes;i++)
	{
		XyActionFlag = m_XyPlatform.actionScheme(Point2, P2);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,P2,1,m_Int);   //���ǰһ�κ������г�����ֱ�����е��ڶ���
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,P2,1,m_Int);   //���ǰһ�߶�����û������Բ�ߵ���ڶ���

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = m_XyPlatform.actionScheme(Point3, P3);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,P3,1,m_Int);
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,P3,1,m_Int);

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = m_XyPlatform.actionScheme(Point4, P4);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,P4,1,m_Int);   //���ǰһ�κ������г�����ֱ�����е����ĵ�
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,P4,1,m_Int);   //���ǰһ�߶�����û������Բ�ߵ���ڶ���

		//gtsmotion.WaitMotor();    //�ȴ������λ
		XyActionFlag = m_XyPlatform.actionScheme(Point1, P1);
		gtsmotion.GetCurPos(Point0);
		if(flag == 0)
			flag = gtsmotion.Pvt_DynamicPT(Point0,P1,1,m_Int);   //���ǰһ�κ������г�����ֱ�����е���һ��
		else
			flag = gtsmotion.Pvt_DynamicPT(Point0,P1,1,m_Int);   //���ǰһ�߶�����û������Բ�ߵ����һ��

		/*m_looptimes_al++;*/
	}
	gtsmotion.WaitMotor();    //�ȴ������λ
	double t2=GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(ms)
	m_RunTime_Loop = (double)(t2-t1)/(2*m_LoopTimes);
	m_looptimes_al = 60*1000/m_RunTime_Loop;

	XyActionFlag = m_XyPlatform.actionScheme(P0, P0_turn);
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
