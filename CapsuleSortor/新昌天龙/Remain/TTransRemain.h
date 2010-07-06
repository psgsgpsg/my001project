// TTransRemain.h: interface for the TTransRemain class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TTRANSREMAIN_H
#define TTRANSREMAIN_H

#include "Timagetmpl.h"
#include "ArcTemple.h"
#include <vector>
using namespace std;

#ifdef _AFXDLL
#include "afx.h"
#else
#include "windows.h"
#endif

#include "Typedefine.h"

#include "icvcimg.h"
#include "icvfoundation.h"
#include "icvcEdge.h"

#pragma comment(lib,"cvcimg.lib")
#pragma comment(lib,"cvfoundation.lib")
#pragma comment(lib, "cvcEdge.lib")

class TTransRemain  
{
public:
	TTransRemain();
	~TTransRemain();
	
	enum	REMAINMODE {TRANSMODE,	OPAQUEMODE};

	bool	RemainCapsule	(	const TAlloc<PixelMem>	&rawImg, 
								const TImgDim			&rawImgDim, 
								TImage<PelGray8>		&remain,								
								RemainParam				&remainParam,
								REMAINMODE				remainMode,
								bool					frontLight = false);

	void	ReInit			(	IMG		&image);
private:
	TImage<unsigned char>	m_rawImage;
	TImage<unsigned char>	m_binImage;
	TImage<unsigned char>	m_sobelImage;
	TImage<unsigned char>	m_andedImage;
	TImage<unsigned char>	m_minBoxImage;


	REMAINMODE				m_mode;
	IMG						m_binIMG;
	IMG						m_rawIMG;
	IMG						m_minBoxIMG;
	FBLOB					m_whiteBlob;
	FBLOB					m_blackBlob;

	vector<RECT>&			GetMouldRect	(	FBLOB						&blob);
	void					WhiteBlob		(	IMG							&image, 
												FBLOB						&blob	);
	bool					IsEmpty			(	IMG							&image, 
												RECT						rect);
	void					CheckPosition	(	TImage<unsigned char>		&srcImage,
												TImage<unsigned char >		&dstImage,
												vector<RECT>				&rectSet, 
												const	size_t				radius);
	void					GetTopPos		(	TImage<unsigned char>		&image,
												TRect2D<int>				&rect, 
												TPoint2D<int>				&centre,
												ArcTemple					&temp);
	void					FillDest		(	TImage<unsigned char>		&image,
												TPoint2D<int>				&upCentre, 
												TPoint2D<int>				&downCentre,
												HDC							&memHDC,
												const size_t				radius);
	bool					BlackDetect		(	IMG							&image,
												const size_t				sizeLimit);
	void					ExcludeEmpty	(	vector<RECT>				&rectSet,
												const int					imgHeight,
												const int					imgWidth);
	void					BlobInit		(	IMG							&image,
												FBLOB						&blob);
	bool					IsEmpty1		(	IMG							&image, 
												RECT						&rect,
												size_t						maxHeight);
	RECT					RectNoBlack		(	IMG							&image, 
												TRect2D<int>				&rect,
												IMG							debugImage);
	bool					DynamicEdge		(	IMG							&image,
												TArea						&area,
												TEdgeResult					&edgeResult,
												double						&thres,
												double						termValue);
	bool					EdgeJudge		(	IMG							&image,
												RECT						&rect,	
												RECT						&edgeRect);
	void					SetArea			(	int					x0,
												int					y0,
												int					x1,
												int					y1,
												int					x2,
												int					y2,
												TArea				&area);
	void					FillRect		(	IMG							&image,
												RECT						&rect);
};

#endif //TTRANSREMAIN_H
