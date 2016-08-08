#include "StdAfx.h"
#include "Trajectory.h"
#include <cmath>

double wayPoint1[1024] = {0};   //·����ؽ�ת������
double wayPoint2[1024] = {0};
double wayPoint3[1024] = {0};
long T_gap = 40;         //����ʱ����
int n = 0;                 //����

Trajectory::Trajectory(void)
{
}


Trajectory::~Trajectory(void)
{
}

double Trajectory::MaxJoint(double Angles[])
{
	double temp = 0.0;

	temp = Angles[1]>=Angles[0]? Angles[1]:Angles[0];
	temp = Angles[2]>=temp? Angles[2]:temp;

	return temp;
}

double Trajectory::Dis_Line(IN double POS_0[3],IN double POS_1[3])
{
	double dis_all = sqrt(pow(POS_0[0]-POS_1[0],2)+pow(POS_0[1]-POS_1[1],2)+pow(POS_0[2]-POS_1[2],2));
	return dis_all;
}

double Trajectory::Time_Line(IN double POS_0[3],IN double POS_1[3],IN short vel)
{
	int flag;
	double end_temp[3]={0}; //�岹��
	double calPos[3];
	double maxjoint;
	double T_all;

	flag = kine.IKine(POS_1,end_temp);
	calPos[0] = end_temp[0];
	calPos[1] = end_temp[1];
	calPos[2] = end_temp[2];

	flag = kine.IKine(POS_0,end_temp);
	calPos[0] = fabs(calPos[0]-end_temp[0]);
	calPos[1] = fabs(calPos[1]-end_temp[1]);
	calPos[2] = fabs(calPos[2]-end_temp[2]);

	maxjoint = MaxJoint(calPos);
	T_all = (maxjoint*PULSE*RIGIDRATIO/ANGLE)/vel;

	return T_all;
}

void Trajectory::Set_est(IN double est_values[3])
{
	est_sp = est_values[0];
	est_acc = est_values[1];
	est_jerk = est_values[2];
}

int Trajectory::StraightLine(IN double POS_0[],IN double POS_1[],IN short vel)
{
	int flag;
	double end_temp[3]={0}; //�岹��
	double end_POS[3];
	double T_all;
	double dis_all;
	double t_pos;
	double dis_pos;

	T_all = Time_Line(POS_0,POS_1,vel);//��ʱ��
	double T_test = T_all*2;
	double TIMES_test = 60*1000/(int)T_test;
	dis_all = Dis_Line(POS_0,POS_1);//ֱ�߳���
	n = (int)(dis_all+0.5);//Ԥ�����
	T_gap = (long)(T_all/n+1);//����ʱ����
	n = (int)((T_all/(T_gap*(double)n))*n);//��������

	for (int i=0;i<=n;i++)//����·��ؽڽ�
	{
		t_pos = (double)i/(double)n;
		dis_pos = -10*pow(t_pos,6)+36*pow(t_pos,5)-45*pow(t_pos,4)+20*pow(t_pos,3);

		////�岹������(3-4-5-6�ζ���ʽ�ָ
		end_POS[0] = POS_0[0]+(-POS_0[0]+POS_1[0])*dis_pos;
		end_POS[1] = POS_0[1]+(-POS_0[1]+POS_1[1])*dis_pos;
		end_POS[2] = POS_0[2]+(-POS_0[2]+POS_1[2])*dis_pos;

		//�������ؽ�ת��
		flag = kine.IKine(end_POS,end_temp);if (flag==0)return 0;

		//�����򼰸���·�㸳ֵ
		wayPoint1[i] = end_temp[0];	wayPoint2[i] = end_temp[1];wayPoint3[i] = end_temp[2];
	}
	return 1;
}

//��Բ���߹켣����
int Trajectory::EllipseLine(IN double POS_0[AXIS_Z],IN double POS_1[AXIS_Z],IN short vel)
{
	double a,b = 50;  //�����ᣬ�̰���
	double proz;                    //ӳ�䵽ƽ�������z��
	double interpoint[3];          //�岹������ֵ
	double end_temp[3];            //�岹���Ӧ�Ĺؽ�ת��
	double calPos[3]={0};              //�߹��Ĺؽ�������
	double dis_SLine,dis_Eline;               //ֱ�߳��ȡ���Բ����
	double T1,T2,T_all;                  //��һ���ι滮ʱ��
	double t_pos,dis_pos;
	double AK,XY_ratio;
	int flag;              //�������ر�־

	a = Dis_Line(POS_0,POS_1)/2;//���㳤���᳤��
	calPos[0] = (POS_0[0]+POS_1[0])/2;//��Բ�м������
	calPos[1] = (POS_0[1]+POS_1[1])/2;
	calPos[2] = (POS_0[2]+POS_1[2])/2-b;

	dis_Eline = PI*(3*(a+b)-sqrt((3*a+b)*(a+3*b)))/4;  //1/4��Բ�ܳ�
	dis_SLine = Dis_Line(POS_0,calPos);//ֱ�߳���

	T1 = Time_Line(POS_0,calPos,vel)*(dis_Eline/dis_SLine);//����ʱ���ֵ
	T2 = Time_Line(calPos,POS_1,vel)*(dis_Eline/dis_SLine);
	T_all = T1+T2;//��ʱ��
	double T_test = T_all*2;
	double TIMES_test = 60*1000/(int)T_test;
	n = (int)(dis_Eline*2+0.5);//Ԥ�����
	T_gap = (long)(T_all/n+1);
	n = (int)((T_all/(T_gap*(double)n))*n);

	//����岹��Ĺؽڿռ�ֵ
	for (int i=0;i<=n;i++)
	{
		t_pos = (double)i/(double)n;
		dis_pos = -10*pow(t_pos,6)+36*pow(t_pos,5)-45*pow(t_pos,4)+20*pow(t_pos,3);

		//AK = (180 - 180*(double)i/(double)n)*PI_RAD;         //����岹��Ļ���ֵ
		AK = (180 - 180*dis_pos)*PI_RAD;         //����岹��Ļ���ֵ
		proz = b*sin(AK);		//����岹��ĸ߶�ֵ
		XY_ratio = (cos(AK)+1)/2;           //����岹����XYƽ���ڵı���

		interpoint[AXIS_X-1] = POS_0[AXIS_X-1]+(POS_1[AXIS_X-1]-POS_0[AXIS_X-1])*XY_ratio;       //����岹����ȫ������ϵ�е�����ֵ
		interpoint[AXIS_Y-1] = POS_0[AXIS_Y-1]+(POS_1[AXIS_Y-1]-POS_0[AXIS_Y-1])*XY_ratio;
		interpoint[AXIS_Z-1] = POS_0[AXIS_Z-1]-proz;

		flag = kine.IKine(interpoint,end_temp);if (flag==0) return 0;

		//��ֵ��ȫ�ֱ�������Ϊ����ֵ
		wayPoint1[i] = end_temp[0];	wayPoint2[i] = end_temp[1];wayPoint3[i] = end_temp[2];
	}
	return 1;
}

int Trajectory::LameLine(IN double POS_0[3],IN DOUBLE POS_1[3],IN short vel)
{
	int flag;
	double dis_SLine,h1,h2,e=25,d;//ʼĩ��ֱ�߾��롢��ʼֱ�߶Ρ�ĩ��ֱ�ߡ�Lame���߸߶ȡ�����
	double dis_lame,dis_all;//Lame���߳��ȡ�·���ܾ���
	double tan_temp,f[3],g[3],calPos[3];//�м����ֵ
	double T_all;
	double t_pos,rate_pos,dis_pos;//·��ʱ�������λ�Ʊ�����·��λ����
	double interpoint[3],end_temp[3];//�岹������������Ӧ�ؽ���

	if (POS_0[2]<POS_1[2])//��ʼ���ĩ���
	{
		h1 = 0;h2 = POS_1[2]-POS_0[2];
		calPos[0] = POS_1[0];calPos[1] = POS_1[1];calPos[2] = POS_0[2];//ƽ��·����ĩ���Ϸ�
		d = Dis_Line(POS_0,calPos)/2;
	} 
	else//��ʼ���ĩ���
	{
		h2 = 0;h1 = POS_0[2]-POS_1[2];
		calPos[0] = POS_0[0];calPos[1] = POS_0[1];calPos[2] = POS_1[2];//ƽ��·���ڳ�ʼ���Ϸ�
		d = Dis_Line(POS_1,calPos)/2;
	}
	for (int i=0;i<3;i++)//lame���߳���simpson���ֹ�ʽ����
	{
		tan_temp = tan(i*PI/8);
		f[i] = ((1+pow(tan_temp,2))*sqrtf(pow(d,2)*pow(tan_temp,4)+pow(e,2)))/pow(1+pow(tan_temp,3),4/3);
		g[i] = ((1+pow(tan_temp,2))*sqrtf(pow(e,2)*pow(tan_temp,4)+pow(d,2)))/pow(1+pow(tan_temp,3),4/3);
	}
	dis_lame = (PI/24)*(f[0]+g[0]+4*(f[1]+g[1])+f[2]+g[2]);
	dis_all = h1+2*dis_lame+h2;
	dis_SLine = Dis_Line(POS_0,POS_1);

	T_all = Time_Line(POS_0,POS_1,vel)*dis_all/dis_SLine;
	n = (int)(dis_all+0.5);//Ԥ�����
	T_gap = (long)(T_all/n+1);//����ʱ����
	n = (int)((T_all/(T_gap*(double)n))*n);//��������

	//����岹��Ĺؽڿռ�ֵ
	for (int i=0;i<=n;i++)
	{
		t_pos = (double)i/(double)n;
		rate_pos = -10*pow(t_pos,6)+36*pow(t_pos,5)-45*pow(t_pos,4)+20*pow(t_pos,3);
		dis_pos = dis_all*rate_pos;
		
		if (dis_pos<=h1){interpoint[0] = POS_0[0];interpoint[1] = POS_0[1];interpoint[2] = POS_0[2]-dis_pos;}
		else if (dis_pos<=h1+dis_lame)
		{
			double v_init = dis_pos-h1;
		}
		else if (dis_pos<=h1+2*dis_lame)
		{
		}
		else{interpoint[0] = POS_1[0];interpoint[1] = POS_1[1];interpoint[2] = POS_1[2]+dis_pos-dis_all;}

		flag = kine.IKine(interpoint,end_temp);
		if (flag==0) return 0;

		//��ֵ��ȫ�ֱ�������Ϊ����ֵ
		wayPoint1[i] = end_temp[0];	wayPoint2[i] = end_temp[1];wayPoint3[i] = end_temp[2];
	}

	return 1;
}