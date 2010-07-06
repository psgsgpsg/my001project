// TShareMemory.h	: Interface of the TShareMemory class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Create Date	: 2007.01.08
//

#include "TShareMemory.h"

TShareMemory::TShareMemory(LPCTSTR pName, DWORD size)
:m_hMapping(0) , m_pShareBlock(0) , m_accessMutex(pName, FALSE, TEXT("Mu2t3ex"))
{
	Open(pName, size);
}

TShareMemory::~TShareMemory()
{
	Close();
}

bool TShareMemory::Read(void *pVoid, DWORD size) const
{
	TLocker lock(m_accessMutex);
	if(m_pShareBlock && m_pShareBlock->m_capacity >= size)
	{
		memcpy(pVoid, &(m_pShareBlock->m_Memory), size);
		return true;
	}
	else
	{
		return false;
	}
}

bool TShareMemory::Write(const void *pVoid, DWORD size)
{
	TLocker lock(m_accessMutex);
	if(m_pShareBlock && m_pShareBlock->m_capacity >= size)
	{
		memcpy(&(m_pShareBlock->m_Memory), pVoid, size);
		m_pShareBlock->m_validSize = size;
		return true;
	}
	else
	{
		return false;
	}
}

DWORD TShareMemory::Open(LPCTSTR pName, DWORD size)
{
	Close();	//first close the old share memory;

	TLocker lock(m_accessMutex);

	m_hMapping =	CreateFileMapping(
					INVALID_HANDLE_VALUE,
					NULL,
					PAGE_READWRITE,
					0,
					(DWORD)(size + sizeof(ShareBlock) - 1),
					pName);

	if(NULL == m_hMapping)
		return 0;

	bool bExist = (ERROR_ALREADY_EXISTS == GetLastError());

	m_pShareBlock = (ShareBlock*) ::MapViewOfFile(
					m_hMapping,
					FILE_MAP_ALL_ACCESS,
					0,
					0,
					0);
	if(NULL == m_pShareBlock)
		return 0;
													
	if(!bExist)
	{
		m_pShareBlock->m_capacity	= size;
		m_pShareBlock->m_validSize	= 0 ;
	}

	return m_pShareBlock->m_capacity;
}

void TShareMemory::Close( )
{
	TLocker lock(m_accessMutex);

	if(m_pShareBlock)
	{
		::UnmapViewOfFile(m_pShareBlock);
		m_pShareBlock = 0;
	}
	if(m_hMapping)
	{
		::CloseHandle(m_hMapping);
		m_hMapping = 0;
	}
}
