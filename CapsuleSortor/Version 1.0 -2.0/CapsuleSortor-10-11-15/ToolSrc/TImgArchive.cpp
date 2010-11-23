// TImgArchive.cpp	: Interface of the TImgArchive class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.03.26
// Last	  Data  : 
// A simple tool to solve the archive of image

#include "TImgArchive.h"

TImgArchive::TImgArchive()
{ }

TImgArchive::~TImgArchive()
{ }

bool TImgArchive::SetImage(TAlloc<PixelMem>& bmpBuff, const TBmpInfo& bmpInfo)
{
	m_bmpInfo = bmpInfo;
	TImgDim	dimension = bmpInfo.Dimension();
	m_imgBuff = bmpBuff;
	return true;
}
bool TImgArchive::SetImage(void* pData, const TImgDim& dim)
{
	m_bmpInfo.ReInit(dim);
	size_t pelRowBytes = dim.width*dim.bytesPerPixel; 
	size_t bmpRowSpace = ((pelRowBytes + 3)/4)*4;
	m_imgBuff = TAlloc<PixelMem>(dim.height * bmpRowSpace);
	PixelMem *pSrc = (PixelMem*)(pData);
	PixelMem *pDst = m_imgBuff.Base();

	for(size_t i=0; i<dim.height; ++i)
	{
		memcpy(pDst, pSrc, pelRowBytes);
		pDst += bmpRowSpace;
		pSrc += dim.bytesPerRow;
	}
	return true;
}

TBmpInfo& TImgArchive::GetBmpInfo()
{
	return m_bmpInfo;
}

TAlloc<PixelMem> TImgArchive::GetImgMem()
{
	return m_imgBuff;
}

TImgDim TImgArchive::Dimension()
{
	return m_bmpInfo.Dimension();
}

bool TImgArchive::Load(const TCHAR *fileName)
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
						&m_bmpFileHead,
						sizeof(BITMAPFILEHEADER),
						&bytesRead,
						NULL);
	if(!success || bytesRead != sizeof(BITMAPFILEHEADER))
	{
		CloseHandle(hFile);
		return false;
	}

	if(m_bmpFileHead.bfType != *(WORD*)"BM")
	{
		CloseHandle(hFile);
		return false;
	}

	size_t bmpInfoSize = m_bmpFileHead.bfOffBits - sizeof(BITMAPFILEHEADER);
	TAlloc<char> bmpInfo (bmpInfoSize);
	success = ReadFile(hFile, bmpInfo.Base(), bmpInfoSize, &bytesRead, NULL);
	if(!success || bytesRead != bmpInfoSize)
	{
		CloseHandle(hFile);
		return false;
	}
	m_bmpInfo.ReInit((BITMAPINFO*)(bmpInfo.Base()), bmpInfo.Size());
	TImgDim dimension = m_bmpInfo.Dimension();
	size_t frameSize  = dimension.bytesPerRow * dimension.height;
	m_imgBuff = TAlloc<PixelMem>(frameSize, e16ByteAlign);

	size_t bmpBuffSize = m_bmpFileHead.bfSize - m_bmpFileHead.bfOffBits;
	success = ReadFile(	hFile,
						m_imgBuff.Base(),
						frameSize,
						&bytesRead,
						NULL);
	if(!success || bytesRead != bmpBuffSize)
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}

bool TImgArchive::Save(const TCHAR *fileName)
{
	size_t	bmpInfoSize = 0;
	void*	pBmpInfo	= m_bmpInfo.BmpInfoPoint(&bmpInfoSize);
	size_t	frameSize	= m_imgBuff.Size();
	
	m_bmpFileHead.bfType		= *(WORD*)"BM";
	m_bmpFileHead.bfOffBits		= sizeof(BITMAPFILEHEADER) + bmpInfoSize;
	m_bmpFileHead.bfSize		= m_bmpFileHead.bfOffBits  + frameSize;
	m_bmpFileHead.bfReserved1	= 0;
	m_bmpFileHead.bfReserved2	= 0;

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
		WriteFile(hFile, &m_bmpFileHead,	sizeof(BITMAPFILEHEADER),	&bytesWrite,	NULL);
		WriteFile(hFile, pBmpInfo,			bmpInfoSize,				&bytesWrite,	NULL);
		WriteFile(hFile, m_imgBuff.Base(),	frameSize,			&bytesWrite,	NULL);
		fileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
	}

	if(fileSize != m_bmpFileHead.bfSize)
	{
		DeleteFile(fileName);
		return false;
	}
	return true;
}


