// TLocker.h : Interface and Implement of the TLocker class
//
// Copyright (c) 2007 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2007.01.09
//
// A tool to use critical sections

#ifndef TLOCKER_H
#define TLOCKER_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

class TLockObject
{
public:
	TLockObject() {}
	virtual ~TLockObject() {}
	virtual void Lock()		= 0;
	virtual void Unlock()	= 0;
};

class TLocker
{
public:
	TLocker(TLockObject & object);
	~TLocker();

private:
	TLocker(const TLocker& src);
	TLocker& operator= (const TLocker& src);

private:
	TLockObject& m_lockObject;
};

inline TLocker::TLocker(TLockObject & object)
: m_lockObject(object)
{
	m_lockObject.Lock();
}

inline TLocker::~TLocker()
{
	m_lockObject.Unlock();
}

#endif //TCRITSECT_H