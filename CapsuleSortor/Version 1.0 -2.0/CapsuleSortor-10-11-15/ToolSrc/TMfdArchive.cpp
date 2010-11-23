// TMdfArchive.cpp	: Interface of the TMdfArchive class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.03.26
// Last	  Data  : 
// A simple tool to solve the archive of Multi Frame image

#include "TMfdArchive.h"

TMfdArchive::TMfdArchive()
{
	memcpy(m_mfdInfo.flag, "MFDI", 4);
	m_mfdInfo.frameCount = 0;
}

TMfdArchive::~TMfdArchive()
{ }

bool TMfdArchive::SetSingleImage(void* pData, const TImgDim& dim)
{
	m_imgBuff.ReInit(dim, 1);
	memcpy(m_imgBuff.FrameData(0), pData, dim.bytesPerRow * dim.height);
	return true;
}

bool TMfdArchive::SetMultiImage(const TImgBuffer& imgBuff)
{
	m_imgBuff = imgBuff;
	return true;
}

TImgBuffer TMfdArchive::GetImgBuffer()
{
	return m_imgBuff;
}

TImgDim TMfdArchive::Dimension()
{
	return m_imgBuff.Dimension();
}

bool TMfdArchive::Load(const TCHAR *fileName)
{
	HANDLE hFile = CreateFile(	fileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_SEQUENTIAL_SCAN,
								NULL);

	if(INVALID_HANDLE_VALUE == hFile) 
	{	return false; }

	DWORD	bytesRead	= 0;
	BOOL	success		= FALSE;
	success = ReadFile(	hFile,
						&m_mfdInfo,
						sizeof(MfdInfo),
						&bytesRead,
						NULL);
	if(!success || bytesRead != sizeof(MfdInfo))
	{
		CloseHandle(hFile);
		return false;
	}

	if( 0 != memcmp( m_mfdInfo.flag, "MFDI", 4))
	{
		CloseHandle(hFile);
		return false;
	}
	
	m_imgBuff.ReInit(m_mfdInfo.dimension, m_mfdInfo.frameCount);
	size_t buffSize	=	m_mfdInfo.dimension.bytesPerRow * 
						m_mfdInfo.dimension.height *
						m_mfdInfo.frameCount;

	success = ReadFile(	hFile,
						m_imgBuff.FrameData(0),
						buffSize,
						&bytesRead,
						NULL);
	if(!success || bytesRead != buffSize)
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}

bool TMfdArchive::Save(const TCHAR *fileName)
{
	
	memcpy(m_mfdInfo.flag, "MFDI", 4);
	m_mfdInfo.dimension		= m_imgBuff.Dimension();
	m_mfdInfo.frameCount	= m_imgBuff.FrameCount();
	size_t buffSize			= m_mfdInfo.dimension.bytesPerRow * 
							  m_mfdInfo.dimension.height *
							  m_mfdInfo.frameCount;	
	HANDLE hFile = CreateFile(	fileName,
								GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	DWORD fileSize = 0;
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWrite = 0;
		WriteFile(hFile, &m_mfdInfo,			sizeof(MfdInfo),	&bytesWrite,	NULL);
		WriteFile(hFile, m_imgBuff.FrameData(0),buffSize,			&bytesWrite,	NULL);
		fileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
	}

	if(fileSize != sizeof(MfdInfo) + buffSize)
	{
		DeleteFile(fileName);
		return false;
	}
	return true;
}


