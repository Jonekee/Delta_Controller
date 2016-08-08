#pragma once

#include "Robot.h"
///////////////////////////////////////////////////////////
/////////Kine window

//λ�˾���
struct MtxKine
{
public:
	double R11;
	double R12;
	double R13;
	double R21;
	double R22;
	double R23;
	double R31;
	double R32;
	double R33;

	double X;
	double Y;
	double Z;
};

//TCP����
struct MtxTcp
{
	double K1;
	double K2;
	double K3;
	double M1;
	double M2;
	double M3;
	double N1;
	double N2;
	double N3;

	double u1;
	double u2;
	double u3;
};

/************************************************************************
 * �˶�ѧ����                                                             
 ************************************************************************/
class Kine:public CEdit
{
	//Construction
public:
	Kine();

	//Attributes
protected:
	//inverse kinematics helper functions,calculates angle theta1 for YZ-pane
	bool calcAngleYZ(float x0, float y0, float z0, double *theta);

public:
	//Operations
public:

	//Implementation
public:
	void Robot_IncreTransTool(IN double* currpos, IN double* increpos, OUT double* outpos);
	void Mtx_Multiply(MtxKine* input, MtxKine* middle, MtxKine* output, int inv);
	void Trans_PosToMtx(double* pos, MtxKine* output, int inv);
	void Trans_MtxToPos(MtxKine* input, double* outpos);

	/************************************************************************
	 * ������IKine()                                                          
	 * ���ܣ����
	 *
	 * ���룺double* gdCPos  - λ�����飬��x,y,x)
	 * �����double* gdJPos  - �ؽڽ�����
	 * ���أ�int - 0�ɹ�����������
	 ************************************************************************/
	int IKine(IN double gdCPos[3],OUT double gdJPos[3]);

	/************************************************************************
	 * ������FKine()                                                          
	 * ���ܣ�����
	 *
	 * ���룺double* gdJPos  - �ؽ�ת��  ������ת�ǣ�
	 * �����double* gdJPos  - ����λ�ˣ���x,y,z)
	 * ���أ�int - 0�ɹ�����������
	 ************************************************************************/
	int FKine(IN double gdJPos[3],OUT double gdCPos[6]);

	virtual ~Kine();
};