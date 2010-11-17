#ifndef TOINTEGER_H
#define TOINTEGER_H
// ToInteger.h	: interface and implement of the ToInteger class
//
// High-speed convertion from double to int
//
// Copyright (c) 2007 zhao_fsh  Zibo Creative Computor CO.,LTD
//
//////////////////////////////////////////////////////////////////////////
//
// ��������ȫ���������¼�������
// (1)��x86���ܣ�
// (2)����IEEE�ĸ�������׼��
// (3)intΪ32λ��floatΪ32λ��doubleΪ64λ��

// �������뵽����
inline int RoundToInt (double dval)
{
	static double magic = 6755399441055744.0;
	dval += magic;
	return *(int*)&dval;
}

// ��ȡ������
inline int FloorToInt(double dval)
{
	static double magic_delta=0.499999999999;
	return RoundToInt(dval-magic_delta);
}

// ��λ������
inline int CeilToInt (double dval)
{
	static double magic_delta=0.499999999999;
	return RoundToInt(dval+magic_delta);
}

#endif //TOINTEGER_H