// TTransRemain.cpp: implementation of the TTransRemain class.
//
//////////////////////////////////////////////////////////////////////

#include "TTransRemain.h"
#include "TImgProcess.h"
#include "afxwin.h"
#include "ttimediff.h"
#include <sstream>
#include "TToInteger.h"
#include "SortObserver.h "
#include "CapsuleProc.h"
using namespace std;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TTransRemain::TTransRemain():
m_whiteBlob(NULL),
m_binIMG(NULL),
m_rawIMG(NULL),
m_minBoxIMG(NULL),
m_blackBlob(NULL)
{

}

TTransRemain::~TTransRemain()
{

}

bool	TTransRemain::RemainCapsule	(	const TAlloc<PixelMem>	&rawImg, 
										const TImgDim			&rawImgDim, 
										TImage<PelGray8>		&remain,
										RemainParam				&remainParam,
										REMAINMODE				remainMode,
										bool					frontLight)
{
	m_mode = remainMode;
	if(rawImgDim.bytesPerPixel == 3)
	{
		TImage<PelRGB24> rgb(rawImg, rawImgDim);
		if(frontLight)	{	TImgProcess::RGBToGray (rgb, m_rawImage);	}
		else			{	TImgProcess::RGBToGray2(rgb, m_rawImage); }
	}
	else
	{
		m_rawImage.SetImage(rawImg.Base(), rawImgDim);
	}
	remain.SetZero();

	try
	{
		
		TImgProcess::BinaryGray(m_rawImage, m_binImage, remainParam.binaryLowThres, remainParam.binaryUpThres);
//		m_sortObserver.ObserverIMG(SortObserver::MAll, m_cvbProfileImg);
				
		IMG minImage = NULL; 
		FilterBoxMin(	m_binIMG,
						remainParam.minBoxWidth,
						remainParam.minBoxWidth, 
						remainParam.minBoxWidth/2, 
						remainParam.minBoxWidth/2, 
						minImage);
		unsigned char	*pImage = NULL;
		PVPAT			VPA		= NULL;
		GetImageVPA	(	minImage, 0, (void**)&pImage, &VPA);
		memcpy		(	m_minBoxImage.Base(), pImage, m_minBoxImage.Width()*m_minBoxImage.Height());
		ReleaseImage(	minImage);
		
		TImgProcess::SobelFilter(	m_rawImage,	m_sobelImage);
		CapsuleProc::ObservedIMG(SortObserver::MEnh, m_sobelIMG);
		TImgProcess::AndGrayImg	(	m_minBoxImage, m_sobelImage, m_andedImage);	

		CapsuleProc::ObservedIMG(SortObserver::MAnd, m_andedIMG);
	}
	catch (...)
	{
		static int count = 0;
		CString txt;
		txt.Format("D:\\\n%d.txt", count);
		count++;
		
		FILE* pFile = NULL;
		fopen_s(&pFile, "D:\\CatchFile.txt", "wb");
		fwrite(txt, sizeof(char), strlen(txt), pFile);
		fclose(pFile);
		return false;
	}
		
	try
	{
		WhiteBlob		(	m_binIMG, m_whiteBlob);	
		vector<RECT> rectSet = GetMouldRect(m_whiteBlob);	
		
		ExcludeEmpty	(	rectSet, remain.Height(), remain.Width());
		
		CheckPosition	(	m_andedImage, remain, rectSet, remainParam.radius);
		TImgProcess::AndGrayImg(m_rawImage, remain, remain);
	}	
	catch (...)
	{
		static int count = 0;
		CString txt;
		txt.Format("D:\\\n%d.txt", count);
		count++;

		FILE* pFile = NULL;
		fopen_s(&pFile, "D:\\CatchFile1.txt", "wb");
		fwrite(txt, sizeof(char), strlen(txt), pFile);
		fclose(pFile);
		return false;
	}
	return true;	
}

void	TTransRemain::ReInit(IMG	&image)
{
	size_t	imgWidth = ImageWidth(image);
	size_t	imgHeight = ImageHeight(image);
	TRect2D<int>	bound(0, 0, imgWidth-1, imgHeight-1);
	m_rawImage.ReInit(bound);
	m_binImage.ReInit(bound);
	m_minBoxImage.ReInit(bound);
	m_sobelImage.ReInit(bound);
	m_andedImage.ReInit(bound);
	if (m_binIMG)
	{
		ReleaseImage(m_binIMG);
	}
	if (m_rawIMG)
	{
		ReleaseImage(m_rawIMG);
	}
	if (m_minBoxIMG)
	{
		ReleaseImage(m_minBoxIMG);
	}
	CreateImageFromPointer(m_binImage.Base(), m_binImage.Width()*m_binImage.Height(), m_binImage.Width(), m_binImage.Height(),
							1, 8, 1, m_binImage.Width(), 1, NULL, NULL, NULL, m_binIMG);
	CreateImageFromPointer(m_rawImage.Base(), m_rawImage.Width()*m_rawImage.Height(), m_rawImage.Width(), m_rawImage.Height(),
							1, 8, 1, m_rawImage.Width(), 1, NULL, NULL, NULL, m_rawIMG);
	CreateImageFromPointer(m_minBoxImage.Base(), m_minBoxImage.Width()*m_minBoxImage.Height(), m_minBoxImage.Width(), m_minBoxImage.Height(),
							1, 8, 1, m_minBoxImage.Width(), 1, NULL, NULL, NULL, m_minBoxIMG );
	CreateImageFromPointer(m_andedImage.Base(), m_andedImage.Width()*m_andedImage.Height(), m_andedImage.Width(), m_andedImage.Height(),
		1, 8, 1, m_andedImage.Width(), 1, NULL, NULL, NULL, m_andedIMG );
	CreateImageFromPointer(m_sobelImage.Base(), m_sobelImage.Width()*m_sobelImage.Height(), m_sobelImage.Width(), m_sobelImage.Height(),
		1, 8, 1, m_sobelImage.Width(), 1, NULL, NULL, NULL, m_sobelIMG );

	BlobInit(m_rawIMG, m_whiteBlob);
	BlobInit(m_rawIMG, m_blackBlob);
}
void	TTransRemain::WhiteBlob(IMG &image, FBLOB &blob)
{
	FBlobSetImage				(blob, image, 0);
	FBlobSetObjectFeatureRange	(blob, 128,	255);
	FBlobSetObjectTouchBorder	(blob, FBLOB_BORDER_NONE);
	FBlobSetLimitArea			(blob, 10, -1);
	FBlobSetLimitWidth			(blob, 30, -1);
	FBlobSetSortMode			(blob, FBLOB_SORT_POSX, 0, 0, 0, FBLOB_SORT_RISING);
	FBlobSetSkipBinarization	(blob, TRUE);
	FBlobExec					(blob);
}

RECT	TTransRemain::RectNoBlack(IMG &image, TRect2D<int> &rect, IMG debugImage)
{
	unsigned char				*pImage = NULL;
	PVPAT						VPA		= NULL;
	GetImageVPA					(image, 0, (void**)&pImage, &VPA);

	//const size_t rectWidth	= rect.Width();
	//const size_t rectHeight = rect.Height();

	RECT	resultRect = { 0, 0, 0, 0};

	bool lineEmpty = false;
	for(int i = rect.x0() ; i < rect.x1(); ++i)
	{
		for (int j = rect.y0(); j < rect.y1(); ++j)
		{
			if (0 == *(VPA[i].XEntry + pImage + VPA[j].YEntry) )
			{
				break;
			}
			//*(rawVPA[i].XEntry + pRawImage + rawVPA[j].YEntry) = 255;
			
			if (rect.y1() -1 == j)
			{
				lineEmpty = true;
			}
		}
		if (lineEmpty)
		{
			resultRect.left = rect.x0() + i;
			resultRect.top	= rect.y0();
			break;
		}		
	}

	lineEmpty	= false;
	for (int i = rect.x1()-1; i >rect.x0(); --i)
	{
		for (int j = rect.y0(); j < rect.y1(); ++j)
		{			
			
			if ( 0 == *(VPA[i].XEntry + pImage + VPA[j].YEntry))
			{
				break;
			}
			if ( rect.y1()-1 == j)
			{
				lineEmpty = true;
			}
		}
		if (lineEmpty)
		{
			resultRect.right = rect.x0() + i;
			resultRect.bottom = rect.y1();
			break;
		}
	}
	return resultRect;
}

vector<RECT>	TTransRemain::GetMouldRect(FBLOB& blob)
{
	long	blobCount = 0;
	FBlobGetNumBlobs(blob, blobCount);	
	vector<RECT>	rectSet;
	rectSet.clear();
	RECT 	blobRect;
	memset(&blobRect, 0X00, sizeof(RECT));
	for( int i = 0; i < blobCount; ++i)
	{
		long startX = 0, startY = 0, dX = 0, dY = 0; 
		FBlobGetBoundingBox(blob, i, startX, startY, dX, dY);
		blobRect.left = startX;
		blobRect.top  = startY;
		blobRect.right = startX+dX;
		blobRect.bottom = startY+dY;
		rectSet.push_back(blobRect);
	}
	return rectSet;
}

bool	TTransRemain::IsEmpty(IMG &image, RECT rect)
{
	IMG tempIMG = NULL;
	IMG maskIMG = NULL;
	CreateImageMap(	m_rawIMG,		rect.left,				rect.top, 
		rect.right, rect.bottom,	rect.right-rect.left, rect.bottom-rect.top, tempIMG);
	CreateImageMap(	m_minBoxIMG,	rect.left,				rect.top, 
		rect.right, rect.bottom,	rect.right-rect.left, rect.bottom-rect.top, maskIMG);
	
	const size_t imgWidth	= ImageWidth(tempIMG);
	const size_t imgHeight	= ImageHeight(tempIMG);

	IMG			 andedImage = NULL;	
	AndImages	(	tempIMG, maskIMG, andedImage);

	const size_t	roiWidth = imgWidth/3;
	const size_t	roiHeigh = imgHeight/2;
	const size_t	roiXStart = imgWidth/3;
	const size_t	roiYStart = imgHeight/4;

	IMG roiImg = NULL;
	CreateImageMap(andedImage, roiXStart, roiYStart, roiXStart+roiWidth, roiYStart+roiHeigh,
		roiWidth, roiHeigh, roiImg);

	bool	isEmpty = BlackDetect(roiImg, roiWidth*roiHeigh/3);

	ReleaseImage(roiImg);
	ReleaseImage(andedImage);
	ReleaseImage(maskIMG);
	ReleaseImage(tempIMG);	

	if (isEmpty) return false;
	else		 return true;
}
//出于速度考虑
bool	TTransRemain::IsEmpty1(IMG &image, RECT& rect, size_t maxHeight)
{
	IMG tempIMG = NULL;
	IMG maskIMG = NULL;
	CreateImageMap(	m_rawIMG,		rect.left,				rect.top, 
		rect.right, rect.bottom,	rect.right-rect.left, rect.bottom-rect.top, tempIMG);
	CreateImageMap(	m_minBoxIMG,	rect.left,				rect.top, 
		rect.right, rect.bottom,	rect.right-rect.left, rect.bottom-rect.top, maskIMG);

	const size_t imgWidth	= ImageWidth(tempIMG);
	const size_t imgHeight	= ImageHeight(tempIMG);
	IMG			 andedImage = NULL;
	AndImages	(	tempIMG, maskIMG, andedImage);

	size_t detectCount = 0;
	size_t indent = 1 ;

	for (int i  = 1; i < 5; ++i)
	{
		RECT	ROIRect = { indent, imgHeight*i/5, imgWidth-indent,   imgHeight*i/5 + 20};
		//RECT			edgeRect = RectNoBlack(andedImage, ROIRect, tempIMG);
		if (EdgeJudge(andedImage, rect , ROIRect))
		{
			detectCount++;
		}
	}	

	ReleaseImage(andedImage);
	ReleaseImage(maskIMG);
	ReleaseImage(tempIMG);

	if (detectCount >= 3)
	{
		return false;
	}

	return true;	
}

void TTransRemain::CheckPosition(	TImage<unsigned char>	&srcImage,
							TImage<unsigned char >	&dstImage,
							vector<RECT>			&rectSet, 
							const	size_t			radius)
{
	TBmpInfo info;
	info.Width	(srcImage.Width());
	info.Height	(srcImage.Height());
	info.AccordMem();
	
	TBmpBoard bmpBoard;
	bmpBoard.SetBmpInfo(info);
	PixelMem *pPixel = bmpBoard.GetPixelBase();
	memcpy(pPixel, dstImage.Base(), dstImage.Width() * dstImage.Height());
	HDC memHDC = bmpBoard.GetMemDC();
	ArcTemple uptemp	= ArcCreator::CreateArc(radius, ArcTemple::eUpArc);
	ArcTemple downTemp	= ArcCreator::CreateArc(radius, ArcTemple::eDownArc);
	TPoint2D<int>	upCentre;
	TPoint2D<int>	downCentre;
	for(size_t i = 0; i < rectSet.size(); ++i)
	{
		long	upSpeciSide		= rectSet[i].top + (rectSet[i].bottom - rectSet[i].top)/3;
		long	downSpeciSide	= rectSet[i].top + (rectSet[i].bottom - rectSet[i].top)*2/3;
		TRect2D<int>	selectUpArea(rectSet[i].left, rectSet[i].top, rectSet[i].right-1, upSpeciSide);
		TRect2D<int>	selectDownArea(rectSet[i].left, downSpeciSide, rectSet[i].right, rectSet[i].bottom);
		GetTopPos(srcImage, selectUpArea, upCentre, uptemp);
		if (!selectUpArea.Within(upCentre.x(), upCentre.y()))
		{
			upCentre = selectUpArea.Center();
		}
		GetTopPos(srcImage, selectDownArea, downCentre, downTemp);
		if (!selectDownArea.Within(downCentre.x(), downCentre.y()))
		{
			downCentre = selectDownArea.Center();
		}
		
		FillDest(dstImage, upCentre, downCentre, memHDC, radius);
	}
	
	memcpy(dstImage.Base(), pPixel, dstImage.Width()* dstImage.Height());
}

void TTransRemain::GetTopPos(	TImage<unsigned char>		&image,
						TRect2D<int>				&rect, 
						TPoint2D<int>				&centre,
						ArcTemple					&temp)
{
	ArcMatchor matchor;
	matchor.SetImage(image);	
	matchor.GetMaxMatchPos(temp, rect, centre);
}

void TTransRemain::FillDest(	TImage<unsigned char>		&image,
								TPoint2D<int>				&upCentre, 
								TPoint2D<int>				&downCentre,
								HDC							&memHDC,
								const size_t				radius)
{	
	CBrush	bush;
	bush.CreateHatchBrush(HS_CROSS, RGB(255,255,255));
	
	CRgn upRgn;
	upRgn.CreateEllipticRgn(	upCentre.x()- radius, upCentre.y()-radius, 
								upCentre.x()+ radius, upCentre.y()+radius);	
	FillRgn(memHDC , upRgn, bush);
	CRgn downRgn;
	downRgn.CreateEllipticRgn(	downCentre.x()-radius, downCentre.y()-radius,
								downCentre.x()+radius, downCentre.y()+radius);
	FillRgn(memHDC,downRgn, bush);
	CRgn midRgn;
	POINT	midRect[4];
	midRect[0].x = upCentre.x()-radius;
	midRect[0].y = upCentre.y();
	midRect[1].x = downCentre.x()-radius;
	midRect[1].y = downCentre.y();
	midRect[2].x = downCentre.x()+radius;
	midRect[2].y = downCentre.y();
	midRect[3].x = upCentre.x()+radius;
	midRect[3].y = upCentre.y();	
	
	midRgn.CreatePolygonRgn(midRect, 4, ALTERNATE);
	FillRgn(memHDC, midRgn, bush);	
}

bool	TTransRemain::BlackDetect(		IMG					&image,
										const size_t		sizeLimit)
{
	//static FBLOB blob = FBlobCreate(image, 0);
	
	FBlobSetImage				(m_blackBlob, image, 0);
	FBlobSetObjectFeatureRange	(m_blackBlob, 0, 128);
	FBlobSetLimitArea			(m_blackBlob, sizeLimit, -1);
	FBlobExec					(m_blackBlob);
	
	long blobCount = 0;
	FBlobGetNumBlobs(m_blackBlob, blobCount);
	
	if (blobCount)	return true;
	else			return false;
}

void	TTransRemain::ExcludeEmpty(	vector<RECT>	&rectSet, 
									const int		imgHeight,
									const int		imgWidth)
{
	//取凹槽的最大高度
	int maxHeight = 0;
	for(size_t i = 0 ; i< rectSet.size(); ++i)
	{
		int dY = rectSet[i].bottom- rectSet[i].top;
		if (dY > maxHeight)
		{
			maxHeight = dY;
		}
	}
	//去掉与边界接触的区域
	size_t rectCount = rectSet.size();
	for(size_t i = 0; i < rectCount; ++i)
	{
		if (rectSet.at(i).left == 0||rectSet.at(i).right >= (imgWidth - 1))
		{
			rectSet.erase(rectSet.begin() + i);
			rectCount--;
			i--;
		}
	}

	//去掉与上下边界接触的区域
	rectCount = rectSet.size();
	vector<RECT>	zeroRect;
	for(size_t i = 0; i < rectCount; ++i)
	{		
		if (rectSet.at(i).top == 0||rectSet.at(i).bottom >= (imgHeight - 1))
		{
			zeroRect.push_back(rectSet.at(i));
			rectSet.erase(rectSet.begin() + i);
			rectCount--;
			i--;
		}
	}

	//去掉链板间间隙的区域
	rectCount = rectSet.size();
	size_t zeroRectCount = zeroRect.size();
	for(size_t i = 0; i < rectCount; ++i)
	{
		int centreX = (rectSet.at(i).left + rectSet.at(i).right)/2;
		for(size_t j = 0; j < zeroRectCount; ++j)
		{
			if (((rectSet.at(i).left>= zeroRect.at(j).left)&&(rectSet.at(i).left <= zeroRect.at(j).right))||
				((rectSet.at(i).right >= zeroRect.at(j).left)&&(rectSet.at(i).right <= zeroRect.at(j).right))||
				((centreX >= zeroRect.at(j).left)&&(centreX <= zeroRect.at(j).right)))
			{
				zeroRect[j].left = zeroRect[j].left>rectSet[i].left?rectSet[i].left:zeroRect[j].left;
				zeroRect[j].right = zeroRect[j].right>rectSet[i].right?zeroRect[j].right:rectSet[i].right;
				rectSet.erase(rectSet.begin() + i);
				rectCount--;
				i--;
				break;
			}
		}
	}

	//去掉上下匹配错误的区域
	rectCount = rectSet.size();
	for(size_t i = 0; i < rectCount; ++i)
	{
		int centreX = (rectSet.at(i).left + rectSet.at(i).right)/2;
		for(size_t j = i+1; j < rectCount; ++j)
		{
			RECT curRect = rectSet.at(i);
			RECT dstRect = rectSet.at(j);
			if (((curRect.left>= dstRect.left)&&(curRect.left <= dstRect.right))||
				((curRect.right >= dstRect.left)&&(curRect.right <= dstRect.right))||
				((dstRect.left >= curRect.left)&&(dstRect.left<= curRect.right))||
				((dstRect.right>=curRect.left)&&(dstRect.right<= curRect.right))||
				((centreX >= dstRect.left)&&(centreX <= dstRect.right)))
			{
				rectSet[j].left		= rectSet[j].left	>rectSet[i].left?rectSet[i].left:rectSet[j].left;
				rectSet[j].right	= rectSet[j].right	>rectSet[i].right?rectSet[j].right:rectSet[i].right;
				rectSet[j].top		= rectSet[j].top	>rectSet[i].top?rectSet[i].top:rectSet[j].top;
				rectSet[j].bottom	= rectSet[j].bottom > rectSet[i].bottom?rectSet[j].bottom:rectSet[i].bottom;
				rectSet.erase(rectSet.begin() + i);
				rectCount--;
				i--;
				break;
			}
		}
		
	}
	
	//去掉没有胶囊的凹槽的区域
	rectCount = rectSet.size();
	for(size_t i = 0; i < rectCount; ++i)
	{
		bool	isEmpty = (m_mode == TTransRemain::TRANSMODE)?
			IsEmpty1(m_rawIMG, rectSet.at(i), maxHeight/10):IsEmpty(m_rawIMG, rectSet.at(i));
							
		if (isEmpty)
		{
			rectSet.erase(rectSet.begin() + i);
			rectCount--;
			i--;
		}	
	}
}

void	TTransRemain::BlobInit	(	IMG			&image,
									FBLOB		&blob)
{
	if (blob)
	{
		ReleaseImage(blob);
	}
	blob = FBlobCreate(image, 0);
	FBlobSetImage(blob, image, 0);
	FBlobExec(blob);
}


bool	TTransRemain::DynamicEdge(	IMG				&image,
									TArea			&area,
									TEdgeResult		&edgeResult,
									double			&thres,
									double			termValue)
{
	const size_t		density = 1000;
	const double		stepLen = 0.3;
	TEdgeResult			resArr[256];

	RECT				roi;
	roi.left	=	TToInteger::Floor(area.X0);
	roi.right	=	TToInteger::Floor(area.X2);
	roi.top		=	TToInteger::Floor(area.Y0);
	roi.bottom	=	TToInteger::Floor(area.Y1);

	//FillRect( image, roi);
	TCoordinateMap cm;
	InitCoordinateMap(cm);
	SetImageCoordinates (image, cm); 
	SetImageOrigin(image, 0, 0);

	for (thres = 3.0f;  thres < termValue; thres += stepLen)
	{
		long	edgeCount = 0;
 		if (CFindAllEdges(image, 0, density, area, thres, FALSE, 256, resArr, edgeCount))
 		{
 			if ( 1 == edgeCount )
 			{ 				return true; 			}
 		}		
		else
		{			return false;					}
	}
	return false;
}

bool	TTransRemain::EdgeJudge	(	IMG							&image,
									RECT						&rect,	
									RECT						&edgeRect	)
{	
	const double	termValue = 15.0f;
	TArea			area;
	TEdgeResult		edgeResult;
	double			thres;

// 	SetArea(	rect.left + edgeRect.left, rect.top + edgeRect.top, rect.left +edgeRect.right, rect.top +edgeRect.bottom,
// 				(rect.left +edgeRect.left + rect.left + edgeRect.right)/2, rect.top + edgeRect.top, area);
	SetArea(	edgeRect.left, edgeRect.top, edgeRect.left, edgeRect.bottom, (edgeRect.left + edgeRect.right)/2, edgeRect.top, area);
	if (DynamicEdge(image, area, edgeResult, thres, termValue)) return true;
	else
	{
//		SetArea(	rect.left + edgeRect.right, rect.top + edgeRect.top, rect.left + edgeRect.right,rect.top + edgeRect.bottom, 
//				(rect.left + edgeRect.left + rect.left + edgeRect.right)/2, rect.top + edgeRect.top, area);
		SetArea(	edgeRect.right, edgeRect.top, edgeRect.right, edgeRect.bottom, (edgeRect.left + edgeRect.right)/2, edgeRect.top, area);
		if (DynamicEdge(image, area, edgeResult, thres, termValue)) return true;
	}
	return false;
}

void	TTransRemain::SetArea	(	int					x0,
									int					y0,
									int					x1,
									int					y1,
									int					x2,
									int					y2,
									TArea				&area)
{
	area.X0 = x0;
	area.X1 = x1;
	area.X2 = x2;
	area.Y0 = y0;
	area.Y1 = y1;
	area.Y2 = y2;
}

void	TTransRemain::FillRect		(	IMG							&image,
										 RECT						&rect)
{
	unsigned char *pImage	= NULL;
	PVPAT			VPA		= NULL;
	GetImageVPA(image, 0, (void**)&pImage, &VPA);

	const size_t rectWidth = rect.right - rect.left;
	const size_t rectHeight = rect.bottom - rect.top;

	for (int i = rect.top; i < rect.bottom; ++i )
	{
		for(int j = rect.left; j < rect.right; ++j)
		{
			*(VPA[j].XEntry + pImage + VPA[i].YEntry) = 255;
		}
	}
}