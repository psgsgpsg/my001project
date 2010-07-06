// TCritSect.h : Interface and Implement of the TCritSect class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2006.12.13
//
// A tool to use critical sections

#ifndef TCRITSECT_H
#define TCRITSECT_H

#include "TLocker.h"

class TCritSect : public TLockObject
{
public:
	TCritSect	()
	{
		InitializeCriticalSection(&m_critsect);
	}
	virtual ~TCritSect	()
	{
		DeleteCriticalSection(&m_critsect);
	}
	virtual void Lock	()
	{
		EnterCriticalSection(&m_critsect);
	}
	virtual void Unlock	()
	{
		LeaveCriticalSection(&m_critsect);
	}

private:
	TCritSect(const TCritSect& src);
	TCritSect& operator= (const TCritSect& src);

private:
	CRITICAL_SECTION m_critsect;
};

#endif //TCRITSECT_H