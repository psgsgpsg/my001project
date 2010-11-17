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
// 下述方法全都基于以下几个假设
// (1)在x86上；
// (2)符合IEEE的浮点数标准；
// (3)int为32位，float为32位，double为64位。

// 四舍五入到整型
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

// 截取到整数
inline int TToInteger::Floor(double value)
{
	double magic_delta=0.499999999999;
	return Round(value-magic_delta);
}

// 进位到整数
inline int TToInteger::Ceil (double value)
{
	double magic_delta=0.499999999999;
	return Round(value+magic_delta);
}

#endif //TTOINTEGER_H