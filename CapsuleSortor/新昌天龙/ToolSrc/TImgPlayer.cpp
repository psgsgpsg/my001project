// TImgPlay.h	: Interface of the TImgPlay class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2006.11.27
//
// A simple tool to solve the dispaly of image(1,4,8,16,24,32bits)

#include "TImgPlayer.h"

TImgPlayer::TImgPlayer(TBmpInfo::EPelBit ePelBits)
 : m_bmpInfo(ePelBits), m_pBits(0)
{	}

TImgPlayer::~TImgPlayer()
{	}

void TImgPlayer::SetImage(void* pPixelBuff, size_t width, size_t height)
{
	m_pBits = pPixelBuff;
	m_bmpInfo.Width (width);
	m_bmpInfo.Height(height);
}

void TImgPlayer::SetImage( void* pPixelBuff, const TBmpInfo& bmpInfo)
{
	m_bmpInfo	= bmpInfo;
	m_pBits		= pPixelBuff;
}

size_t TImgPlayer::Width()
{
	return m_bmpInfo.Width();
}

size_t TImgPlayer::Height()
{
	return m_bmpInfo.Height();
}

void TImgPlayer::Flip()
{
	m_bmpInfo.Flip();
}

TBmpInfo& TImgPlayer::GetBmpInfo()
{
	return m_bmpInfo;
}

bool TImgPlayer::CopyToDevice(
		HDC hdc,		// handle to device context
		int xDst,		// x-coordinate of upper-left corner of dest
		int yDst,		// y-coordinate of upper-left corner of dest
		int xSrc,		// x-coordinate of lower-left corner of source
		int ySrc,		// y-coordinate of lower-left corner of source
		DWORD srcWidth, // source rectangle width
		DWORD srcHeight // source rectangle height
		)
{
	int showLines = 0; 
	if(0 != m_pBits)
	{
		size_t scanLines = m_bmpInfo.Height();

		showLines = SetDIBitsToDevice(hdc,
						xDst,	yDst, srcWidth,	srcHeight,
						xSrc,	ySrc,	0,		scanLines,
						m_pBits,	(BITMAPINFO*)m_bmpInfo.BmpInfoPoint(),	DIB_RGB_COLORS);
	}
	return (showLines != 0);
}

bool TImgPlayer::StrechToDevice(
		HDC hdc,		// handle to device context
		int xDst,		// x-coordinate of upper-left corner of dest
		int yDst,		// y-coordinate of upper-left corner of dest
		int dstWidth,	// destination rectangle width 
		int dstHeight,	// destination rectangle height
		int xSrc,		// x-coordinate of lower-left corner of source
		int ySrc,		// y-coordinate of lower-left corner of source
		int srcWidth,	// source rectangle width
		int srcHeight	// source rectangle height
		)
{
	int showLines = GDI_ERROR;
	if(0 != m_pBits)
	{
		SetStretchBltMode(hdc, COLORONCOLOR);
		showLines = StretchDIBits(hdc,	xDst, yDst, dstWidth, dstHeight,
										xSrc, ySrc, srcWidth, srcHeight,
										m_pBits, (BITMAPINFO*)m_bmpInfo.BmpInfoPoint(), DIB_RGB_COLORS,SRCCOPY);
	}
	return (showLines != GDI_ERROR);
}