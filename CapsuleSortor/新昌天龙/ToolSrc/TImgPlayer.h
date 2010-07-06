// TImgPlay.h	: Interface of the TImgPlay class
//
// Copyright (c) 2006 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2006.11.27
// Last	  Data  : 2008.03.26
// A simple tool to solve the dispaly of image(1,4,8,16,24,32bits)

#ifndef TIMGPLAYER_H
#define TIMGPLAYER_H

#include "TBmpInfo.h"

class TImgPlayer
{
public:
	explicit TImgPlayer(TBmpInfo::EPelBit ePelBit= TBmpInfo::e8Bit);
	~TImgPlayer();

public:

	void	SetImage( void* pPixelBuff, size_t width, size_t height);
	void	SetImage( void* pPixelBuff, const TBmpInfo& bmpInfo);

	size_t	Width	();
	size_t	Height	();
	void	Flip	();
	TBmpInfo& GetBmpInfo ();

	bool CopyToDevice(HDC hdc,	int xDst, int yDst,
								int xSrc, int ySrc, DWORD srcWidth, DWORD srcHeight);

	bool StrechToDevice(HDC hdc,int xDst, int yDst, int dstWidth, int dstHeight,
								int xSrc, int ySrc, int srcWidth, int srcHeight);
private:
	TImgPlayer				(const TImgPlayer& src);
	TImgPlayer& operator=	(const TImgPlayer& src);

private:
	TBmpInfo	m_bmpInfo;
	void*		m_pBits;
};

#endif//TIMGPLAY_H

