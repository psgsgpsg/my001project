// TSIMDChecker.h : Interface and Implement of the TAlloc class
//
// Copyright (c) 2008 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2008.06.01
//
// A tool to check cpu support SIMD instruction

#ifndef TSIMDCHECKER_H
#define TSIMDCHECKER_H

class TSIMDChecker
{
public:
	static bool IsSupportMMX();
	static bool IsSupportSSE();
	static bool IsSupportSSE2();
	static bool IsSupportSSE3();
	static bool IsSupportSSSE3();
private:
	TSIMDChecker();
	~TSIMDChecker();
};

inline bool TSIMDChecker::IsSupportMMX()
{
	__asm	mov eax, 1
	__asm	cpuid
	__asm	test EDX, 00800000h ; EDX Bit 23 in feature flags equal to 1
	__asm	jnz Found
	return false;
Found:
	return true;
}

inline bool TSIMDChecker::IsSupportSSE()
{
	__asm	mov eax, 1
	__asm	cpuid
	__asm	test EDX, 002000000h ; EDX Bit 25 in feature flags equal to 1
	__asm	jnz Found
	return false;
Found:
	return true;
}

inline bool TSIMDChecker::IsSupportSSE2()
{
	__asm	mov eax, 1
	__asm	cpuid
	__asm	test EDX, 004000000h ; EDX Bit 26 in feature flags equal to 1
	__asm	jnz Found
	return false;
Found:
	return true;
}

inline bool TSIMDChecker::IsSupportSSE3()
{
	__asm	mov eax, 1
	__asm	cpuid
	__asm	test ECX, 000000001h ; ECX Bit 0 in feature flags equal to 1
	__asm	jnz Found
	return false;
Found:
	return true;
}

inline bool TSIMDChecker::IsSupportSSSE3()
{
	__asm	mov eax, 1
	__asm	cpuid
	__asm	test ECX, 000000200h ; ECX bit 9 in feature flags equal to 1
	__asm	jnz Found
	return false;
Found:
	return true;
}

#endif //TSIMDCHECKER_H