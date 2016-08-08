// Conveyor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DELTA_CONTROLLER.h"
#include "Conveyor.h"
#include "afxdialogex.h"
#include "Clibration.h"


// CConveyor �Ի���

IMPLEMENT_DYNAMIC(CConveyor, CDialogEx)

CConveyor::CConveyor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConveyor::IDD, pParent)
	, m_ScaleFactor(0)
{
	//m_GetSecondPos.EnableWindow(FALSE);
	//GetDlgItem(IDC_BtSecondPos)->EnableWindow(FALSE);
}

CConveyor::~CConveyor()
{
}

void CConveyor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BtFirstPos, m_GetFirstPos);
	DDX_Control(pDX, IDC_BtSecondPos, m_GetSecondPos);
	DDX_Control(pDX, IDC_BtThirdPos, m_GetThirdPos);
	DDX_Text(pDX, IDC_ScaleFactor, m_ScaleFactor);
}


BEGIN_MESSAGE_MAP(CConveyor, CDialogEx)
	ON_BN_CLICKED(IDC_BtThirdPos, &CConveyor::OnBnClickedBtthirdpos)
	ON_BN_CLICKED(IDC_BtFirstPos, &CConveyor::OnBnClickedBtfirstpos)
	ON_BN_CLICKED(IDC_BtSecondPos, &CConveyor::OnBnClickedBtsecondpos)
	ON_BN_CLICKED(IDCANCEL, &CConveyor::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CalAct, &CConveyor::OnBnClickedCalact)
END_MESSAGE_MAP()


// CConveyor ��Ϣ�������

void CConveyor::OnBnClickedBtthirdpos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_gtsmotion.GetCurPos(m_robpos3);
	flag = m_gtsmotion.GetConveyorPos(&m_conpos3);
	m_GetThirdPos.EnableWindow(FALSE);
}


void CConveyor::OnBnClickedBtfirstpos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_gtsmotion.GetCurPos(m_robpos1);
	flag = m_gtsmotion.GetConveyorPos(&m_conpos1);
	m_GetSecondPos.EnableWindow(TRUE);
	m_GetFirstPos.EnableWindow(FALSE);
}


void CConveyor::OnBnClickedBtsecondpos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = m_gtsmotion.GetConveyorPos(&m_conpos2);
	m_gtsmotion.GetCurPos(m_robpos2);
	m_GetThirdPos.EnableWindow(TRUE);
	m_GetSecondPos.EnableWindow(FALSE);
}


BOOL CConveyor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_GetSecondPos.EnableWindow(FALSE);
	m_GetThirdPos.EnableWindow(FALSE);
	m_ScaleFactor = 10;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CConveyor::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_GetFirstPos.EnableWindow(TRUE);
	m_GetSecondPos.EnableWindow(FALSE);
	m_GetThirdPos.EnableWindow(FALSE);
}


void CConveyor::OnBnClickedCalact()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Clibration *calibrate = new Clibration;
	flag = calibrate->Con_Cal(m_robpos1,m_robpos2,m_robpos3,&m_conpos1,&m_conpos2,&m_conpos2);
	delete calibrate;
	UpdateData(FALSE);
}
