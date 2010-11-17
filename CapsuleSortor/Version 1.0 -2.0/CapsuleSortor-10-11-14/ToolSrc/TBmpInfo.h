// TBmpInfo.h	: Interface of the TBmpInfo class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.03.26
// Last	  Data  : 
// A simple tool to handle BMP infomation ver 3 (1,4,8,16,24,32bits)

#ifndef TBMPINFO_H
#define TBMPINFO_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif
#include "PixelType.h"
#include "TImgDim.h"
#include "TAllocTmpl.h"

class TBmpInfo
{
public:
	typedef enum {	e1Bit = 1,
					e4Bit = 4,
					e8Bit = 8,
					e16Bit = 16,
					e24Bit = 24,
					e32Bit = 32
	}EPelBit;

	explicit TBmpInfo (EPelBit ePelBits= e8Bit);
	~TBmpInfo();
	//default copy construct
	//default operater= ()

public:
	size_t	Width	() const;
	size_t	Height	() const;
	EPelBit PixelBits()const;
	TImgDim Dimension()const;
	void	Width	(size_t width);
	void	Height	(size_t height);
	void	Flip	();
	void	AccordMem();	//显示或绘图与内存排列一致

	bool	ReInit		 (EPelBit ePelBits, size_t width=0, size_t height=0);
	bool	ReInit		 (const BITMAPINFO *pBmpInfo, size_t size);
	bool	ReInit		 (const TImgDim& dim);
	bool	SetColorTable(BYTE colorIndex, COLORREF rgb);
	bool	SetColorTable(BYTE colorIndex, BYTE red, BYTE green, BYTE blue);
	bool	SetColorMask (DWORD redMask, DWORD greenMask, DWORD blueMask);

	void*	BmpInfoPoint (size_t* pByteCount = NULL) const;	

private:
	size_t	ColorCount  (const EPelBit ePixelBits) const;
	void	InitColorTable();

private:
	TAlloc<char>	m_bmpInfoBuff;
	BITMAPINFO*		m_pBitmapInfo;
};

#endif	//TBMPINFO_H
