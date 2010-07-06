// TImgArchive.h	: Interface of the TImgArchive class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.03.26
// Last	  Data  : 
// A simple tool to solve the archive of image

#ifndef TIMGARCHIVE_H
#define TIMGARCHIVE_H

#include "TBmpInfo.h"
#include "PixelType.h"
#include "TAllocTmpl.h"

class TImgArchive
{
public:
	explicit TImgArchive();
	~TImgArchive();

public:

	bool				SetImage(TAlloc<PixelMem>& bmpBuff, const TBmpInfo& bmpInfo);
	bool				SetImage(void* pData, const TImgDim& dim);
	TBmpInfo&			GetBmpInfo();
	TAlloc<PixelMem>	GetImgMem();
	TImgDim				Dimension();
	bool				Load(const TCHAR *fileName);
	bool				Save(const TCHAR *fileName);

private:
	TImgArchive				(const TImgArchive& src);
	TImgArchive& operator=	(const TImgArchive& src);

private:
	BITMAPFILEHEADER	m_bmpFileHead;
	TBmpInfo			m_bmpInfo;
	TAlloc<PelGray8>	m_imgBuff;
};

#endif//TIMGARCHIVE_H

