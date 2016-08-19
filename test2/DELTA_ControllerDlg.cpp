
// test2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DELTA_Controller.h"
#include "DELTA_ControllerDlg.h"
#include "afxdialogex.h"
#include "conio.h"
#pragma comment(lib,"gts.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////ȫ�ֱ���///////////////////
bool SimulationCheck;	//����ѡ�����ⲿ����
bool OnMotion;		//�˶������ⲿ����
int m_Int;   //�ٶ�����ȡֵ

/////////////////////////VISION

CDELTA_ControllerDlg* m_iDELTADLg;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnExhibition();
	afx_msg void OnDestroy();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


//	ON_COMMAND(ID_Exhibition, &CAboutDlg::OnExhibition)
	ON_WM_DESTROY()    
END_MESSAGE_MAP()

CAboutDlg dlgAbout;
// Ctest2Dlg dialog




CDELTA_ControllerDlg::CDELTA_ControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDELTA_ControllerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//��ʼ�����������
	m_LoopTimes = 1;
	m_Rtate = 0.0;
	m_Pos_X = 0.0;
	m_Pos_Y = 0.0;
	m_Pos_Z = 326.19012184371931;
	m_Zero_R = 0.0;
	m_Zero_X = 0.0;
	m_Zero_Y = 0.0;
	m_Zero_Z = 326.19012184371931;

	//ָ���ʼ��
	pXYPlatform = new CXYPlatform();
	pES = NULL;
	pCT = NULL;
}

CDELTA_ControllerDlg::~CDELTA_ControllerDlg()
{
	if(pES) 
		delete pES;
	if(pCT) 
		delete pCT;
	if (pXYPlatform)
		delete pXYPlatform;
}

void CDELTA_ControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCARA_ControllerDlg)
	DDX_Control(pDX, IDC_SLIDER_VEL,m_sliderVel);
	//}}AFX_DATA_MAP
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_X, m_Pos_X);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_Y, m_Pos_Y);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_Z, m_Pos_Z);
	//  DDX_Text(pDX, IDC_EtEDITPOSITION_R, m_Rotate);
	DDX_Text(pDX, IDC_EtEDITPOSITION_LOOP, m_LoopTimes);
	DDX_Text(pDX, IDC_EtEDITPOSITION_R, m_Rtate);
	DDX_Text(pDX, IDC_EtEDITPOSITION_X, m_Pos_X);
	DDX_Text(pDX, IDC_EtEDITPOSITION_Y, m_Pos_Y);
	DDX_Text(pDX, IDC_EtEDITPOSITION_Z, m_Pos_Z);
	DDX_Text(pDX, IDC_EtSetZero_R, m_Zero_R);
	//  DDX_Control(pDX, IDC_EtSetZero_X, m_Zero_X);
	DDX_Text(pDX, IDC_EtSetZero_X, m_Zero_X);
	DDX_Text(pDX, IDC_EtSetZero_Y, m_Zero_Y);
	DDX_Text(pDX, IDC_EtSetZero_Z, m_Zero_Z);
}

BEGIN_MESSAGE_MAP(CDELTA_ControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDELTA_ControllerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDELTA_ControllerDlg::OnBnClickedCancel)
	ON_COMMAND(ID_SET_StrokeLimit, &CDELTA_ControllerDlg::OnSetStrokelimit)
	ON_COMMAND(ID_Monitor_Start, &CDELTA_ControllerDlg::OnMonitorStart)
//	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER_VEL, &CDELTA_ControllerDlg::OnNMOutofmemorySliderVel)
//	ON_BN_CLICKED(IDC_BtLINEX_P, &CDELTA_ControllerDlg::OnBnClickedBtlinexP)
//	ON_BN_CLICKED(IDC_BtLINEX_N, &CDELTA_ControllerDlg::OnBnClickedBtlinexN)
	ON_BN_CLICKED(IDC_BtSTOP, &CDELTA_ControllerDlg::OnBnClickedBtstop)
	ON_COMMAND(ID_Help_About, &CDELTA_ControllerDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_BtHANDZERO, &CDELTA_ControllerDlg::OnBnClickedBthandzero)
	ON_BN_CLICKED(IDC_BUTTON2, &CDELTA_ControllerDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VEL, &CDELTA_ControllerDlg::OnNMCustomdrawSliderVel)
	ON_BN_CLICKED(IDC_BtAUTOZERO, &CDELTA_ControllerDlg::OnBnClickedBtautozero)
	ON_BN_CLICKED(IDC_BtJog1_P, &CDELTA_ControllerDlg::OnBnClickedBtjog1P)
	ON_BN_CLICKED(IDC_BtJog1_N, &CDELTA_ControllerDlg::OnBnClickedBtjog1N)
	ON_BN_CLICKED(IDC_BtJog2_P, &CDELTA_ControllerDlg::OnBnClickedBtjog2P)
	ON_BN_CLICKED(IDC_BtJog2_N, &CDELTA_ControllerDlg::OnBnClickedBtjog2N)
	ON_BN_CLICKED(IDC_BtJog3_P, &CDELTA_ControllerDlg::OnBnClickedBtjog3P)
	ON_BN_CLICKED(IDC_BtJog3_N, &CDELTA_ControllerDlg::OnBnClickedBtjog3N)
	ON_BN_CLICKED(IDC_BUTTON3, &CDELTA_ControllerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BtStopOff, &CDELTA_ControllerDlg::OnBnClickedBtstopoff)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CDELTA_ControllerDlg::OnBnClickedButton4)
	ON_COMMAND(ID_Exhibition, &CDELTA_ControllerDlg::OnExhibition)
	ON_COMMAND(ID_Conveyor, &CDELTA_ControllerDlg::OnConveyor)
	ON_EN_CHANGE(IDC_EtEDITPOSITION_X, &CDELTA_ControllerDlg::OnEnChangeEteditpositionX)
	ON_COMMAND(ID_XYPLATEFORM_MONITOR, &CDELTA_ControllerDlg::OnXyplateformMonitor)
END_MESSAGE_MAP()


// Ctest2Dlg message handlers

BOOL CDELTA_ControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_Menu.LoadMenu(IDR_MENU1);  
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED|SWP_DRAWFRAME);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_iDELTADLg = this;

	UpdateData(FALSE);

	//��ʼ��ָ�����
	pES = NULL;
	pCT = NULL;

	//�����ٶȻ����ĳ�ʼ״̬
	OnMotion = TRUE;
	m_sliderVel.SetRange(2, 100);
	m_sliderVel.SetTicFreq(1);
	m_sliderVel.SetPageSize(5);
	m_sliderVel.SetPos(20);
	m_Int=m_sliderVel.GetPos();
	CString str = _T("30%");
	SetDlgItemText(IDC_STATIC_VEL, str);
//	HICON m_hicn1 = AfxGetApp()->LoadIcon(IDI_ESTOP);
//	m_EStop.SetIcon(m_hicn1);

	//��ʼ����������������
	m_pResultList=(CListBox*)GetDlgItem(IDC_DISPLAY);
	if (m_gtsmotion.MotionInit())
	{
		int index = m_pResultList->InsertString (-1,_T(">>ͨѶ�ɹ�����ʼ���ɹ�"));
		m_pResultList->SetCurSel(index);
		OnBnClickedBthandzero();
		//�����ؽڼ��
		OnetimeFlag = TRUE;
		SetTimer(1,50,NULL);
	}
	else
	{
		int index = m_pResultList->InsertString(-1,_T(">>ͨѶʧ�ܣ���ʼ��ʧ��"));
		m_pResultList->SetCurSel(index);
		extern double wayPoint2[];   //·����ؽ�ת����������
		extern long T_gap;         //����ʱ����
		extern int n;                 //����
		int flag;
		Trajectory tratest;
		CString strtest;

		/*flag = zerokine.IKine(P1,P0);
		flag = zerokine.FKine(P0,P1);*/
		//flag = tratest.StraightLine(P0,P1);
		//flag = gtsmotion.Pvt_CompleteLoop(P0,P1,1,0);
		/*strtest.Format(_T("%d"),flag);
		index = m_pResultList->AddString(strtest);
		m_pResultList->SetCurSel(index);*/
	}

	pXYPlatform->Create(IDD_XYDELTADLG);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDELTA_ControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDELTA_ControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDELTA_ControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDELTA_ControllerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDELTA_ControllerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}



void CDELTA_ControllerDlg::OnSetStrokelimit()
{
	// TODO: Add your command handler code here
	m_iStrLimDlg.DoModal();
}

void CDELTA_ControllerDlg::OnMonitorStart()
{
	// TODO: Add your command handler code here
	m_iMonitor.DoModal();
}

void CDELTA_ControllerDlg::OnExhibition()
{
	// TODO: Add your command handler code here
	//���ó�Ա��������һ����ģ̬�Ի���
	if(pES)delete pES;
	pES = new CExhibiton();   //��ָ������ڴ�
	pES->Create(IDD_Exhibition_Show); //����һ����ģ̬�Ի���
	pES->ShowWindow(SW_SHOWNORMAL);  //��ʾ��ģ̬�Ի���
}

void CDELTA_ControllerDlg::OnBnClickedBtstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_gtsmotion.SmoothStop())
	{
		int index = m_pResultList->InsertString(-1, _T(">>ֹͣ�˶�"));
		m_pResultList->SetCurSel(index);
	}	
}


void CDELTA_ControllerDlg::OnHelpAbout()
{
	// TODO: �ڴ���������������
	dlgAbout.DoModal();
}

void CDELTA_ControllerDlg::OnNMCustomdrawSliderVel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_Int=m_sliderVel.GetPos();
	UpdateData(FALSE);
	CString str;
	str.Format(_T("%d"),m_Int);
	str = str+'%';
	SetDlgItemText(IDC_STATIC_VEL, str);
	*pResult = 0;
}

	double ovalue[3];    //��������λ����������λ������
	double zeroposAng[3];//�����λ�Ĺؽڽ�
	double P0[3],P1[3];    //�߶�������յ�

void CDELTA_ControllerDlg::OnBnClickedBthandzero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL gtsflag;
	int zeroflag;
	gtsflag = gtsmotion.SetZero();

	UpdateData(TRUE);

	//����㡢�յ��ʼ��Ϊ���
	P1[0] = P0[0] = ovalue[0] = m_Zero_X;
	P1[1] = P0[1] = ovalue[1] = m_Zero_Y;
	P1[2] = P0[2] = ovalue[2] = m_Zero_Z;

	zeroflag = zerokine.IKine(ovalue,zeroposAng);
}


void CDELTA_ControllerDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	short flag;
	double tranp0[3];
	short int velrate;

	UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡĿ��λ��ֵ
	gtsmotion.GetCurPos(tranp0);  //�õ���ǰ����ֵ
	velrate = m_sliderVel.GetPos();

	//��ȡĿ��ֵ
	P1[0] = m_Pos_X;
	P1[1] = m_Pos_Y;
	P1[2] = m_Pos_Z;

	P0[0] = tranp0[0];
	P0[1] = tranp0[1];
	P0[2] = tranp0[2];

	/*flag = gtsmotion.Pvt_CompleteLoop(P0,P1,m_Int,m_LoopTimes);*/
	flag = gtsmotion.Pvt_DynamicPT(P0,P1,1,velrate);

	if (flag == 0)
	{
		int index = m_pResultList->InsertString (-1,_T(">>���������ռ䣬����������"));
		m_pResultList->SetCurSel(index);
	}
}



void CDELTA_ControllerDlg::OnBnClickedBtautozero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	short flag;
	extern double ovalue[];
	double tranp0[3];
	short int velrate;

	UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡĿ��λ��ֵ
	gtsmotion.GetCurPos(tranp0);  //�õ���ǰ����ֵ
	velrate = m_sliderVel.GetPos();

	//��ȡĿ��ֵ
	P1[0] = ovalue[0];
	P1[1] = ovalue[1];
	P1[2] = ovalue[2];

	P0[0] = tranp0[0];
	P0[1] = tranp0[1];
	P0[2] = tranp0[2];

	/*flag = gtsmotion.Pvt_CompleteLoop(P0,P1,m_Int,1);*/
	flag = gtsmotion.Pvt_DynamicPT(P0,P1,1,velrate);

	if (flag == 0)
	{
		int index = m_pResultList->InsertString (-1,_T(">>���������ռ�"));
		m_pResultList->SetCurSel(index);
	}
}


void CDELTA_ControllerDlg::OnBnClickedBtjog1P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_X,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog1N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_X,'N');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog2P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Y,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog2N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Y,'N');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog3P()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Z,'P');
}


void CDELTA_ControllerDlg::OnBnClickedBtjog3N()
{
	// TODO: Add your control notification handler code here
	gtsmotion.Jog_Test(AXIS_Z,'N');
}


void CDELTA_ControllerDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_gtsmotion.Ena_Stop('E');
}


void CDELTA_ControllerDlg::OnBnClickedBtstopoff()
{
	// TODO: Add your control notification handler code here
	m_gtsmotion.Ena_Stop('S');
}


void CDELTA_ControllerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_gtsmotion.Ena_Stop('C');
	CDialogEx::OnClose();
}


void CDELTA_ControllerDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);         //ˢ�¶Ի��򣬻�ȡѭ������

	extern long T_gap;         //����ʱ����
	extern int n;                 //����
	for (int i=0;i<m_LoopTimes;i++)
	{
		OnBnClickedButton2();
		OnBnClickedBtautozero();
	}
}


void CAboutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
	delete this;
}


void CDELTA_ControllerDlg::OnConveyor()
{
	// TODO: �ڴ���������������
	//���ó�Ա��������һ����ģ̬�Ի���
	if(pCT)delete pCT;
	pCT = new CConveyor();   //��ָ������ڴ�
	pCT->Create(IDD_Conveyor1); //����һ����ģ̬�Ի���
	pCT->ShowWindow(SW_SHOWNORMAL);  //��ʾ��ģ̬�Ի���
}


void CDELTA_ControllerDlg::OnEnChangeEteditpositionX()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDELTA_ControllerDlg::OnXyplateformMonitor()
{
	// TODO:  �ڴ���������������
	pXYPlatform->ShowWindow(SW_SHOWNORMAL);
}
