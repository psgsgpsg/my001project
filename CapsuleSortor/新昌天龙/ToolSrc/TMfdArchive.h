// TMfdArchive.h	: Interface of the TMfdArchive class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.08.26
// Last	  Data  : 2008.08.26
// A simple tool to solve the archive of Mult Frame image

#ifndef TMFDARCHIVE_H
#define TMFDARCHIVE_H

#include "PixelType.h"
#include "TAllocTmpl.h"
#include "TImgBuffer.h"
#include "TImgDim.h"

class TMfdArchive
{
public:
	typedef struct {
		char		flag[4];
		TImgDim		dimension;
		size_t		frameCount;
	} MfdInfo;

	TMfdArchive();
	~TMfdArchive();

public:
	bool				SetSingleImage	(void* pData, const TImgDim& dim);
	bool				SetMultiImage	(const TImgBuffer& buff);
	TImgBuffer			GetImgBuffer	();
	TImgDim				Dimension		();
	size_t				FrameCount		();

	bool				Load(const TCHAR *fileName);
	bool				Save(const TCHAR *fileName);

private:
	TMfdArchive				(const TMfdArchive& src);
	TMfdArchive& operator=	(const TMfdArchive& src);

private:
	TImgBuffer		m_imgBuff;
	MfdInfo			m_mfdInfo;
};

#endif//TMFDARCHIVE_H
