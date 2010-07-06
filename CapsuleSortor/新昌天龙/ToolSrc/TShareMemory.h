// TShareMemory.h	: Interface of the TShareMemory class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Create Date	: 2007.01.08
//
#ifndef TSHAREMEMORY_H
#define TSHAREMEMORY_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "TMutex.h"		//for TMutex, TLocker class

class TShareMemory
{
public:
	TShareMemory (LPCTSTR pName, DWORD capacity);
	~TShareMemory( );

	bool	Read (void *pVoid, DWORD size) const;
	bool	Write(const void *pVoid, DWORD size);

	DWORD	Capacity()  const;
	DWORD	Size	()	const;

private:
	TShareMemory(const TShareMemory& src);
	TShareMemory& operator= (const TShareMemory& src);

	DWORD	Open (LPCTSTR pName, DWORD capacity);
	void	Close( );

private:
	HANDLE		m_hMapping;

	struct ShareBlock
	{
		DWORD	m_capacity;
		DWORD	m_validSize;
		char    m_Memory[1];
	};
	ShareBlock *m_pShareBlock;
	mutable TMutex	m_accessMutex;
};

inline DWORD TShareMemory::Capacity()  const
{
	return (m_pShareBlock) ? m_pShareBlock->m_capacity : 0;
}

inline DWORD TShareMemory::Size	()	const
{
	return (m_pShareBlock) ? m_pShareBlock->m_validSize : 0;
}

#endif //TSHAREMEMORY_H
