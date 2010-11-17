#include <emmintrin.h>
#include "CapsuleProc.h"
#include "HorizonExtend.h"
#include "Translation.h"

#include "TTimediff.h"


CapsuleProc::ESaveState	CapsuleProc::m_eSaveState	= eNoSave;
size_t					CapsuleProc::m_allCount		= 0;	//ȫ����������
size_t					CapsuleProc::m_badCount		= 0;	//��Ʒ��������
SortObserver				CapsuleProc::m_sortObserver = SortObserver::TheSortObserver();		//���Թ۲⹤��
CapsuleProc::CapsuleProc()
: m_cvbImg(0),
  m_cvbProfileImg(0),
  m_profileBlob(0),
  m_cvbBlob(NULL),
  m_lightMeter(0)
{ }

CapsuleProc::~CapsuleProc()
{
	ReleaseImage(m_cvbImg);
	ReleaseImage(m_cvbProfileImg);
	ReleaseImage(m_profileBlob);
	ReleaseImage(m_cvbBlob);
	LMDestroy(m_lightMeter);
}

IMG  CapsuleProc::ObservedIMG( )
{
	return m_sortObserver.GetImage();
}

bool  CapsuleProc::ObservedIMG( SortObserver::ETarget eTarget, IMG img )
{
	return m_sortObserver.ObserverIMG(	eTarget, img);
}

double CapsuleProc::ObservedParam( )
{
	return m_sortObserver.GetParam();
}

void CapsuleProc::ObservingTarget(unsigned int spec)
{
	m_sortObserver.SpecifyTarget(spec);
}

/***************************************************************
*�������ƣ�	ReInit
*
* ������	TImgDim		ͼ��ߴ���Ϣ
*
* ����ֵ��	�ɹ� true, ʧ�� false
*  
* ���ܣ�	��ʼ��ͼ������

****************************************************************/
bool CapsuleProc::ReInit (const TImgDim& imgDimension)
{
	if(m_dim == imgDimension)	return true;

	m_dim = imgDimension;
	ReleaseImage(m_cvbImg);
	ReleaseImage(m_cvbProfileImg);
	ReleaseImage(m_profileBlob);
	ReleaseImage(m_cvbBlob);
	LMDestroy(m_lightMeter);


	m_imgBuffer.ReInit(m_dim, 30);

	m_rawImage	= TAlloc<PixelMem>(m_dim.bytesPerRow * m_dim.height,  e16ByteAlign);

	long pitchPlane = (m_dim.bytesPerPixel == 1) ? 0 : 1;

	if(SUCCESS != CreateImageFromPointer(	m_rawImage.Base(), 
											m_rawImage.Size(), 
											m_dim.width, 
											m_dim.height,
											m_dim.bytesPerPixel, 8, m_dim.bytesPerPixel, 
											m_dim.bytesPerRow, 
											pitchPlane, 0, 0, 0, 
											m_cvbImg))
	{
		return false;
	}

	TRect2D<int> boundary(TPoint2D<int>(0, 0), m_dim.width, m_dim.height);
	m_profileImage = TImage<PelGray8> (boundary, e16ByteAlign);
	if(SUCCESS != CreateImageFromPointer(	m_profileImage.Base(), 
											m_profileImage.RowSpace() * m_profileImage.Height(),
											m_profileImage.Width(),
											m_profileImage.Height(),
											1, 8, 1, 
											m_profileImage.RowSpace(),
											0, 0, 0, 0, 
											m_cvbProfileImg))
	{
		return false;
	}

	m_cvbBlob		= FBlobCreate(m_cvbImg,			0);
	m_profileBlob	= FBlobCreate(m_cvbProfileImg,	0);

	if(SUCCESS != LMCreate(&m_lightMeter))
	{
		return false;
	}
	return true;
}

/***************************************************************
*�������ƣ�	SetRawImage
*
* ������	pMem		ԭʼͼ���ڴ�ָ��
*			bytes		ԭʼͼ���С
*
* ����ֵ��	�ɹ�	true
*			ʧ��	false	  
*
* ���ܣ�	����ͼ���ڴ�����

****************************************************************/
bool CapsuleProc::SetRawImage(void* pMem, size_t bytes)
{
	bool success = false;
	try
	{
		if ((0!=pMem) && (m_rawImage.Size()==bytes))
		{
			memcpy(m_rawImage.Base(), pMem, m_rawImage.Size());
			success = true;
		}
	}
	catch(...)
	{
		FILE* pFile = NULL;
		fopen_s(&pFile,"D:\\SetRawImage.txt", "wb");
		fwrite("SetRawImage error", sizeof(char), strlen("SetRawImage error"), pFile);
		fclose(pFile);
	}
	return success;
}
/***************************************************************
*�������ƣ�	GetRawImage
*
* ������	��
*
* ����ֵ��	ԭʼͼ����	  
*
* ���ܣ�	�õ�ԭʼͼ��

****************************************************************/
IMG	 CapsuleProc::GetRawImage( )
{
	return m_cvbImg;
}
/***************************************************************
*�������ƣ�	CheckPosition
*
* ������	posX	��ǰ���X����
*			posY	��ǰ���Y����
*
* ����ֵ��	��ǰ������ͼ���е����	  
*
* ���ܣ�	�õ���ǰ������ͼ���е���ţ�posX,posYΪ���ҵ�������ͼ���е�����

****************************************************************/
int	CapsuleProc::CheckPosition(long posX, long posY)
{
	int interval = m_dim.width / SUBIMGNUM;
	return posX / interval;
}

/***************************************************************
*�������ƣ�	IsCapsuleShort
*
* ������	stdHeight	��׼�߶�
*			realHeight	ʵ�ʸ߶�
*			tolerance	�߶��ݲ�
*
* ����ֵ��	�ɹ�	true
*			ʧ��	false
*				  
*
* ���ܣ�	�жϽ����Ƿ����

****************************************************************/
bool CapsuleProc::IsCapsuleShort(	size_t realHeight,
									size_t realWidth,
									size_t stdHeight,
									size_t tolerance)
{	
	bool isShort = (realHeight) < (stdHeight - tolerance);
	bool isNarrow	= realWidth < m_capsuleParam.capsuleDim.width/2;
	return isShort || isNarrow;
}
/***************************************************************
*�������ƣ�	FindOverlapCenter
*
* ������	rawSubImg		ԭʼ��ͼ��
*			centerX			�׺���������
*			centerY			�׺���������
*
* ����ֵ��	�ɹ�	true
*			ʧ��	false
*				  
*
* ���ܣ�	�õ������׺����е������

****************************************************************/

bool CapsuleProc::FindOverlapCenter(IMG		rawSubImg,
									long&	centerX,
									long&	centerY)
{
	if(!IsImage(rawSubImg))
	{
		centerX = -1;
		centerY = -1;
		return false;
	}
	
	long width	= ImageWidth (rawSubImg);
	long height = ImageHeight(rawSubImg);
	
	long xc		= width/2;
	long yc0	= height/2 - height/8;
	long yc1	= height/2 + height/8;
	
	double upMean	= 0;
	double downMean = 0;
	
	long offsetX = width/4;
	long offsetY = 5;
	
	TArea upArea = CreateTArea(xc - offsetX,   yc0 + offsetY, 2*offsetX, 2*offsetY);
	
	LMSetImage			(m_lightMeter, rawSubImg);
	LMSetProcessFlag	(m_lightMeter, 0,  TRUE);
	LMSetArea			(m_lightMeter, 0,  upArea);
	LMExecute			(m_lightMeter);
	LMGetStatisticMean	(m_lightMeter, 0, &upMean);
	
	
	TArea downArea = CreateTArea(xc - offsetX, yc1 - offsetY, 2*offsetX, -2*offsetY);
	LMSetImage			(m_lightMeter, rawSubImg);
	LMSetProcessFlag	(m_lightMeter, 0,  TRUE);
	LMSetArea			(m_lightMeter, 0,  downArea);
	LMExecute			(m_lightMeter);
	LMGetStatisticMean	(m_lightMeter, 0, &downMean);
	
	centerX = xc;
	centerY = upMean <= downMean ? yc0 : yc1;
	return true;
}
/***************************************************************
*�������ƣ�	FindFirstEdgeY
*
* ������	image			��ͼ��
*			area			���ؼ������
*			density			���ؼ���ܶ�
*			threshold		��ֵ��С
*
* ����ֵ��	���ص������
*				  
*
* ���ܣ�	�õ����ҵı��صĵ������

****************************************************************/
long CapsuleProc::FindFirstEdgeY(	IMG image, TArea area, long density, double threshold)
{
	long yPos = -1;
	TEdgeResult edge;
	if(CFindFirstEdge (image, 0, density, area, threshold, TRUE, edge))
	{
		yPos = long(edge.y);
	}
	return  yPos;	
}

/***************************************************************
*�������ƣ�	FindUpDownEdge
*
* ������	image			��ͼ��
*			density			���ؼ����ܶ�
*			threshold		��ֵ
*			upPos			�ϱ���
*			downPos			�±���
*
* ����ֵ��	�ɹ�	true
*			ʧ��	false
*				  
*
* ���ܣ�	�õ������׺��������±��ص�����

****************************************************************/
bool CapsuleProc::FindUpDownEdge(	IMG image, long density,double threshold, long &upPos, long &downPos)
{
	long	overX =0, overY =0;
	FindOverlapCenter(image, overX, overY);
	long	roiWidth	= ImageWidth(image)/2;
	long	roiHeight	= ImageHeight(image)/3;
	
	TArea	areaUp		= CreateTArea(overX -roiWidth/2, overY, roiWidth, 	roiHeight);
	long	yUp			= FindFirstEdgeY(image, areaUp, density,threshold);
	
	TArea	areaDown	= CreateTArea(overX -roiWidth/2, overY, roiWidth, 	-roiHeight);
	long	yDown		= FindFirstEdgeY(image, areaDown, density, threshold);
	upPos				= yUp;
	downPos				= yDown;
	return	((upPos != -1) && (downPos != -1));
}

/***************************************************************
*�������ƣ�	CreateTArea
*
* ������	orgX			���ϽǺ�����
*			orgY			���Ͻ�������
*			width			ͼ����
*			height			ͼ��߶�
*
* ����ֵ��	TArea����
*				  
*
* ���ܣ�	����ͼ��TArea����

****************************************************************/
TArea CapsuleProc::CreateTArea(	long orgX,
								long orgY, 
								long width, 
								long height)
{
	TArea area;

	area.X0 = orgX;
	area.Y0 = orgY;
	area.X1 = orgX + width;
	area.Y1 = orgY;
	area.X2 = orgX;
	area.Y2 = orgY - height;

	return area;
}

/***********************************************************
  Name:    ShrinkHorizontal
  Input:   image,          shrinkNum
  Output:  ifsuccess
  Description:   ��ͼ��ˮƽ����shrinkNum������ 
************************************************************/

bool CapsuleProc::ShrinkHorizontal(	IMG				image, 
									const size_t	shrinkNum)
{
	unsigned char	*pImageBit = NULL;
	PVPAT			VPA = NULL;
	
	GetImageVPA	(image, 0, (void**)&pImageBit, &VPA);
	
	const size_t imageWidth		= ImageWidth (image);
	const size_t imageHeight	= ImageHeight(image);
	const size_t halfWidth		= imageWidth / 2;

	const unsigned char  cstVal	= 127; 
	unsigned char*		 curVal	= 0;

	if (halfWidth < shrinkNum)	{	return false;	}

	for(size_t veCo = 0; veCo < imageHeight; ++veCo)
	{
		for(size_t i = 0; i <= halfWidth; ++i)
		{
			curVal = (VPA[veCo].YEntry + pImageBit + VPA[i].XEntry);
			if ( cstVal == *curVal)	{	*curVal = 0;	}
			else if(0 == *curVal)	{	continue;		}
			else
			{
				for(size_t j = 0; j < shrinkNum; ++j)
				{				
					*(VPA[veCo].YEntry + pImageBit + VPA[j + i].XEntry) = 0;
				}
				break;
			}
		}

		for(size_t i = imageWidth; i >= halfWidth; --i)
		{
			curVal = (VPA[veCo].YEntry + pImageBit + VPA[i].XEntry);
			if (cstVal == *curVal)	{	*curVal = 0;	}
			else if(0 == *curVal)	{	continue;		}
			else
			{
				for(size_t j = 0; j < shrinkNum; ++j)
				{				
					*(VPA[veCo].YEntry + pImageBit + VPA[i - j].XEntry) = 0;
				}
				break;
			}
		}
	}	
	return true;
}

/***************************************************************
*�������ƣ�	ShrinkVertical
*
* ������	image			���ͼ��
*			shrinkNum		������������ֵ
*����ֵ��	�ɹ�	true 
*			ʧ��	false
*  
* ���ܣ�	�Խ��ҵ�ͼ��������������
****************************************************************/
bool CapsuleProc::ShrinkVertical(	IMG				image,
									const size_t	shrinkNum)
{
	unsigned char	*pImageBit = NULL;
	PVPAT			VPA = NULL;
	
	GetImageVPA	(image, 0, (void**)&pImageBit, &VPA);
	
	const size_t imageWidth		= ImageWidth (image);
	const size_t imageHeight	= ImageHeight(image);
	const size_t halfHeight		= imageHeight/2;
	
	if (halfHeight < shrinkNum)		{	return false;	}

	const unsigned char  cstVal	= 127; 
	unsigned char*		 curVal	= 0;

	for(size_t i = 0; i < imageWidth; ++i)
	{
		for(size_t j = 0; j <= halfHeight; ++j)
		{
			curVal = (VPA[i].XEntry + pImageBit + VPA[j].YEntry);
			if (cstVal == *curVal)	{  *curVal = 0;	}
			else if (0 == *curVal)	{	continue;	}
			else
			{
				for(size_t eroCount = 0; eroCount < shrinkNum; ++eroCount)
				{
					*(VPA[i].XEntry + pImageBit + VPA[eroCount + j].YEntry) = 0;
				}
				break;
			}
		}

		for(size_t j = imageHeight - 1; j >= halfHeight; --j)
		{
			curVal = (VPA[i].XEntry + pImageBit + VPA[j].YEntry);
			if (cstVal == *curVal)	{  *curVal = 0;	}
			else if (0 == *curVal)	{	continue;	}
			else
			{
				for(size_t eroCount = 0; eroCount < shrinkNum; ++eroCount)
				{
					*(VPA[i].XEntry + pImageBit + VPA[ j-eroCount].YEntry) = 0;
				}
				break;
			}
		}
	}
	return true;
}


/*******************************************************************************
*	�������ƣ�	RotateIMG
*	
*	����	��[IN]	blobIndex		�������	
*			  [IN]  fluteBlob		���۰ߵ�
*			  [IN]	profileBlob		���������ߵ�
*			  [IN]	rawImg			���н��Ұߵ���Ϣ��ͼ��
*			  [IN]	proImg			���н���������Ϣ��ͼ��
*			  [OUT]	&rawSubIMG		���н��Ұߵ���Ϣ����ͼ��
*			  [OUT]	&proSubIMG		���н���������Ϣ����ͼ��
*			  [OUT]	&posIndex		�������
*			  
*
*	����	����ͼ����зָȡ�������ڵ�С�����򣬴˺���������ͼ�������ת
*
*
********************************************************************************/
bool CapsuleProc::RotateIMG(	size_t	blobIndex,
								FBLOB	profileBlob,
								IMG		proImg,
								IMG		&proSubIMG,
								size_t	&posIndex)
{
	long profileCount = 0;
	FBlobGetNumBlobs	(profileBlob, profileCount);
	if ( blobIndex >= static_cast<size_t>(profileCount) )
	{
		posIndex = -1;
		return false;
	}

	//Get Range
	long	x0 = 0,		y0 = 0,		width = 0,	height = 0;	
	FBlobGetBoundingBox	(profileBlob, blobIndex, x0, y0, width, height);
	posIndex = CheckPosition(x0+width/2, y0+height/2);

	IMG		proIMGMap	= NULL;
	CreateImageMap	(proImg,  x0, y0, x0+width, y0+height, width, height,	proIMGMap);

	double	rabi = 0.0, rabi1 = 0.0, rabi2 = 0.0, momentAngle = 0.0;
	FBlobGetMoments	(profileBlob, blobIndex, rabi, rabi1, rabi2, momentAngle);

	static const double ADIVPI		= 180.0 /(4.0*atan(1.0));	//ppi	= 180/PI
	static const double ANGLE90		= 90;					//ppiH	= 180/PI*PI/2

	double angle = 0.0;
	if(momentAngle>=0)
	{	angle =  (ANGLE90 - ADIVPI * momentAngle);	}
	else 
	{	angle = -(ANGLE90 + ADIVPI * momentAngle);	}
	
	RotateImage(	proIMGMap,	angle,	IP_Linear,	proSubIMG);	

	TCoordinateMap cm;
	InitCoordinateMap(cm);
	SetImageCoordinates (proSubIMG, cm); 	

	ShrinkVertical	(proSubIMG, m_segmentParam.shrinkY);	
	ShrinkHorizontal(proSubIMG, m_segmentParam.shrinkX);
	
	ReleaseImage	(proIMGMap);
	return true;
}

/***************************************************************
*�������ƣ�	RectangleBlob
*
* ������	IMG						ͼ��
*			TRect2D					����Ȥ����
*			long					�������
*			long					��̬��ֵ
*			long					�ߵ����С���
*			TBlobTouchBorderFilter	���Ա߽�
*			long					�ߵ���С���
*			long					�ߵ���С�߶�
* ����ֵ��	�����ڰߵ�	true, 
*			���ڰߵ�	false
*  
* ���ܣ�	��ָ���������ڲ��ö�̬��ֵ+
*			�ߵ�����ķ�������Ƿ���ڰߵ�
*
****************************************************************/
bool CapsuleProc::RectangleBlob	(	IMG						image,
									const TRect2D<long>&	roi,
									long					wndSize,
									long					dynThres,
									long					minBlobSize,
									TBlobTouchBorderFilter	ignorBorder,
									long					minBlobWidth,
									long					minBlobHeight,
									bool					alsoReverse)
{
	if(!IsImage(image) || roi.IsNull())
	{	return true;	}

	IMG imageROI	= NULL;
	CreateImageMap(image , roi.x0(), roi.y0(), roi.x1(), roi.y1(), roi.Width(), roi.Height(), imageROI);
	bool success = RectBlackBlob(imageROI, wndSize, dynThres, minBlobSize, 
								ignorBorder, minBlobWidth, minBlobHeight);
	if(alsoReverse && success)
	{
		IMG		imageROI2 = NULL;
		long	maskVal[] = { 255 };
		XorConstant(imageROI, maskVal, imageROI2);
		success = RectBlackBlob(imageROI2, wndSize, dynThres, minBlobSize, 
								ignorBorder, minBlobWidth, minBlobHeight);
		ReleaseImage(imageROI2);
	}
	m_sortObserver.ObserverIMG(SortObserver::MPart, imageROI);
	m_sortObserver.ObserverIMG(SortObserver::CPart, imageROI);
	ReleaseImage(imageROI);
	return success;	
}

/***************************************************************
*�������ƣ�	RectBlackBlob
*
* ������	IMG						ͼ��
*			TRect2D					����Ȥ����
*			long					�������
*			long					��̬��ֵ
*			long					�ߵ����С���
*			TBlobTouchBorderFilter	���Ա߽�
*			long					�ߵ���С���
*			long					�ߵ���С�߶�
* ����ֵ��	�����ڰߵ�	true, 
*			���ڰߵ�	false
*  
* ���ܣ�	��ָ���������ڲ��ö�̬��ֵ+
*			�ߵ�����ķ�������Ƿ���ڰߵ�
*
****************************************************************/
bool CapsuleProc::RectBlackBlob(	IMG			image,
									long		wndSize,
									long		dynThres,
									long		minBlobSize,
									TBlobTouchBorderFilter	ignorBorder,
									long		minBlobWidth,
									long		minBlobHeight)
{
	if(!IsImage(image))	{ return true; }

	IMG DynamicImage = NULL;
	CreateDynamicThresholdImage(image, wndSize, dynThres,  DynamicImage);
	
	FBlobSetImage			(m_cvbBlob, DynamicImage, 0);
	FBlobSetObjectFeatureRange	(m_cvbBlob, 0, 128);
	FBlobSetSkipBinarization	(m_cvbBlob, FALSE);
	FBlobSetLimitArea		(m_cvbBlob, minBlobSize, -1);
	FBlobSetLimitWidth		(m_cvbBlob, minBlobWidth, -1);
	FBlobSetLimitHeight		(m_cvbBlob, minBlobHeight, -1);
	FBlobSetObjectTouchBorder	(m_cvbBlob, ignorBorder);
	FBlobExec			(m_cvbBlob);

	long blobCount	= 0;
	FBlobGetNumBlobs(m_cvbBlob, blobCount);

	if(1 == m_dim.bytesPerPixel)
	{
		m_sortObserver.ObserverIMG(SortObserver::MDyn, DynamicImage);
	}
	else
	{
		m_sortObserver.ObserverIMG(SortObserver::CDyn, DynamicImage);
	}
	

	ReleaseImage(DynamicImage);

	return (blobCount == 0);
}

/***************************************************************
*�������ƣ�	BlobAnalysis
*
*  ������	fblob		�ߵ����������
*			temp		����ģ��
*			image		�������ڵĻҶ�ͼ��
*			topX		���������϶���X����
*			topY		���������϶���Y����
*			up			�׺����϶˵�
*			low			�׺����¶˵�
*			bottomY		���������¶���Y����
*����ֵ��	�аߵ� true �ްߵ�false
*  
*���ܣ�		�Խ������ڵĻҶ�ͼ��������аߵ����
****************************************************************/

bool CapsuleProc::BlobAnalysis(	IMG		image,
								long    upEdgeY, 
								long    downEdgeY,
							    long	wndSize,
								long	dynThres,
								long	blobSize)
{
	long x0 = 0, y0 =0, dx = 0, dy = 0;
	ProfileBlob(image, ImageWidth(image)/2, m_cvbBlob);
	FBlobGetBoundingBox(m_cvbBlob, 0, x0, y0, dx, dy);
	y0 = 0;
	dy = ImageHeight(image) - 1;
	//�м䲿��
	TRect2D<long> roi(x0, upEdgeY, x0+dx, downEdgeY);
	roi.Expand		(-10, -6);
	if(false == RectangleBlob(image, roi, wndSize,  dynThres, blobSize, FBLOB_BORDER_ALL, -1, -1, true))
	{	return false;	}

	if (upEdgeY - y0> (y0 + dy) - downEdgeY)
	{
		roi = TRect2D<long>(x0, y0, x0+dx, upEdgeY-15);
		if(false == RectangleBlob(image, roi, wndSize, dynThres, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}		
		
		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy);
		if(false == RectangleBlob(image, roi, wndSize, dynThres+5, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}
	}
	else
	{
		roi= TRect2D<long>(x0, y0, x0+dx, upEdgeY-15);
		if(false == RectangleBlob(image, roi, wndSize, dynThres+5, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}		
		
		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy);
		if(false == RectangleBlob(image, roi, wndSize, dynThres, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}
	}


	
	if (upEdgeY - y0> (y0 + dy) - downEdgeY)
	{
		roi = TRect2D<long>(x0, y0 + (dx/2), x0+dx, upEdgeY-15);
	}
	else
	{
		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy - (dx/2));
	}
	roi.Expand		(-10, -6);
	if(false == RectangleBlob(image, roi, wndSize,  dynThres, blobSize, FBLOB_BORDER_NONE, -1, -1, true))
	{	return false;	}

	return true;
}

/***************************************************************
*�������ƣ�	RectSobelBlob
*
* ������	IMG						ͼ��
*			TRect2D					����Ȥ����
*			long					�������
*			long					��̬��ֵ
*			long					�ߵ����С���
*			TBlobTouchBorderFilter	���Ա߽�
*			long					�ߵ���С���
*			long					�ߵ���С�߶�
* ����ֵ��	�����ڰߵ�	true, 
*			���ڰߵ�	false
*  
* ���ܣ�	��ָ���������ڲ��ö�̬��ֵ+
*			�ߵ�����ķ�������Ƿ���ڰߵ�
*
****************************************************************/
bool CapsuleProc::RectSobelBlob	(	IMG						image, 
									const TRect2D<long>&	roi,
									long					wndSize,
									long					dynThres,
									long					minBlobSize,
									TBlobTouchBorderFilter	ignorBorder,
									long					minBlobWidth,
									long					minBlobHeight)
{
	if(!IsImage(image) || roi.IsNull() )
	{	return true;	}

	bool	success = true;
	IMG	imageROI= NULL;
	CreateImageMap(image , roi.x0(), roi.y0(), roi.x1(), roi.y1(), roi.Width(), roi.Height(), imageROI);
	if(IsImage(imageROI))
	{
		IMG imageSobel = 0;
		FilterSobelVertical(imageROI, FM_5x5, imageSobel);
		success = RectBlackBlob(	imageSobel, 
						wndSize, 
						dynThres, 
						minBlobSize, 
						ignorBorder,
						minBlobWidth,
						minBlobHeight);		
		if(success)
		{
			IMG	imageSobel2	= NULL;
			long	maskVal[]	= { 255 };
			XorConstant		(	imageSobel, 
							maskVal, 
							imageSobel2);
			success = RectBlackBlob	(	imageSobel2,
							wndSize,
							dynThres, 
							minBlobSize,
							ignorBorder,
							minBlobWidth,
							minBlobHeight);
			ReleaseImage(imageSobel2);
		}

		//m_sortObserver.ObserverIMG(SortObserver::MSobel, image);
		//m_sortObserver.ObserverIMG(SortObserver::CSobel, imageSobel);
		ReleaseImage(imageSobel);
	}
	ReleaseImage(imageROI);
	return success;	
}

/***************************************************************
*�������ƣ�	SobelAnalysis
*
* ������	IMG						ͼ��
*			
*			
* ����ֵ��	�����ڰߵ�	true, 
*			���ڰߵ�	false
*  
* ���ܣ�	��ָ���������ڲ���sobel�˲�+
*			�ߵ�����ķ�������Ƿ���ڰߵ�
*
****************************************************************/

bool CapsuleProc::SobelAnalysis(IMG	image)
{
	IMG copyImage = 0;
	CreateDuplicateImage (image, copyImage);
	HorizonExtend::Extend(copyImage);

	long	width = ImageWidth(copyImage);
	long	height= ImageHeight(copyImage);
	TRect2D<long> roi = TRect2D<long>(TPoint2D<long>(0,0), width, height);
	roi.Expand(-2, -2);
				
	bool success = RectSobelBlob(	copyImage,
					roi,
					m_segmentParam.dynWndSize,
					m_segmentParam.dynThres,
					m_segmentParam.blobSize,
					FBLOB_BORDER_ALL);
	ReleaseImage(copyImage);
	return success;
}

/***************************************************************
*�������ƣ�	TransSobelAnalysis
*
* ������	IMG					ͼ��
*			
*			
* ����ֵ��	�����ڰߵ�	true, 
*			���ڰߵ�	false
*  
* ���ܣ�	��ָ���������ڲ���sobel�˲�+
*			�ߵ�����ķ�������Ƿ���ڰߵ�
*
****************************************************************/
bool CapsuleProc::TransSobelAnalysis(IMG	image)
{
	IMG copyImage = 0;
	CreateDuplicateImage (image, copyImage);
	HorizonExtend::Extend(copyImage);
	
	long	width = ImageWidth(copyImage);
	long	height= ImageHeight(copyImage);
	TRect2D<long> roi = TRect2D<long>(TPoint2D<long>(0,0), width, height);
	roi.Expand(-2, -2);
				
	bool success = false;
	IMG		imageROI= NULL;
	CreateImageMap(	copyImage, roi.x0(), roi.y0(), roi.x1(), roi.y1(), roi.Width(), roi.Height(), imageROI);
	if(IsImage(imageROI))
	{
		IMG imageSobel = 0;
		FilterSobelVertical(imageROI, FM_5x5, imageSobel);
		success = RectBlackBlob	(	imageSobel, 
									m_segmentParam.dynWndSize,
									m_segmentParam.dynThres - 2,
									m_segmentParam.blobSize, 
									FBLOB_BORDER_ALL, -1, 4);

		m_sortObserver.ObserverIMG(SortObserver::MSobel, copyImage);
		m_sortObserver.ObserverIMG(SortObserver::CSobel, imageSobel);
		ReleaseImage(imageSobel);
	}
	ReleaseImage(imageROI);
	ReleaseImage(copyImage);
	return success;
}

/*******************************************************************************
*	�������ƣ�	ProfileBlob
*	
*	����	��	blob		�ߵ��������
*			  	temp		���Ҳ���ģ��
*			  
*  ����ֵ	:	��
*
*	����	��	�Խ���Ŀ����аߵ���
*
*
********************************************************************************/
bool CapsuleProc::ProfileBlob(IMG image, long minWidth, FBLOB &blob)
{
	if(!IsImage(image))		return false;
	FBlobSetImage				(blob, image, 0);
	FBlobSetObjectFeatureRange	(blob, 1, 255);
	FBlobSetLimitWidth			(blob, minWidth,  -1);
	FBlobSetSortParameter		(blob, 2, 0, 0, 1);
	FBlobSetSkipBinarization	(blob, FALSE);
	FBlobExec					(blob);
	return true;
}

TImgBuffer CapsuleProc::GetRecordedImage( )
{
	return m_imgBuffer;
}

/**********************************************
  Name:    ClearRecordedImage
  Input:   void
  Output:  void
  Description: ���ͼ�񻺳��� 
***********************************************/
void CapsuleProc::ClearRecordedImage()
{
	m_imgBuffer.SetZero();
}

/**********************************************
  Name:    RecordingImage
  Input:   void
  Output:  void
  Description:   �洢ԭʼͼ�� 
***********************************************/
bool CapsuleProc::RecordingImage()
{
	static PixelMem errFlag[8] = {	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00 };

	unsigned int errPos	= m_sortResult.GetErrorPosition();
	unsigned int errCode	= m_sortResult.GetErrorCode();

	if(eNoSave == m_eSaveState) { return true;	}
	if(eErrSave == m_eSaveState && 0 == errPos)	{ return true; }

	PixelMem* pBase = m_rawImage.Base();
	memcpy(pBase, errFlag, 8);
	pBase += m_dim.bytesPerRow;
	memcpy(pBase, &errPos,  sizeof(unsigned int));
	pBase += m_dim.bytesPerRow;
	memcpy(pBase, &errCode, sizeof(unsigned int));

	m_imgBuffer.PushData(m_rawImage.Base(), m_dim);

	return true;
}

/***************************************************************
*�������ƣ�	SegmentParam
*
* ������	SortorParam				�������
*			
*			
* ����ֵ��	�� 
*			
*  
* ���ܣ�	ΪCapsuleProc�����SortorParam���и�ֵ����
*			
*
****************************************************************/

void CapsuleProc::SegmentParam(const SortorParam& param)
{
	m_segmentParam = param;
}

/***************************************************************
*�������ƣ�	RemainInfo
*
* ������	RemainParam				�������
*			
*			
* ����ֵ��	�� 
*			
*  
* ���ܣ�	ΪCapsuleProc�����RemainParam���и�ֵ����
*			
*
****************************************************************/
void CapsuleProc::RemainInfo(const RemainParam& param)
{
	m_remainParam = param;
}

const RemainParam&			CapsuleProc::RemainInfo		( ) const
{
	return m_remainParam;
}

/***************************************************************
*�������ƣ�	SegmentParam
*
* ������	��
*			
*			
* ����ֵ��	SortorParam		������� 
*			
*  
* ���ܣ�	�õ�Capsule�����SortorParam��Ա����
*			
*
****************************************************************/
const SortorParam& CapsuleProc::SegmentParam( ) const
{
	return m_segmentParam;
}

/***************************************************************
*�������ƣ�	Dimension
*
* ������	��
*			
*			
* ����ֵ��	TImgDim		ͼ���ʽ����
*			
*  
* ���ܣ�	�õ�TImgDim��Ա����
*			
*
****************************************************************/

const TImgDim&	CapsuleProc::Dimension( ) const
{
	return m_dim;
}

/***************************************************************
*�������ƣ�	CapsuleInfo
*
* ������	CapsuleParam	���Ҳ���
*			
*			
* ����ֵ��	��
*			
*  
* ���ܣ�	�õ����Ҳ���
*			
*
****************************************************************/

void CapsuleProc::CapsuleInfo(const CapsuleParam& param)
{
	m_capsuleParam = param;
}

/***************************************************************
*�������ƣ�	CapsuleInfo
*
* ������	��
*			
*			
* ����ֵ��	CapsuleParam	���Ҳ���
*			
*  
* ���ܣ�	�õ����Ҳ���
*			
*
****************************************************************/
const CapsuleParam& CapsuleProc::CapsuleInfo( )
{
	return m_capsuleParam;
}

/***************************************************************
*�������ƣ�	GetSaveState
*
* ������	��
*			
*			
* ����ֵ��	ESaveState	���淽ʽ
*			
*  
* ���ܣ�	�õ�ͼ��ı��淽ʽ
*			
*
****************************************************************/

CapsuleProc::ESaveState CapsuleProc::GetSaveState()
{
	return m_eSaveState;
}

/***************************************************************
*�������ƣ�	SetSaveState
*
* ������	ESaveState	���淽ʽ
*			
*			
* ����ֵ��	ESaveState	���淽ʽ
*			
*  
* ���ܣ�	���ñ��淽ʽ
*			
*
****************************************************************/
CapsuleProc::ESaveState CapsuleProc::SetSaveState(ESaveState NextState)
{
	m_eSaveState = NextState;
	return m_eSaveState;
}

/***************************************************************
*�������ƣ�	TransProcess
*
* ������	��
*			
*			
* ����ֵ��	unsigned int	������
*			
*  
* ���ܣ�	͸�����Ҵ���
*			
*
****************************************************************/
unsigned int CapsuleProc::TransProcess( WORKMODE mode )
{
	TTimeDiff td;
	td.Reset();
	
	m_curData.Clear();
	m_sortResult.NewPeriod();
	m_sortObserver.NewPeriod(SortObserver::Mono);
	
	TImgDim dim = Dimension();
	if (!m_transRemain.RemainCapsule(m_rawImage, dim, m_profileImage, m_remainParam, TTransRemain::TRANSMODE))
	{
		return 0;
	}	

	m_sortObserver.ObserverIMG	(SortObserver::MAll, m_cvbProfileImg);	
	
	long profileBlobCount = 0;
	long minWidth = 1 + m_capsuleParam.capsuleDim.width/8;
	ProfileBlob		(m_cvbProfileImg, minWidth, m_profileBlob);	
	FBlobGetNumBlobs(m_profileBlob, profileBlobCount);	
	
	int counter = 0;
	for(int i = 0; i < profileBlobCount; i++)
	{
		IMG		proSubRot	= NULL;
		size_t  posIndex	= 0;
		if(TransRotateIMG(i, m_profileBlob, m_cvbProfileImg, proSubRot, posIndex))
		{
			if(posIndex ==0 || posIndex == 2)
			{
				counter++;
			}
			m_sortObserver.ObserverIMG(SortObserver::MSub, proSubRot);
			
			const long width = ImageWidth (proSubRot);
			const long height= ImageHeight(proSubRot);
			
			m_curData.WidthAndHeight(width, height);
			
			//all short
			if( IsCapsuleShort(	height, width,
				m_capsuleParam.capsuleDim.height, 
				m_capsuleParam.capsuleDim.tolerance))
			{
				if (REALTIME == mode) 
				{
					m_sortResult.SetResult(SortResult::ShortErr, posIndex);
				//	OutputDebugString("Capsule Short!");
					ReleaseImage(proSubRot);
					continue;
				}
			}

			if (false == TransDoubleJudge(proSubRot, width/2))
			{
				if (REALTIME == mode) 
				{
					m_sortResult.SetResult(SortResult::DoubleEdge, posIndex);
				//	OutputDebugString("Double Edge!");
					ReleaseImage(proSubRot);
					continue;
				}
			}

			long  upEdge = 0, downEdge = 0;
			bool findEdge = FindUpDownEdge(	proSubRot, 
												m_segmentParam.edgeDensity, 
												m_segmentParam.edgeThres,
												upEdge, 
												downEdge);
			if (findEdge)
			{
				if (false == TransIsPartShort(height, upEdge, downEdge))
				{
					if (REALTIME == mode) 
					{
						m_sortResult.SetResult(SortResult::PartShort, posIndex);
					//	OutputDebugString("Part Edge!");
						ReleaseImage(proSubRot);
						continue;
					}
				}
			}
			
			//Soble Analysis
			if( false == TransSobelAnalysis(proSubRot))
			{
				if (REALTIME == mode) 
				{
					m_sortResult.SetResult(SortResult::SobelErr, posIndex);
				//	OutputDebugString("Sobel Edge!");
					ReleaseImage(proSubRot);
					continue;
				}
			}	

			//blobAnalysis
			if(false == BlobAnalysis(	proSubRot, upEdge, downEdge,
				m_segmentParam.dynWndSize, 
				m_segmentParam.dynThres,
				m_segmentParam.blobSize))
			{
				if (REALTIME == mode) 
				{
					m_sortResult.SetResult(SortResult::BlobErr, posIndex);
					ReleaseImage(proSubRot);
					//		OutputDebugString("BlobErr!");
					continue;
				}
			}
			
			ReleaseImage(proSubRot);
		}
	}
	unsigned int badResult = m_sortResult.GetErrorPosition();
	m_sortObserver.ObserverParam(SortObserver::MResult, badResult & 0x0F);
	m_sortObserver.ObserverParam(SortObserver::MTime,	td.msec()		);

	if( Valve::eSecond == m_processIndex )
	{
		CapsuleProc::AddToAllCount( counter );
	}
	
	if(profileBlobCount > 0)
	{
		RecordingImage();
	}
	return badResult;
}

/***************************************************************
*�������ƣ�	TransRotateIMG
*
* ������	size_t
*			FBLOB
*			IMG	
*			IMG	
*			size_t
*			
* ����ֵ��	
*			
*  
* ���ܣ�	͸�����ҵ���ת����
*			
*
****************************************************************/
bool CapsuleProc::TransRotateIMG(	size_t	blobIndex,
									FBLOB	profileBlob,
									IMG		proImg,
									IMG		&proSubIMG,
									size_t	&posIndex)
{
	long profileCount = 0;
	FBlobGetNumBlobs	(profileBlob, profileCount);
	if ( blobIndex >= static_cast<size_t>(profileCount) )
	{
		posIndex = -1;
		return false;
	}
	
	//Get Range
	long	x0 = 0,		y0 = 0,		width = 0,	height = 0;	
	FBlobGetBoundingBox	(profileBlob, blobIndex, x0, y0, width, height);
	posIndex = CheckPosition(x0+width/2, y0+height/2);
	
	IMG		proIMGMap	= NULL;
	CreateImageMap	(proImg,  x0, y0, x0+width, y0+height, width, height,	proIMGMap);
	
	double	rabi = 0.0, rabi1 = 0.0, rabi2 = 0.0, momentAngle = 0.0;
	FBlobGetMoments	(profileBlob, blobIndex, rabi, rabi1, rabi2, momentAngle);
	
	static const double ADIVPI		= 180.0 /(4.0*atan(1.0));	//ppi	= 180/PI
	static const double ANGLE90		= 90;					//ppiH	= 180/PI*PI/2
	
	double angle = 0.0;
	if(momentAngle>=0)
	{	angle =  (ANGLE90 - ADIVPI * momentAngle);	}
	else 
	{	angle = -(ANGLE90 + ADIVPI * momentAngle);	}
	
	RotateImage(	proIMGMap,	angle,	IP_Linear,	proSubIMG);	
	
	TCoordinateMap cm;
	InitCoordinateMap(cm);
	SetImageCoordinates (proSubIMG, cm); 	
	
	ShrinkVertical	(proSubIMG, m_segmentParam.shrinkY);	
	ShrinkHorizontal(proSubIMG, m_segmentParam.shrinkX*3/2);
	
	ReleaseImage	(proIMGMap);
	return true;
}

/***************************************************************
* �������ƣ�	TransDoubleJudge
*
* ������	IMG			��ͼ��
*			size_t		�뾶
*			
* ����ֵ��	����	true
*			������	false
*			
*  
* ���ܣ�	�ж��Ƿ���������߽�
*			
*
****************************************************************/
bool CapsuleProc::TransDoubleJudge	(   IMG				&subImage,
										const size_t	radius)
{
	const long areaWidth = 20;
	long centreX = ImageWidth(subImage)/2;
	long centreY = ImageHeight(subImage)/2;
	TArea	area;
	area.X0 = centreX -areaWidth;
	area.X2 = centreX -areaWidth;
	area.X1 = centreX +areaWidth;
	area.Y0 = centreY;
	area.Y2 = centreY-radius;
	area.Y1 = centreY;
	TEdgeResult	tr;
	CFindFirstEdge	(	subImage, 0, m_segmentParam.edgeDensity, 
						area, m_segmentParam.edgeThres, FALSE, tr);
	area.X0 = centreX -areaWidth;
	area.X2 = centreX -areaWidth;
	area.X1 = centreX +areaWidth;
	area.Y0 = centreY;
	area.Y2 = centreY+radius;
	area.Y1 = centreY;
	TEdgeResult downEdge;
	CFindFirstEdge	(	subImage, 0, m_segmentParam.edgeDensity, 
						area, m_segmentParam.edgeThres, FALSE, downEdge);

	if ((tr.y == 0.f)||(downEdge.y == 0.f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/***************************************************************
*�������ƣ�	TransIsPartShort
*
* ������	size_t		���ָ߶�
*			size_t		�ϱ߽�
*			size_t		�±߽�
*
* ����ֵ��	����|��ñ	true
*			
*  
* ���ܣ�	�Ƿ���ڶ���|��ñ
*			
*
****************************************************************/
bool CapsuleProc::TransIsPartShort	(	const	size_t height,	
										const	size_t upEdge,
										const	size_t downEdge										)
{
	const size_t part1Length = downEdge + m_segmentParam.shrinkY;
	const size_t part2Length = height - upEdge + m_segmentParam.shrinkY;
	m_curData.PartLength(part1Length, part2Length);
	if (TransIsShort( part1Length, m_capsuleParam.capLength, m_capsuleParam.bodyLength, m_capsuleParam.partTolerance)&&
		TransIsShort( part2Length, m_capsuleParam.bodyLength, m_capsuleParam.capLength, m_capsuleParam.partTolerance))
	{
		return true;
	}
	return false;
}

/***************************************************************
*�������ƣ�	TransIsShort
*
* ������	size_t		��ʵ�߶�
*			size_t		�ϰ벿�ֵĸ߶�
*			size_t		�°벿�ֵĸ߶�
*			size_t		�ݲ�
*
* ����ֵ��	����|��ñ	true
*			
*  
* ���ܣ�	�Ƿ���ڶ���|��ñ
*			
*
****************************************************************/
bool	CapsuleProc::TransIsShort(	const size_t realSize,
									const size_t stdSize1,
									const size_t stdSize2,
									const size_t tolerance)
{
	bool bStd1 = static_cast<size_t>(abs(static_cast<int>(realSize - stdSize1)))<tolerance;
	bool bStd2 = static_cast<size_t>(abs(static_cast<int>(realSize - stdSize2)))<tolerance;

	if (bStd1||bStd2)
	{
		return true;
	}
	return false;
}

/***************************************************************
*�������ƣ�	TransBlobAnalysis
*
*
* ����ֵ��	�Ƿ���ڰߵ�
*			
*  
* ���ܣ�Ϊ͸��������Ƶ�Blob�ߵ���
*			
*
****************************************************************/
bool CapsuleProc::TransBlobAnalysis(	IMG	image,
							  long  upEdgeY, 
							  long  downEdgeY,
							  long	wndSize,
							  long	dynThres,
							  long	blobSize)
{
	long x0 = 0, y0 =0, dx = 0, dy = 0;
	ProfileBlob(image, ImageWidth(image)/2, m_cvbBlob);
	FBlobGetBoundingBox(m_cvbBlob, 0, x0, y0, dx, dy);
	y0 = 0;
	dy = ImageHeight(image) - 1;
	//�м䲿��
	TRect2D<long> roi(x0, upEdgeY, x0+dx, downEdgeY);
	roi.Expand		(-10, -6);
	if(false == RectangleBlob(image, roi, wndSize,  dynThres, blobSize, FBLOB_BORDER_ALL, -1, -1, true))
	{	return false;	}

	if (upEdgeY - y0> (y0 + dy) - downEdgeY)
	{
		roi = TRect2D<long>(x0, y0, x0+dx, upEdgeY-15);
		if(false == RectangleBlob(image, roi, wndSize, dynThres, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}		

		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy);
		if(false == RectangleBlob(image, roi, wndSize, dynThres+5, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}
	}
	else
	{
		roi= TRect2D<long>(x0, y0, x0+dx, upEdgeY-15);
		if(false == RectangleBlob(image, roi, wndSize, dynThres+5, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}		

		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy);
		if(false == RectangleBlob(image, roi, wndSize, dynThres, blobSize, FBLOB_BORDER_ALL))
		{	return false;	}
	}

	if (upEdgeY - y0> (y0 + dy) - downEdgeY)
	{
		roi = TRect2D<long>(x0, y0 + (dx/2), x0+dx, upEdgeY-15);
	}
	else
	{
		roi = TRect2D<long>(x0, downEdgeY+15, x0+dx, y0+dy - (dx/2));
	}
	roi.Expand		(-10, -6);
	if(false == RectangleBlob(image, roi, wndSize,  dynThres, blobSize, FBLOB_BORDER_NONE, -1, -1, true))
	{	return false;	}

	return true;

}