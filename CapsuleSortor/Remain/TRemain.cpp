// TRemain.cpp: implementation of the TRemain class.
//
//////////////////////////////////////////////////////////////////////

#include "TRemain.h"
#include "TRect2D.h"
#include "TimgProcess.h"
#include "afxwin.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TRemain::TRemain():
m_emptyBlob(NULL)
,m_whiteBlob(NULL)
{

}

TRemain::~TRemain()
{

}

void	TRemain::ReInit(IMG &image)
{
	if (m_rawIMG)	ReleaseImage(m_rawIMG);

	size_t imageWidth = ImageWidth(image);
	size_t imageHeight = ImageHeight(image);

	TRect2D<int>	bound(0, 0, imageWidth-1, imageHeight-1);
	m_rawImage.ReInit(bound);
	m_sobelImage.ReInit(bound);
	CreateImageFromPointer(	m_rawImage.Base(), m_rawImage.Width()*m_rawImage.Height(), m_rawImage.Width(), m_rawImage.Height(),
							1, 8, 1, m_rawImage.Width(), 1, NULL, NULL, NULL, m_rawIMG);
	ReInitCVBTool(m_rawIMG);
}

void	TRemain::ReInitCVBTool(IMG &image)
{
	if (m_emptyBlob) ReleaseImage(m_emptyBlob);
	if (m_whiteBlob) ReleaseImage(m_whiteBlob);
	m_emptyBlob = FBlobCreate(image, 0);
	m_whiteBlob = FBlobCreate(image, 0);
	WhiteBlob(image, m_emptyBlob);
	WhiteBlob(image, m_whiteBlob);
}

bool	TRemain::RemainCapsule	(	const TAlloc<PixelMem>&	rawImg, 
									const TImgDim&			rawImgDim, 
									TImage<PelGray8>&		remain,									
									RemainParam&			remainParam,
									bool					frontLight)
{
	if(rawImgDim.bytesPerPixel == 3)
	{
		TImage<PelRGB24> rgb(rawImg, rawImgDim);
		if(frontLight)	{	TImgProcess::RGBToGray (rgb, m_rawImage);	}
		else						{	TImgProcess::RGBToGray2(rgb, m_rawImage); }
	}
	else
	{
		m_rawImage.SetImage(rawImg.Base(), rawImgDim);
	}
	
	//memcpy(m_rawImage.Base(), rawImg.Base(), m_rawImage.Width()*m_rawImage.Height());
	remain.SetZero();
	WhiteBlob(m_rawIMG, m_whiteBlob);
	vector<RECT> rectSet = GetMouldRect(m_whiteBlob);
	TImgProcess::SobelFilter(m_rawImage, m_sobelImage);
	int rectCount = rectSet.size();
	for(int i = 0; i < rectCount; ++i)
	{
		if (IsEmpty(m_rawIMG, rectSet.at(i)))
		{
			rectSet.erase(&rectSet[i]);
			rectCount--;
			i--;
		}	
	}		
	CheckPosition(m_sobelImage, remain, rectSet, remainParam.radius);
	int rcco = rectSet.size();	
	TImgProcess::AndGrayImg(m_rawImage, remain, remain);

	return true;
}

void	TRemain::WhiteBlob(IMG &image, FBLOB &blob)
{
	FBlobSetImage(blob, image, 0);
	FBlobSetObjectFeatureRange(blob, 200,255);
	FBlobSetObjectTouchBorder(blob, FBLOB_BORDER_ALL);
	FBlobSetLimitArea(blob, 10, -1);
	FBlobSetSortMode(blob, FBLOB_SORT_POSX, 0, 0, 0, FBLOB_SORT_RISING);
	FBlobExec(blob);
}

vector<RECT>&	TRemain::GetMouldRect(FBLOB& blob)
{
	long	blobCount = 0;
	FBlobGetNumBlobs(blob, blobCount);
	long	maxWidth = 0;
	for(int i = 0; i < blobCount; ++i)
	{
		long startX, startY, dX, dY;
		FBlobGetBoundingBox(blob, i, startX, startY, dX, dY);
		if (dX > maxWidth)
		{
			maxWidth = dX;
		}
	}	
	
	static vector<RECT>	rectSet;
	rectSet.clear();
	const long	widthThres = maxWidth/4;
	RECT 	blobRect;
	memset(&blobRect, 0X00, sizeof(RECT));
	for( i = 0; i < blobCount; ++i)
	{
		long startX = 0, startY = 0, dX = 0, dY = 0; 
		FBlobGetBoundingBox(blob, i, startX, startY, dX, dY);
		if (0 == i)
		{
			blobRect.left	= startX;
			blobRect.top	= startY;
			blobRect.right	= startX + dX;
			blobRect.bottom = startY + dY;
		}
		else
		{
			if (( startX - blobRect.right)<widthThres)
			{
				blobRect.left	= (blobRect.left < startX)?blobRect.left:startX;
				blobRect.top	= (blobRect.top	< startY)?blobRect.top:startY;
				blobRect.right	= (blobRect.right > (startX+dX))?blobRect.right:(startX+dX);
				blobRect.bottom = (blobRect.bottom >(startY+dY))?blobRect.bottom:(startY+dY);
			}
			else
			{
				rectSet.push_back(blobRect);
				blobRect.left	= startX;
				blobRect.top	= startY;
				blobRect.right	= startX+dX;
				blobRect.bottom = startY+dY;
			}
		}
	}
	rectSet.push_back(blobRect);
	return rectSet;
}

bool	TRemain::IsEmpty(IMG &image, RECT& rect)
{
	size_t width	= rect.right	- rect.left;
	size_t height	= rect.bottom	- rect.top;
	size_t centreX	= (rect.left	+ rect.right)/2;
	size_t centreY	= (rect.top		+ rect.bottom)/2;
	size_t testXRad = width / 4;
	size_t testYRad = height /4;
	
	FBlobSetImage				(m_emptyBlob, image, 0);
	FBlobSetObjectFeatureRange	(m_emptyBlob, 0, 200);
	FBlobSetArea				(m_emptyBlob, 0, centreX - testXRad, centreY -testYRad, centreX+testXRad, centreY + testYRad);
	FBlobSetObjectTouchBorder	(m_emptyBlob, FBLOB_BORDER_NONE);
	FBlobSetLimitArea			(m_emptyBlob, 10, -1);
	FBlobExec					(m_emptyBlob);

	long blobCount = 0;
	FBlobGetNumBlobs(m_emptyBlob, blobCount);
	if (blobCount)	return false;
	else			return true;
}

void TRemain::CheckPosition(	TImage<unsigned char>	&srcImage,
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
	for(int i = 0; i < rectSet.size(); ++i)
	{
		TRect2D<int>	selectUpArea(rectSet[i].left, rectSet[i].top, rectSet[i].right-1, rectSet[i].top + (rectSet[i].bottom - rectSet[i].top)/2);
		TRect2D<int>	selectDownArea(rectSet[i].left, rectSet[i].top + (rectSet[i].bottom - rectSet[i].top)/2, rectSet[i].right, rectSet[i].bottom);
		GetTopPos(srcImage, selectUpArea, upCentre, uptemp);		
		GetTopPos(srcImage, selectDownArea, downCentre, downTemp);		
		FillDest(dstImage, upCentre, downCentre, memHDC, radius);
	}
	
	memcpy(dstImage.Base(), pPixel, dstImage.Width()* dstImage.Height());
}

void TRemain::GetTopPos(	TImage<unsigned char>		&image,
							TRect2D<int>				&rect, 
							TPoint2D<int>				&centre,
							ArcTemple					&temp)
{
	ArcMatchor matchor;
	matchor.SetImage(image);	
	matchor.GetMaxMatchPos(temp, rect, centre);
}

void TRemain::FillDest(	TImage<unsigned char>		&image,
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