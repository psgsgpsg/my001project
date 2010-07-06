// TMutex.h : Interface of the TMutex class
//
// Copyright (c) 2007 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2007.01.09
//
// A tool to use Mutex

#ifndef TMUTEX_H
#define TMUTEX_H

#include "TLocker.h"

class TMutex : public TLockObject
{
public:
	TMutex(LPCTSTR pName, BOOL bInitialOwner = FALSE, LPCTSTR pExtName = NULL);
	virtual ~TMutex();
	virtual void Lock	();
	virtual void Unlock	();	

private:
	TMutex(const TMutex& src);
	TMutex& operator= (const TMutex& src);

private:
	HANDLE m_hMutex;
};

#endif //TMUTEX_H