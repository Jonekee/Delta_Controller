#include "StdAfx.h"
#include <math.h>
#include "GtsMotion.h"



GtsMotion::GtsMotion(void)
{}

GtsMotion::~GtsMotion(void)
{}

bool GtsMotion::MotionInit()
{
	short sRtn;
	sRtn = GT_Open();  // ���˶�������
	// ��λ������
	sRtn += GT_Reset();   
	// ���ø���Ϊģ�������(�ջ�)ģʽ
	/*sRtn += GT_CtrlMode(AXIS_X, 0);
	sRtn += GT_CtrlMode(AXIS_Y, 0);
	sRtn += GT_CtrlMode(AXIS_Z, 0);*/

	// ���ø���Ϊ���������(����)ģʽ
	/*sRtn += GT_CtrlMode(AXIS_X, 1);
	sRtn += GT_CtrlMode(AXIS_Y, 1);
	sRtn += GT_CtrlMode(AXIS_Z, 1);
*/
	// �����˶�������
	// ע�⣺�����ļ�ȡ���˸���ı�������λ
	sRtn += GT_LoadConfig("pulse_dir_5.cfg");
	Sleep(100);
	// �������ı�������λ
	sRtn += GT_ClrSts(AXIS_X, AXIS_Z);     
	//���ø���PID����
	TPid pid;
	//��ȡ����PID����
	sRtn = GT_GetPid(AXIS_X,1,&pid);
	sRtn = GT_GetPid(AXIS_Y,1,&pid);
	sRtn = GT_GetPid(AXIS_Z,1,&pid);

	pid.kp=5;
	pid.kd=5;
	pid.kvff=3000;
	pid.derivativeLimit=0;
	pid.integralLimit=32767;
	pid.limit=32767;

	//���¸����PID����
	sRtn += GT_SetPid(AXIS_X, 1, &pid);
	sRtn += GT_SetPid(AXIS_Y, 1, &pid);
	sRtn += GT_SetPid(AXIS_Z, 1, &pid); 

	//λ������
	sRtn += GT_ZeroPos(AXIS_X,AXIS_Z);
	sRtn += GT_SetEncPos(AXIS_X,0);
	sRtn += GT_SetEncPos(AXIS_Y,0);
	sRtn += GT_SetEncPos(AXIS_Z,0);

	//���õ�λ����
	sRtn = GT_SetAxisBand(AXIS_X,2,20);
	sRtn = GT_SetAxisBand(AXIS_Y,2,20);
	sRtn = GT_SetAxisBand(AXIS_Z,2,20);

	//����ƽ���ͽ���ֹͣ���ٶ�
	sRtn += GT_SetStopDec(AXIS_X,SmoothStopDec,EstopDec);
	sRtn += GT_SetStopDec(AXIS_Y,SmoothStopDec,EstopDec);
	sRtn += GT_SetStopDec(AXIS_Z,SmoothStopDec,EstopDec);

	//����������λ
	sRtn += GT_SetSoftLimit(AXIS_X,150000,-50000);
	sRtn += GT_SetSoftLimit(AXIS_Y,150000,-50000);
	sRtn += GT_SetSoftLimit(AXIS_Z,150000,-50000);
	if (sRtn == 0)
	{
		waitMotorFlag = true;
		return TRUE;
	}
	else
		return FALSE;
}

bool GtsMotion::SetZero()
{
	//λ������
	short sRtn;
	sRtn = GT_ZeroPos(AXIS_X,AXIS_Z);  //�滮������
	sRtn = GT_SetEncPos(AXIS_X,0);//����������
	sRtn += GT_SetEncPos(AXIS_Y,0);
	sRtn += GT_SetEncPos(AXIS_Z,0);

	if (sRtn == 0)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void GtsMotion::WaitMotor()
{
	//�ȴ�����˶���λ����������
	//waitMotorFlag = false;
	do 
	{
		sRtn = GT_GetSts(AXIS_X,&sts,3);
	} while (0x800!=(sts & 0x800));
	//waitMotorFlag = true;
}

bool GtsMotion::GetCurPos(double* curpos)
{
	double pVal[AXIS_Z];
	double curAng[3];
	double curcord[3];
	extern double zeroposAng[];
	int flag;

	WaitMotor();//�ȴ�����˶���λ����������
	sRtn = GT_GetEncPos(AXIS_X,pVal,AXIS_Z);//��ȡAXIS��ı�����λ��

	//��ȡ�ؽ�ת�ǣ���ת���Ƕȼ�����λʱ��ĽǶ�
	curAng[0] = (((pVal[AXIS_X-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[0];
	curAng[1] = (((pVal[AXIS_Y-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[1];
	curAng[2] = (((pVal[AXIS_Z-1]/PULSE)/RIGIDRATIO)*ANGLE)+zeroposAng[2];

	//�����˶�ѧ����õ�����λ��
	flag = kine.FKine(curAng,curcord);
	curpos[0] = curcord[0];//��������ֵ
	curpos[1] = curcord[1];
	curpos[2] = curcord[2];
	return true;
}
bool GtsMotion::GetConveyorPos(OUT double* pos)
{
	return true;
}

bool GtsMotion::ConCalibrate()
{
	return true;
}

bool GtsMotion::VisCalibrate()
{
	return true;
}

short GtsMotion::Compensation(IN double POS[3],IN short EndPostype)
{
	
	return 0;
}

short GtsMotion::P2P_COM(IN double POS[3])
{
	TTrapPrm trap;
	double prfPos;

	double testpos[3];
	testpos[0] = POS[AXIS_X-1];
	testpos[0] = POS[AXIS_Y-1];
	testpos[0] = POS[AXIS_Z-1];

	sRtn = GT_PrfTrap(AXIS_X);   //����Ӧ����Ϊ��λģʽ
	sRtn = GT_PrfTrap(AXIS_Y); 
	sRtn = GT_PrfTrap(AXIS_Z); 

	sRtn = GT_GetTrapPrm(AXIS_X,&trap);   //��ȡ��λ�˶�����
	sRtn = GT_GetTrapPrm(AXIS_Y,&trap);
	sRtn = GT_GetTrapPrm(AXIS_Z,&trap);

	trap.acc = 10;
	trap.dec = 10;
	trap.smoothTime = 2;

	//���õ�λ�˶�����
	sRtn = GT_SetTrapPrm(AXIS_X,&trap);
	sRtn = GT_SetTrapPrm(AXIS_Y,&trap);
	sRtn = GT_SetTrapPrm(AXIS_Z,&trap);

	//�������Ŀ��λ��
	sRtn = GT_SetPos(AXIS_X,POS[AXIS_X-1]);
	sRtn = GT_SetPos(AXIS_Y,POS[AXIS_Y-1]);
	sRtn = GT_SetPos(AXIS_Z,POS[AXIS_Z-1]);

	//�������Ŀ���ٶ�
	sRtn = GT_SetVel(AXIS_X,50);
	sRtn = GT_SetVel(AXIS_Y,50);
	sRtn = GT_SetVel(AXIS_Z,50);

	//����AXIS����˶�
	sRtn = GT_Update(1<<(AXIS_X-1));
	sRtn = GT_Update(1<<(AXIS_Y-1));
	sRtn = GT_Update(1<<(AXIS_Z-1));

	return 0;
}

bool GtsMotion::SmoothStop()
{
	sRtn=GT_Stop(15,0);//���������ֹͣ
	if (sRtn == 0)
	{
		return TRUE;
	}
	else
		return FALSE;
}

short GtsMotion::Pvt_CompleteLoop(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum)
{
	long mask = 0;
	int flag;        //�켣�滮��������ֵ
	double time[1024];
	extern double wayPoint1[];   //·����ؽ�ת����������
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //����ʱ����
	extern int  n;                 //����
	short table_x = TABLEX;
	short table_y = TABLEY;
	short table_z = TABLEZ;
	extern double ovalue[];    //��������λ����������λ������
	extern double zeroposAng[];//�����λ�Ĺؽڽ�

	Trajectory traline;
	for (int i=0;i<3;i++)        //ÿ������λ�ü��ϳ�ʼ��λ����
	{
		Point_0[i] = Point_0[i]+ovalue[i];
		Point_1[i] = Point_1[i]+ovalue[i];
	}
	flag = traline.StraightLine(Point_0,Point_1,VelRatio);

	for (int i=0;i<=2*n;i++)     
	{
		time[i] = i*(T_gap*100/VelRatio);
		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE; //ÿ������ֵ��ȥ��ʼ��λ�ؽڽ�
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE; //�ٳ������嵱��������������
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;
	}


	//A1���м����ֵ
	double a1[1024],b1[1024],c1[1024];
	//A2���м����ֵ
	double a2[1024],b2[1024],c2[1024];
	//A3���м����ֵ
	double a3[1024],b3[1024],c3[1024];

	//����ΪPVTģʽ
	sRtn = GT_PrfPvt(AXIS_X);
	sRtn = GT_PrfPvt(AXIS_Y);
	sRtn = GT_PrfPvt(AXIS_Z);

	//��������
	sRtn = GT_PvtTableComplete(table_x,2*n,&time[0],&wayPoint1[0],&a1[0],&b1[0],&c1[0],0,0);
	sRtn = GT_PvtTableComplete(table_y,2*n,&time[0],&wayPoint2[0],&a2[0],&b2[0],&c2[0],0,0);
	sRtn = GT_PvtTableComplete(table_z,2*n,&time[0],&wayPoint3[0],&a3[0],&b3[0],&c3[0],0,0);

	// ѡ�����ݱ�
	sRtn = GT_PvtTableSelect(AXIS_X, table_x); 
	sRtn = GT_PvtTableSelect(AXIS_Y, table_y); 
	sRtn = GT_PvtTableSelect(AXIS_Z, table_z); 

	//����Ϊѭ��ִ��
	sRtn = GT_SetPvtLoop(AXIS_X,LoopNum);
	sRtn = GT_SetPvtLoop(AXIS_Y,LoopNum);
	sRtn = GT_SetPvtLoop(AXIS_Z,LoopNum);

	//�����˶����ƿ�
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PvtStart(mask);

	if (sRtn == 0)
	{
		return FALSE;
	}
	else{return TRUE;}
}

short GtsMotion::Pvt_PointToPoint(IN double Point_0[3],IN double Point_1[3],IN double VelRatio,IN int LoopNum)
{
	long mask;
	int flag;        //�켣�滮��������ֵ
	double time[1024];
	extern double wayPoint1[];   //·����ؽ�ת����������
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //����ʱ����
	extern int  n;                 //����
	short table_x = TABLEX;
	short table_y = TABLEY;
	short table_z = TABLEZ;
	extern double ovalue[];    //��������λ����������λ������
	extern double zeroposAng[];//�����λ�Ĺؽڽ�

	Trajectory traline;
	for (int i=0;i<3;i++)        //ÿ������λ�ü��ϳ�ʼ��λ����
	{
		Point_0[i] = Point_0[i]+ovalue[i];
		Point_1[i] = Point_1[i]+ovalue[i];
	}
	flag = traline.StraightLine(Point_0,Point_1,VelRatio);

	for (int i=0;i<=n;i++)     
	{
		time[i] = i*T_gap;
		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE; //ÿ������ֵ��ȥ��ʼ��λ�ؽڽ�
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE; //�ٳ������嵱��������������
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;
	}

	double a1[1024],b1[1024],c1[1024];//A1���м����ֵ
	double a2[1024],b2[1024],c2[1024];//A2���м����ֵ
	double a3[1024],b3[1024],c3[1024];//A3���м����ֵ

	//����ΪPVTģʽ
	sRtn = GT_PrfPvt(AXIS_X);
	sRtn = GT_PrfPvt(AXIS_Y);
	sRtn = GT_PrfPvt(AXIS_Z);

	//��������
	sRtn = GT_PvtTableComplete(table_x,n,&time[0],&wayPoint1[0],&a1[0],&b1[0],&c1[0],0,0);
	sRtn = GT_PvtTableComplete(table_y,n,&time[0],&wayPoint2[0],&a2[0],&b2[0],&c2[0],0,0);
	sRtn = GT_PvtTableComplete(table_z,n,&time[0],&wayPoint3[0],&a3[0],&b3[0],&c3[0],0,0);

	// ѡ�����ݱ�
	sRtn = GT_PvtTableSelect(AXIS_X, table_x); 
	sRtn = GT_PvtTableSelect(AXIS_Y, table_y); 
	sRtn = GT_PvtTableSelect(AXIS_Z, table_z); 

	//����Ϊѭ��ִ��
	sRtn = GT_SetPvtLoop(AXIS_X,1);
	sRtn = GT_SetPvtLoop(AXIS_Y,1);
	sRtn = GT_SetPvtLoop(AXIS_Z,1);

	//�����˶����ƿ�
	mask = 1<<(AXIS_X-1);
	mask = 1<<(AXIS_Y-1);
	mask = 1<<(AXIS_Z-1);
	sRtn = GT_PvtStart(mask);

	if (sRtn == 0)
	{
		return FALSE;
	}
	else{return TRUE;}
}

short GtsMotion::Jog_Test(short Axis,char dir)
{
	TJogPrm jog;
	double prfPos,prfVel;

	sRtn = GT_PrfJog(Axis);    //��Axis������ΪJogģʽ
	sRtn = GT_GetJogPrm(Axis,&jog); //��ȡJog�˶�����
	jog.acc = 0.0625;
	jog.dec = 0.0625;

	sRtn = GT_SetJogPrm(Axis,&jog); //����Jog�˶�����
	if (dir == 'P') sRtn = GT_SetVel(Axis,5);
	else sRtn = GT_SetVel(Axis,-5);
	sRtn = GT_Update(1<<(Axis-1));

	if (sRtn == 0)
	{
		return 0;
	}
	else return 1;
}

void GtsMotion::Ena_Stop(char index)
{
	switch (index)
	{
	case 'E'://������ʹ��
		sRtn += GT_AxisOn(AXIS_X);
		sRtn += GT_AxisOn(AXIS_Y);
		sRtn += GT_AxisOn(AXIS_Z);
		break;
	case 'S'://������ʹ�ܹر�
		sRtn += GT_AxisOff(AXIS_X);
		sRtn += GT_AxisOff(AXIS_Y);
		sRtn += GT_AxisOff(AXIS_Z);
		break;
	case 'C'://������ʹ�ܹر�
		sRtn += GT_AxisOff(AXIS_X);
		sRtn += GT_AxisOff(AXIS_Y);
		sRtn += GT_AxisOff(AXIS_Z);
		GT_Close();  //�ر��˶����ƿ�
		break;
	}
}

short GtsMotion::Pvt_DynamicPT(IN double Point_0[3],IN double Point_1[3],IN short linetype,short velrate)
{
	short space;
	int flag;        //�켣�滮��������ֵ
	long time[1024]={0};
	short int vel = 0;
	extern double wayPoint1[];   //·����ؽ�ת����������
	extern double wayPoint2[];
	extern double wayPoint3[];
	extern long T_gap;         //����ʱ����
	extern int  n;                 //����
	extern double zeroposAng[];//�����λ�Ĺؽڽ�
	Trajectory traline;

	vel = 4*velrate;

	if(linetype==1)flag = traline.StraightLine(Point_0,Point_1,vel);
	else if(linetype==2)flag = traline.EllipseLine(Point_0,Point_1,vel);
	if (flag==0)return 0;

	double testzeroang[3][1024];   //�������������ֵ�Ƿ��д�
	double testpos[3];

	testpos[0] = wayPoint1[0];
	testpos[1] = wayPoint2[0];
	testpos[2] = wayPoint3[0];

	wayPoint1[0] = (wayPoint1[0]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE;
	wayPoint2[0] = (wayPoint2[0]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE;
	wayPoint3[0] = (wayPoint3[0]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE;

	for (int i=1;i<=n;i++)     
	{
		time[i] = (long)(i*T_gap);

		wayPoint1[i] = (wayPoint1[i]-zeroposAng[0])*PULSE*RIGIDRATIO/ANGLE-wayPoint1[0]; //ÿ������ֵ��ȥ��ʼ��λ�ؽڽ�
		wayPoint2[i] = (wayPoint2[i]-zeroposAng[1])*PULSE*RIGIDRATIO/ANGLE-wayPoint2[0]; //�ٳ������嵱��������������
		wayPoint3[i] = (wayPoint3[i]-zeroposAng[2])*PULSE*RIGIDRATIO/ANGLE-wayPoint3[0];

		testzeroang[0][i] = wayPoint1[i];
		testzeroang[1][i] = wayPoint2[i];
		testzeroang[2][i] = wayPoint3[i];
	}
	testzeroang[0][0] = wayPoint1[0];
	testzeroang[1][0] = wayPoint2[0];
	testzeroang[2][0] = wayPoint3[0];

	//��AXIS������ΪPT��̬ģʽ
	sRtn = GT_PrfPt(AXIS_X,PT_MODE_STATIC);
	sRtn = GT_PrfPt(AXIS_Y,PT_MODE_STATIC);
	sRtn = GT_PrfPt(AXIS_Z,PT_MODE_STATIC);

	//���AXIS���FIFO
	sRtn = GT_PtClear(AXIS_X);
	sRtn = GT_PtClear(AXIS_Y);
	sRtn = GT_PtClear(AXIS_Z);

	//����FIFO��С
	sRtn = GT_SetPtMemory(AXIS_X,1);
	sRtn = GT_SetPtMemory(AXIS_Y,1);
	sRtn = GT_SetPtMemory(AXIS_Z,1);

	for (int i=1;i<=n;i++)
	{
		sRtn = GT_PtSpace(AXIS_X,&space);
		if (space>0)
		{
				sRtn = GT_PtData(AXIS_X,wayPoint1[i],time[i]);
				sRtn = GT_PtData(AXIS_Y,wayPoint2[i],time[i]);
				sRtn = GT_PtData(AXIS_Z,wayPoint3[i],time[i]);
		}
		else break;
	}

	//�������
	//AfxBeginThread(_thread_Robot_Action, (void*)this);
	mask = 0;
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PtStart(mask);
	//WaitMotor();//�ȴ�����˶���λ����������
	return 1;
}

UINT __cdecl GtsMotion::_thread_Robot_Action(LPVOID pParam)
{
	GtsMotion *gts = (GtsMotion*)pParam;

	gts->threadFuctionRobotAction();
	return 0;
}

void GtsMotion::threadFuctionRobotAction()
{
	mask = 0;
	mask += 1<<(AXIS_X-1);
	mask += 1<<(AXIS_Y-1);
	mask += 1<<(AXIS_Z-1);
	sRtn = GT_PtStart(mask);
	WaitMotor();//�ȴ�����˶���λ����������
}