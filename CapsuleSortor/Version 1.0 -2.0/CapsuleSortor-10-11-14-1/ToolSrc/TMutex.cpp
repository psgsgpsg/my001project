// TMutex.h : Implement of the TMutex class
//
// Copyright (c) 2007 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2006.01.09
//
// A tool to use Mutex

#include "TMutex.h"

TMutex::TMutex(LPCTSTR pName, BOOL bInitialOwner, LPCTSTR pExtName)
: m_hMutex(NULL)
{
	if(NULL == pExtName)
	{
		m_hMutex = ::CreateMutex(NULL, bInitialOwner, pName);
	}
	else
	{
		int nameLen = lstrlen(pName);
		int extLen	= lstrlen(pExtName);
		TCHAR *pMutexName = new TCHAR[nameLen + extLen + 1];
		*pMutexName = 0;

		lstrcat(pMutexName, pName);
		lstrcat(pMutexName, pExtName);
		m_hMutex = ::CreateMutex(NULL, bInitialOwner, pMutexName);

		delete []pMutexName;
	}
}

TMutex::~TMutex()
{
	if(m_hMutex)
		::CloseHandle(m_hMutex);
}
void TMutex::Lock()
{
	::WaitForSingleObject(m_hMutex, INFINITE);
}
void TMutex::Unlock()
{
	::ReleaseMutex(m_hMutex);
}
