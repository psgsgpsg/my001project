#include <emmintrin.h>
#include "CapsuleProc.h"
#include "HorizonExtend.h"
#include "Translation.h"

#include "TTimediff.h"


CapsuleProc::ESaveState	CapsuleProc::m_eSaveState	= eNoSave;

SortObserver				CapsuleProc::m_sortObserver;		//调试观测工具
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

double CapsuleProc::ObservedParam( )
{
	return m_sortObserver.GetParam();
}

void CapsuleProc::ObservingTarget(unsigned int spec)
{
	m_sortObserver.SpecifyTarget(spec);
}

/***************************************************************
*函数名称：	ReInit
*
* 参数：	TImgDim		图像尺寸信息
*
* 返回值：	成功 true, 失败 false
*  
* 功能：	初始化图像处理函数

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
*函数名称：	SetRawImage
*
* 参数：	pMem		原始图像内存指针
*			bytes		原始图像大小
*
* 返回值：	成功	true
*			失败	false	  
*
* 功能：	拷贝图像内存数据

****************************************************************/
bool CapsuleProc::SetRawImage(void* pMem, size_t bytes)
{
	bool success = false;
	if ((0!=pMem) && (m_rawImage.Size()==bytes))
	{
		memcpy(m_rawImage.Base(), pMem, m_rawImage.Size());
		success = true;
	}
	return success;
}
/***************************************************************
*函数名称：	GetRawImage
*
* 参数：	无
*
* 返回值：	原始图像句柄	  
*
* 功能：	得到原始图像

****************************************************************/
IMG	 CapsuleProc::GetRawImage( )
{
	return m_cvbImg;
}
/***************************************************************
*函数名称：	CheckPosition
*
* 参数：	posX	当前点的X坐标
*			posY	当前点的Y坐标
*
* 返回值：	当前胶囊在图像中的序号	  
*
* 功能：	得到当前胶囊在图像中的序号，posX,posY为胶囊的中心在图像中的坐标

****************************************************************/
int	CapsuleProc::CheckPosition(long posX, long posY)
{
	int interval = m_dim.width / SUBIMGNUM;
	return posX / interval;
}

/***************************************************************
*函数名称：	IsCapsuleShort
*
* 参数：	stdHeight	标准高度
*			realHeight	实际高度
*			tolerance	高度容差
*
* 返回值：	成功	true
*			失败	false
*				  
*
* 功能：	判断胶囊是否短体

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
*函数名称：	FindOverlapCenter
*
* 参数：	rawSubImg		原始子图像
*			centerX			套合区横坐标
*			centerY			套合区纵坐标
*
* 返回值：	成功	true
*			失败	false
*				  
*
* 功能：	得到胶囊套合区中点的坐标

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
*函数名称：	FindFirstEdgeY
*
* 参数：	image			子图像
*			area			边沿检测区域
*			density			边沿检测密度
*			threshold		阈值大小
*
* 返回值：	边沿点的坐标
*				  
*
* 功能：	得到胶囊的边沿的点的坐标

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
*函数名称：	FindUpDownEdge
*
* 参数：	image			子图像
*			density			边沿检测的密度
*			threshold		阈值
*			upPos			上边沿
*			downPos			下边沿
*
* 返回值：	成功	true
*			失败	false
*				  
*
* 功能：	得到胶囊套合区的上下边沿的坐标

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
*函数名称：	CreateTArea
*
* 参数：	orgX			左上角横坐标
*			orgY			左上角纵坐标
*			width			图像宽度
*			height			图像高度
*
* 返回值：	TArea对象
*				  
*
* 功能：	创建图像TArea对象

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
  Description:   对图像水平缩进shrinkNum个像素 
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

	for(int veCo = 0; veCo < imageHeight; ++veCo)
	{
		for(int i = 0; i <= halfWidth; ++i)
		{
			curVal = (VPA[veCo].YEntry + pImageBit + VPA[i].XEntry);
			if ( cstVal == *curVal)	{	*curVal = 0;	}
			else if(0 == *curVal)	{	continue;		}
			else
			{
				for(int j = 0; j < shrinkNum; ++j)
				{				
					*(VPA[veCo].YEntry + pImageBit + VPA[j + i].XEntry) = 0;
				}
				break;
			}
		}

		for(i = imageWidth; i >= halfWidth; --i)
		{
			curVal = (VPA[veCo].YEntry + pImageBit + VPA[i].XEntry);
			if (cstVal == *curVal)	{	*curVal = 0;	}
			else if(0 == *curVal)	{	continue;		}
			else
			{
				for(int j = 0; j < shrinkNum; ++j)
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
*函数名称：	ShrinkVertical
*
* 参数：	image			检测图像
*			shrinkNum		纵向缩减的数值
*返回值：	成功	true 
*			失败	false
*  
* 功能：	对胶囊的图像进行纵向的缩减
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

	for(int i = 0; i < imageWidth; ++i)
	{
		for(int j = 0; j <= halfHeight; ++j)
		{
			curVal = (VPA[i].XEntry + pImageBit + VPA[j].YEntry);
			if (cstVal == *curVal)	{  *curVal = 0;	}
			else if (0 == *curVal)	{	continue;	}
			else
			{
				for(int eroCount = 0; eroCount < shrinkNum; ++eroCount)
				{
					*(VPA[i].XEntry + pImageBit + VPA[eroCount + j].YEntry) = 0;
				}
				break;
			}
		}

		for(j = imageHeight - 1; j >= halfHeight; --j)
		{
			curVal = (VPA[i].XEntry + pImageBit + VPA[j].YEntry);
			if (cstVal == *curVal)	{  *curVal = 0;	}
			else if (0 == *curVal)	{	continue;	}
			else
			{
				for(int eroCount = 0; eroCount < shrinkNum; ++eroCount)
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
*	函数名称：	RotateIMG
*	
*	参数	：[IN]	blobIndex		凹槽序号	
*			  [IN]  fluteBlob		凹槽斑点
*			  [IN]	profileBlob		胶囊轮廓斑点
*			  [IN]	rawImg			含有胶囊斑点信息的图像
*			  [IN]	proImg			含有胶囊轮廓信息的图像
*			  [OUT]	&rawSubIMG		含有胶囊斑点信息的子图像
*			  [OUT]	&proSubIMG		含有胶囊轮廓信息的子图像
*			  [OUT]	&posIndex		凹槽序号
*			  
*
*	功能	：对图像进行分割，取胶囊所在的小的区域，此函数并不对图像进行旋转
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
	if ( blobIndex >= profileCount )
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

	static const double ADIVPI		= 180.0 /(4.0*atan(1));	//ppi	= 180/PI
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
*函数名称：	RectangleBlob
*
* 参数：	IMG						图像
*			TRect2D					感兴趣区域
*			long					窗口面积
*			long					动态阈值
*			long					斑点的最小面积
*			TBlobTouchBorderFilter	忽略边界
*			long					斑点最小宽度
*			long					斑点最小高度
* 返回值：	不存在斑点	true, 
*			存在斑点	false
*  
* 功能：	在指定的区域内采用动态阈值+
*			斑点分析的方法检测是否存在斑点
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
*函数名称：	RectBlackBlob
*
* 参数：	IMG						图像
*			TRect2D					感兴趣区域
*			long					窗口面积
*			long					动态阈值
*			long					斑点的最小面积
*			TBlobTouchBorderFilter	忽略边界
*			long					斑点最小宽度
*			long					斑点最小高度
* 返回值：	不存在斑点	true, 
*			存在斑点	false
*  
* 功能：	在指定的区域内采用动态阈值+
*			斑点分析的方法检测是否存在斑点
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
*函数名称：	BlobAnalysis
*
*  参数：	fblob		斑点分析对象句柄
*			temp		程序模板
*			image		胶囊所在的灰度图像
*			topX		胶囊中轴上顶点X坐标
*			topY		胶囊中轴上顶点Y坐标
*			up			套和区上端点
*			low			套和区下端点
*			bottomY		胶囊中轴下顶点Y坐标
*返回值：	有斑点 true 无斑点false
*  
*功能：		对胶囊所在的灰度图像区域进行斑点分析
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
	//中间部分
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
*函数名称：	RectSobelBlob
*
* 参数：	IMG						图像
*			TRect2D					感兴趣区域
*			long					窗口面积
*			long					动态阈值
*			long					斑点的最小面积
*			TBlobTouchBorderFilter	忽略边界
*			long					斑点最小宽度
*			long					斑点最小高度
* 返回值：	不存在斑点	true, 
*			存在斑点	false
*  
* 功能：	在指定的区域内采用动态阈值+
*			斑点分析的方法检测是否存在斑点
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
*函数名称：	SobelAnalysis
*
* 参数：	IMG						图像
*			
*			
* 返回值：	不存在斑点	true, 
*			存在斑点	false
*  
* 功能：	在指定的区域内采用sobel滤波+
*			斑点分析的方法检测是否存在斑点
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
*函数名称：	TransSobelAnalysis
*
* 参数：	IMG					图像
*			
*			
* 返回值：	不存在斑点	true, 
*			存在斑点	false
*  
* 功能：	在指定的区域内采用sobel滤波+
*			斑点分析的方法检测是否存在斑点
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
	CreateImageMap(copyImage, roi.x0(), roi.y0(), roi.x1(), roi.y1(), roi.Width(), roi.Height(), imageROI);
	if(IsImage(imageROI))
	{
		IMG imageSobel = 0;
		FilterSobelVertical(imageROI, FM_5x5, imageSobel);
		success = RectBlackBlob	(	imageSobel, 
									m_segmentParam.dynWndSize,
									m_segmentParam.dynThres,
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
*	函数名称：	ProfileBlob
*	
*	参数	：	blob		斑点检测对象句柄
*			  	temp		胶囊参数模板
*			  
*  返回值	:	无
*
*	功能	：	对胶囊目标进行斑点检测
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
  Description: 清空图像缓冲区 
***********************************************/
void CapsuleProc::ClearRecordedImage()
{
	m_imgBuffer.SetZero();
}

/**********************************************
  Name:    RecordingImage
  Input:   void
  Output:  void
  Description:   存储原始图像 
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
*函数名称：	SegmentParam
*
* 参数：	SortorParam				处理参数
*			
*			
* 返回值：	无 
*			
*  
* 功能：	为CapsuleProc对象的SortorParam进行赋值操作
*			
*
****************************************************************/

void CapsuleProc::SegmentParam(const SortorParam& param)
{
	m_segmentParam = param;
}

/***************************************************************
*函数名称：	RemainInfo
*
* 参数：	RemainParam				处理参数
*			
*			
* 返回值：	无 
*			
*  
* 功能：	为CapsuleProc对象的RemainParam进行赋值操作
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
*函数名称：	SegmentParam
*
* 参数：	无
*			
*			
* 返回值：	SortorParam		处理参数 
*			
*  
* 功能：	得到Capsule对象的SortorParam成员数据
*			
*
****************************************************************/
const SortorParam& CapsuleProc::SegmentParam( ) const
{
	return m_segmentParam;
}

/***************************************************************
*函数名称：	Dimension
*
* 参数：	无
*			
*			
* 返回值：	TImgDim		图像格式数据
*			
*  
* 功能：	得到TImgDim成员数据
*			
*
****************************************************************/

const TImgDim&	CapsuleProc::Dimension( ) const
{
	return m_dim;
}

/***************************************************************
*函数名称：	CapsuleInfo
*
* 参数：	CapsuleParam	胶囊参数
*			
*			
* 返回值：	无
*			
*  
* 功能：	得到胶囊参数
*			
*
****************************************************************/

void CapsuleProc::CapsuleInfo(const CapsuleParam& param)
{
	m_capsuleParam = param;
}

/***************************************************************
*函数名称：	CapsuleInfo
*
* 参数：	无
*			
*			
* 返回值：	CapsuleParam	胶囊参数
*			
*  
* 功能：	得到胶囊参数
*			
*
****************************************************************/
const CapsuleParam& CapsuleProc::CapsuleInfo( )
{
	return m_capsuleParam;
}

/***************************************************************
*函数名称：	GetSaveState
*
* 参数：	无
*			
*			
* 返回值：	ESaveState	保存方式
*			
*  
* 功能：	得到图像的保存方式
*			
*
****************************************************************/

CapsuleProc::ESaveState CapsuleProc::GetSaveState()
{
	return m_eSaveState;
}

/***************************************************************
*函数名称：	SetSaveState
*
* 参数：	ESaveState	保存方式
*			
*			
* 返回值：	ESaveState	保存方式
*			
*  
* 功能：	设置保存方式
*			
*
****************************************************************/
CapsuleProc::ESaveState CapsuleProc::SetSaveState(ESaveState NextState)
{
	m_eSaveState = NextState;
	return m_eSaveState;
}

/***************************************************************
*函数名称：	TransProcess
*
* 参数：	无
*			
*			
* 返回值：	unsigned int	处理结果
*			
*  
* 功能：	透明胶囊处理
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
	m_transRemain.RemainCapsule(m_rawImage, dim, m_profileImage, m_remainParam, TTransRemain::REMAINMODE::TRANSMODE);
	m_sortObserver.ObserverIMG	(SortObserver::MAll, m_cvbProfileImg);	
	
	long profileBlobCount = 0;
	long minWidth = 1 + m_capsuleParam.capsuleDim.width/8;
	ProfileBlob		(m_cvbProfileImg, minWidth, m_profileBlob);	
	FBlobGetNumBlobs(m_profileBlob, profileBlobCount);	
	
	int counter = 0;
	for(int i = 0; i < profileBlobCount;i++)
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
			
			ReleaseImage(proSubRot);
		}
	}
	unsigned int badResult = m_sortResult.GetErrorPosition();
	m_sortObserver.ObserverParam(SortObserver::MResult, badResult & 0x0F);
	m_sortObserver.ObserverParam(SortObserver::MTime,	td.msec()		);

	if( Valve::eSecond == m_processIndex )
	{
		Valve::AddToAllCount( counter );
	}
	
	if(profileBlobCount > 0)
	{
		RecordingImage();
	}
	return badResult;
}

/***************************************************************
*函数名称：	TransRotateIMG
*
* 参数：	size_t
*			FBLOB
*			IMG	
*			IMG	
*			size_t
*			
* 返回值：	
*			
*  
* 功能：	透明胶囊的旋转处理
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
	if ( blobIndex >= profileCount )
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
	
	static const double ADIVPI		= 180.0 /(4.0*atan(1));	//ppi	= 180/PI
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
* 函数名称：	TransDoubleJudge
*
* 参数：	IMG			子图像
*			size_t		半径
*			
* 返回值：	存在	true
*			不存在	false
*			
*  
* 功能：	判断是否存在两个边界
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
*函数名称：	TransIsPartShort
*
* 参数：	size_t		部分高度
*			size_t		上边界
*			size_t		下边界
*
* 返回值：	短体|短帽	true
*			
*  
* 功能：	是否存在短体|短帽
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
*函数名称：	TransIsShort
*
* 参数：	size_t		真实高度
*			size_t		上半部分的高度
*			size_t		下半部分的高度
*			size_t		容差
*
* 返回值：	短体|短帽	true
*			
*  
* 功能：	是否存在短体|短帽
*			
*
****************************************************************/
bool	CapsuleProc::TransIsShort(	const size_t realSize,
									const size_t stdSize1,
									const size_t stdSize2,
									const size_t tolerance)
{
	if (abs(realSize - stdSize1)<tolerance||abs(realSize - stdSize2)<tolerance)
	{
		return true;
	}
	return false;
}
