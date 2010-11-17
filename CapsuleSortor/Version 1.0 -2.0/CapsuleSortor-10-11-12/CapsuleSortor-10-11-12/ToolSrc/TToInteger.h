#ifndef TTOINTEGER_H
#define TTOINTEGER_H
// ToInteger.h	: interface and implement of the ToInteger class
//
// High-speed convertion from double to int
//
// Copyright (c) 2007 zhao_fsh  Zibo Creative Computor CO.,LTD
//
//////////////////////////////////////////////////////////////////////////
//
// ��������ȫ���������¼�������
// (1)��x86�ϣ�
// (2)����IEEE�ĸ�������׼��
// (3)intΪ32λ��floatΪ32λ��doubleΪ64λ��

// �������뵽����
class TToInteger
{
public:
	static int Round(double value);
	static int Floor(double value);
	static int Ceil (double value);
private:
	TToInteger();
};

inline int TToInteger::Round(double value)
{
	double magic = 6755399441055744.0;
	value += magic;
	return *(int*)&value;
}

// ��ȡ������
inline int TToInteger::Floor(double value)
{
	double magic_delta=0.499999999999;
	return Round(value-magic_delta);
}

// ��λ������
inline int TToInteger::Ceil (double value)
{
	double magic_delta=0.499999999999;
	return Round(value+magic_delta);
}

#endif //TTOINTEGER_H