#pragma once

//�˳�
//���������λ
#define AXIS1LimitP 128*4*50*2500/360
#define AXIS1LimitN -128*4*50*2500/360
#define AXIS2LimitP 123*4*50*2500/360
#define AXIS2LimitN -123*4*50*2500/360
#define AXIS3LimitP 25*4*1.5*2500/16    //����4����Ϊ�ڵ��Թ����з���ʵ��λ����滮λ�ò�4��,�ڿ��ƿ��滮�������任�н��ѽ��滮�������������ı�����Ϊ1��4����λ��ҲҪ��Ӧ������4��
#define AXIS3LimitN -55*4*1.5*2500/16   //����4����Ϊ�ڵ��Թ����з���ʵ��λ����滮λ�ò�4�����ڿ��ƿ��滮�������任�н��ѽ��滮�������������ı�����Ϊ1��4����λ��ҲҪ��Ӧ������4��
#define AXIS4LimitP 180*4*3*2500/360
#define AXIS4LimitN -180*4*3*2500/360

//������
#define TABLEX 1
#define TABLEY 2
#define TABLEZ 3
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3

//���ƽ��ֹͣ�ͽ���ֹͣ���ٶ�
#define SmoothStopDec 100
#define EstopDec 1000

//////////////////////////////////////�˳�
#define SR 160.0      //��ƽ̨���Բ�뾶
#define Mr 50.0       //��ƽ̨���Բ�뾶
#define PL 200.0      //�����۳���
#define NL 450.0      //�Ӷ��۳���

//////////////////////////////////////��ƽ̨�Ƕ�
#define S_Ang_1 0.0
#define S_Ang_2 120.0
#define S_Ang_3 240.0

//////////////////////////////////////Բ�������
#define PI 3.1415926535897
#define PI2 6.2831853071796
#define PI_RAD 0.0174532925199       //�Ƕ�ת��Ϊ����
#define PI_DEG 57.2957795130823      //����ת��Ϊ�ǶȲ���

//////////////////////////////////////����������
#define IN     //�������
#define OUT    //�������

//////////////////////////////////////���嵱������ֵ
#define PULSE 10000
#define ANGLE 360
#define RIGIDRATIO 41

/////////////////////////////////////�������
#define SIDELENGTH_FIXED  160*3.4641016   //sidelength of the fix/end platform triangle,
#define SIDELENGTH_END    50*3.4641016 	 //not the centre-privot distance

/////////////////////////////////////XYƽ̨����
#define RECTANGLESIZE_X 160    //
#define RECTANGLESIZE_Y 220    //

