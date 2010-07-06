// TRemain.h: interface for the TRemain class.
//
//////////////////////////////////////////////////////////////////////
#ifndef TREMAIN_H
#define TREMAIN_H

#ifdef _AFXDLL
#include "afx.h"
#else
#include "windows.h"
#endif

#include "TypeDefine.h"
#include "TImagetmpl.h"
#include "ArcTemple.h"
#include "icvcimg.h"
#include "icvfoundation.h"
#include <vector>
using namespace std;

#pragma comment(lib, "cvcimg.lib")
#pragma comment(lib, "cvfoundation.lib")

class TRemain  
{
public:
	TRemain();
	~TRemain();
	void					ReInit(IMG &image);
	bool					RemainCapsule	(	const TAlloc<PixelMem>	&rawImg, 
												const TImgDim			&rawImgDim, 
												TImage<PelGray8>		&remain,
												RemainParam				&remainParam,			
												bool					frontLight = false);
private:
	void					ReInitCVBTool	(	IMG						&image);
	void					WhiteBlob		(	IMG						&image, FBLOB &blob);
	vector<RECT>&			GetMouldRect	(	FBLOB					&blob);
	bool					IsEmpty			(	IMG						&image, RECT& rect);
	void					CheckPosition	(	TImage<unsigned char>	&srcImage,
												TImage<unsigned char >	&dstImage,
												vector<RECT>			&rectSet, 
												const	size_t			radius);
	void					GetTopPos		(	TImage<unsigned char>		&image,
												TRect2D<int>				&rect, 
												TPoint2D<int>				&centre,
												ArcTemple					&temp);
	void					FillDest		(	TImage<unsigned char>		&image,
												TPoint2D<int>				&upCentre, 
												TPoint2D<int>				&downCentre,
												HDC							&memHDC,
												const size_t				radius);
	TImage<unsigned char>	m_rawImage;
	TImage<unsigned char>	m_sobelImage;

	IMG						m_rawIMG;
	FBLOB					m_emptyBlob;
	FBLOB					m_whiteBlob;

};

#endif // TREMAIN_H
