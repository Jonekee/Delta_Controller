#pragma once

#include "Kine.h"

class Trajectory
{
public:
	Trajectory(void);
	~Trajectory(void);
private:
	double est_sp;            //�ٶ�
	double est_acc;           //���ٶ�
	double est_jerk;          //�Ӽ��ٶ�
	Kine kine;   //ʵ�����˶�ѧ

	double MaxJoint(double Angles[]);
	double Time_Line(IN double POS_0[3],IN double POS_1[3],IN short vel);
	double Dis_Line(IN double POS_0[3],IN double POS_1[3]);
public:
	void Set_est(IN double est_values[3]);      //��ʼ���ٶȡ����ٶȡ��Ӽ��ٶ�
	int StraightLine(IN double POS_0[3],IN double POS_1[3],IN short vel);
	int EllipseLine(IN double POS_0[3],IN double POS_1[3],IN short vel);
	int LameLine(IN double POS_0[3],IN DOUBLE POS_1[3],IN short vel);

	//double MaxJoint(double Angles[3]);
};

