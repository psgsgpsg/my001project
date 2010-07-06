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
// 下述方法全都基于以下几个假设
// (1)在x86上跑；
// (2)符合IEEE的浮点数标准；
// (3)int为32位，float为32位，double为64位。

// 四舍五入到整型
inline int RoundToInt (double dval)
{
	static double magic = 6755399441055744.0;
	dval += magic;
	return *(int*)&dval;
}

// 截取到整数
inline int FloorToInt(double dval)
{
	static double magic_delta=0.499999999999;
	return RoundToInt(dval-magic_delta);
}

// 进位到整数
inline int CeilToInt (double dval)
{
	static double magic_delta=0.499999999999;
	return RoundToInt(dval+magic_delta);
}

#endif //TOINTEGER_H