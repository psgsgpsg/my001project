#include <string>
#include <sstream>
#include <emmintrin.h>

#include "ColorProcess.h"
#include "../ToolSrc/TTimeDiff.h"
#include "icvcedge.h"
#include "TSIMDChecker.h"
#include "Translation.h"
#include "HorizonExtend.h"
#include "TImgDim.h"

using namespace std;

ColorProcess::ColorProcess()
{ }

ColorProcess::~ColorProcess()
{ }

/*******************************************************************************
*	函数名称：Init
*	
*	参数	：	[IN]	width			图像宽度
*				[IN]	height			图像高度
*
*	功能	：初始化图像
*
*
*******************************************************************************/
bool ColorProcess::ReInit(const TImgDim& imgDimension)
{
	if(imgDimension == Dimension())	{	return true;	}

	bool success = false;
	if(imgDimension.bytesPerPixel == 3)
	{
		CapsuleProc::ReInit(imgDimension);
		TImgDim dim = Dimension();
		m_remainTool.ReInit(m_cvbImg);
		m_transRemain.ReInit(m_cvbImg);
	//	m_remainTool.CapsuleMaxVal(m_segmentParam.maxValue);
	//	m_remainTool.FluteMinWidth(m_capsuleParam.capsuleDim.width/4);
		success = true;
	}
	return success;
}

/*******************************************************************************
*	函数名称：Process
*	功能	：彩色图像处理主入口函数
*******************************************************************************/
unsigned int ColorProcess::Process(WORKMODE mode)
{
	if (m_segmentParam.transparent)
	{
		return TransProcess(mode);
	}
	else
	{
		return ComProcess(mode);
	}
}
unsigned int ColorProcess::ComProcess(WORKMODE mode)
{
	unsigned int resultVal = 0;		
	resultVal = BackLightProcess();	
	return resultVal;
}

/************************************************
  Name:    BackLightProcess
  Input:   void
  Output:  result
  Description: 对采用背面光的图像进行处理 
**************************************************/
unsigned int ColorProcess::BackLightProcess()
{
	TTimeDiff td; td.Reset();	

	m_curData.Clear();
	m_sortResult.NewPeriod();
	m_sortObserver.NewPeriod(SortObserver::Color);

	TImgDim dim = Dimension();
	m_remainTool.RemainCapsule(m_rawImage, dim,  m_profileImage,m_remainParam);

	m_sortObserver.ObserverIMG	(SortObserver::CAll, m_cvbProfileImg);	

	long profileBlobCount	= 0;
	long minWidth = 1 + m_capsuleParam.capsuleDim.width/8;
	ProfileBlob		(m_cvbProfileImg, minWidth, m_profileBlob);	
	FBlobGetNumBlobs(m_profileBlob, profileBlobCount);
	
	for(int i = 0; i < profileBlobCount; i++)
	{
		IMG		proSubRot	= NULL;
		size_t  posIndex	= 0;
		
		if(RotateIMG(i, m_profileBlob, m_cvbProfileImg, proSubRot, posIndex))
		{
			m_sortObserver.ObserverIMG(SortObserver::CSub,	proSubRot);
	
			const long height	= ImageHeight(proSubRot);
			const long width	= ImageWidth(proSubRot);
			//分析是否短小
			if( IsCapsuleShort(height, width, TToInteger::Floor(0.9f*m_capsuleParam.capsuleDim.height), 0))
			{
				m_sortResult.SetResult(SortResult::ShortErr, posIndex);
				ReleaseImage(proSubRot);
				continue;
			}
			
			//Soble Analysis
			if( false == SobelAnalysis(proSubRot))
			{
				m_sortResult.SetResult(SortResult::SobelErr, posIndex);
				ReleaseImage(proSubRot);
				continue;
			}

			// Search Edge;
			long  upEdge = 0, downEdge = 0;

			if (!FindUpDownEdge(	proSubRot, 
									m_segmentParam.edgeDensity, 
									m_segmentParam.edgeThres,
									upEdge, downEdge))
			{
				m_sortResult.SetResult(SortResult::EdgeErr, posIndex);
				ReleaseImage (proSubRot);
				continue;
			}
 
			//Color Analysis
			PelRGB24 capRGB, bodyRGB;			

			CapsuleColor(i, m_profileBlob, m_cvbImg, upEdge, downEdge, capRGB, bodyRGB);
	
			if(!RightColor(capRGB, bodyRGB))
			{
				m_sortResult.SetResult(SortResult::HueErr, posIndex);
				ReleaseImage (proSubRot);
				continue;
			}

			//Blob Analysis
			if (false == BlobAnalysis(	proSubRot, upEdge, downEdge, 
										m_segmentParam.dynWndSize,
										m_segmentParam.dynThres,
										m_segmentParam.blobSize))
			{
				m_sortResult.SetResult(SortResult::BlobErr, posIndex);
				ReleaseImage(proSubRot);
				continue;
			}
			ReleaseImage	(proSubRot);
		}
	}

	unsigned int badResult	= m_sortResult.GetErrorPosition();

	m_sortObserver.ObserverParam(SortObserver::CResult, badResult & 0x0F);
	m_sortObserver.ObserverParam(SortObserver::CTime,	td.msec());	
	if(profileBlobCount > 0)
	{
		RecordingImage();
	}
	return badResult;
}
/************************************************
  Name:    FrontLightProcess
  Input:   void
  Output:  result
  Description: 对采用正面检测的图像进行处理 
************************************************/

unsigned int ColorProcess::FrontLightProcess()
{
	TTimeDiff td; 
	td.Reset();	

	m_curData.Clear();
	m_sortResult.NewPeriod();
	m_sortObserver.NewPeriod(SortObserver::Color);

	TImgDim dim = Dimension();
//	m_remainTool.CapsuleMaxVal(m_segmentParam.maxValue);
	m_remainTool.RemainCapsule(m_rawImage, dim, m_profileImage,m_remainParam, true);


	m_sortObserver.ObserverIMG	(SortObserver::CAll, m_cvbProfileImg);	

	long profileBlobCount	= 0;
	long minWidth = 1 + m_capsuleParam.capsuleDim.width/8;
	ProfileBlob		(m_cvbProfileImg, minWidth, m_profileBlob);	
	FBlobGetNumBlobs(m_profileBlob, profileBlobCount);
	
	for(int i = 0; i < profileBlobCount; i++)
	{
		IMG		proSubRot	= NULL;
		size_t  posIndex	= 0;
		
		if(RotateIMG(i, m_profileBlob, m_cvbProfileImg, proSubRot, posIndex))
		{
			m_sortObserver.ObserverIMG(SortObserver::CSub,	proSubRot);
	
			const long height	= ImageHeight(proSubRot);
			const long width	= ImageWidth(proSubRot);
			//分析是否短小
/*
			if( IsCapsuleShort(height, width, 0.9f*m_capsuleParam.capsuleDim.height, 0))
			{
				m_sortResult.SetResult(SortResult::ShortErr, posIndex);
				ReleaseImage(proSubRot);
				continue;
			}*/
			

			//Soble Analysis
/*
						if( false == SobelAnalysis(proSubRot))
						{
							m_sortResult.SetResult(SortResult::SobelErr, posIndex);
							ReleaseImage(proSubRot);
							continue;
						}*/
			
	
			// Search Edge;
			long middleEdge = 0;
			if (!FindMiddleEdge(	proSubRot, 
									m_segmentParam.edgeDensity, 
									m_segmentParam.edgeThres,
									middleEdge))
			{
				m_sortResult.SetResult(SortResult::EdgeErr, posIndex);
				ReleaseImage (proSubRot);
				continue;
			}
 
			//Color Analysis
			PelRGB24 capRGB, bodyRGB;			

			CapsuleColor(i, m_profileBlob, m_cvbImg, middleEdge, middleEdge, capRGB, bodyRGB);
	
			if(!RightColor(capRGB, bodyRGB))
			{
				m_sortResult.SetResult(SortResult::HueErr, posIndex);
				ReleaseImage (proSubRot);
				continue;
			}

			//Blob Analysis
			if (false == BlobAnalysis2(	proSubRot, middleEdge, 
										m_segmentParam.dynWndSize,
										m_segmentParam.dynThres,
										m_segmentParam.blobSize))
			{
				m_sortResult.SetResult(SortResult::BlobErr, posIndex);
				ReleaseImage(proSubRot);
				continue;
			}
			ReleaseImage	(proSubRot);
		}
	}

	unsigned int badResult	= m_sortResult.GetErrorPosition();

	m_sortObserver.ObserverParam(SortObserver::CResult, badResult & 0x0F);
	m_sortObserver.ObserverParam(SortObserver::CTime,	td.msec());	
	if(profileBlobCount > 0)
	{
		RecordingImage();
	}
	return badResult;
}


/*******************************************************************************
*	函数名称：FindMiddleEdge
*	
*	参数	：	 	
*
*	功能	：
*
*******************************************************************************/

bool ColorProcess::FindMiddleEdge	(	IMG		image, 
										long	density,
										double	threshold, 
										long&	middlePos)
{
	long	imgWidth	= ImageWidth(image);
	long	imgHeight	= ImageHeight(image);

	long	centerX		= imgWidth/2;
	long	centerY		= imgHeight/2;
	
	long	roiWidth	= imgWidth/2;
	long	roiHeight	= imgHeight/3;
	
	TArea	area_upDown = CreateTArea(centerX -roiWidth/2, centerY - roiHeight/2, roiWidth,  -roiHeight);
	long	yUpToDown	= FindFirstEdgeY(image, area_upDown, density,threshold);
	
	TArea	area_downUp	= CreateTArea(centerX +roiWidth/2, centerY + roiHeight/2, -roiWidth, +roiHeight);
	long	yDownToUp	= FindFirstEdgeY(image, area_downUp, density, threshold);
	
	const long InvalidData = -1;
	if((InvalidData != yUpToDown) && (InvalidData != yDownToUp))
	{
		middlePos = (yUpToDown + yDownToUp)/2;
	}
	else if(InvalidData != yUpToDown)
	{
		middlePos = yUpToDown;
	}
	else if(InvalidData != yDownToUp)
	{
		middlePos = yDownToUp;
	}
	else
	{
		middlePos = 0;
	}
	return (InvalidData != yUpToDown) || (InvalidData != yDownToUp);
}

/*******************************************************************************
*	函数名称：GetRGBMean
*	
*	参数	：[IN]	image		 原始彩色图像
*			  [IN]	lightMeter	 LIGHTERMETER工具
*			  [IN]	area		 	
*
*	功能	：
*
*
*******************************************************************************/
PelRGB24 ColorProcess::GetRGBMean(IMG image, LMH lightMeter, TArea area)
{
	const long imgDim = 3;
	PelRGB24 pixelMean;
	pixelMean.B = pixelMean.G = pixelMean.R = 0;

	if(IsImage(image) && (imgDim == ImageDimension(image)) )
	{
		LMSetImage(lightMeter, image);
		for(long dimIndex = 0; dimIndex < imgDim; ++dimIndex)
		{
			LMSetProcessFlag	(lightMeter, dimIndex, TRUE);
			LMSetArea			(lightMeter, dimIndex, area);
		}
		LMExecute			(lightMeter);
		
		double mean[imgDim] = {0, 0, 0};
		for(long dimIndex = 0; dimIndex < imgDim; ++dimIndex)
		{
			LMGetStatisticMean	(lightMeter, dimIndex, &mean[dimIndex]);
		}
		pixelMean.V[0] = static_cast<UCHAR>(TToInteger::Floor(mean[0]));
		pixelMean.V[1] = static_cast<UCHAR>(TToInteger::Floor(mean[1]));
		pixelMean.V[2] = static_cast<UCHAR>(TToInteger::Floor(mean[2]));
	}
	return pixelMean;
}

/******************************************************
*  Name:    CapsuleColor
*  Input:   blobIndex,  profileBlob, rawIMG, upEdge, downEdge
*  Output:  capRGB,     bodyRGB
*  Description: 返回胶囊指定区域的颜色信息 
*******************************************************/
bool ColorProcess::CapsuleColor	(	size_t		blobIndex,
									FBLOB		profileBlob,
									IMG			rawIMG,
									long		upEdge, 
									long		downEdge, 
									PelRGB24&	capRGB, 
									PelRGB24&	bodyRGB)
{

	long profileCount = 0;
	FBlobGetNumBlobs	(profileBlob, profileCount);
	if ( blobIndex >= static_cast<size_t>(profileCount) )	{	return false;	}

	//Get Range
	long	x0 = 0,		y0 = 0,		width = 0,	height = 0;	
	FBlobGetBoundingBox	(profileBlob, blobIndex, x0, y0, width, height);

	IMG		subIMG	= NULL;
	CreateImageMap	(rawIMG,  x0, y0, x0+width, y0+height, width, height, subIMG);

	TCoordinateMap cm;
	InitCoordinateMap(cm);
	SetImageCoordinates (subIMG, cm); 	

	long centerX	= width/2;
	long centerY	= height/2;
	long delt		= 2;
	long upDist		= abs(upEdge - centerY);
	long downDist	= abs(downEdge - centerY);

	TRect2D<long> capROI(TPoint2D<long>(0,0), 10, 10);
	TRect2D<long> bodyROI = capROI;
	if(upDist - downDist > delt)
	{
		capROI.TranslateCenterTo	(TPoint2D<long>(centerX, upEdge		- height/10));
		bodyROI.TranslateCenterTo	(TPoint2D<long>(centerX, downEdge	+ height/4));
	}
	else if(downDist - upDist > delt)
	{
		capROI.TranslateCenterTo	(TPoint2D<long>(centerX, upEdge		- height/4));
		bodyROI.TranslateCenterTo	(TPoint2D<long>(centerX, downEdge	+ height/10));
	}
	else
	{
		capROI.TranslateCenterTo	(TPoint2D<long>(centerX, upEdge		- height/10));
		bodyROI.TranslateCenterTo	(TPoint2D<long>(centerX, downEdge	+ height/10));		
	}


	TArea	capArea		= CreateTArea(capROI.x0(), capROI.y0(), capROI.Width(), -capROI.Height());
	TArea	bodyArea	= CreateTArea(bodyROI.x0(),bodyROI.y0(),bodyROI.Width(),-bodyROI.Height());

	capRGB	= GetRGBMean (subIMG, m_lightMeter, capArea);
	bodyRGB	= GetRGBMean (subIMG, m_lightMeter, bodyArea);

	ReleaseImage(subIMG);
	return true;
}

/***************************************************************
*函数名称：	RightColor
****************************************************************/
bool ColorProcess::RightColor(const PelRGB24& capRGB, const PelRGB24& bodyRGB)
{
	TColorChecker capCheck;
	capCheck.SetStandard(m_capsuleParam.capColor, m_capsuleParam.capColor.L);

	TColorChecker bodyCheck;
	bodyCheck.SetStandard(m_capsuleParam.bodyColor, m_capsuleParam.bodyColor.L);

	PelLab24 capLab = capCheck.CvtRGBToLab(capRGB);
	PelLab24 bodyLab= bodyCheck.CvtRGBToLab(bodyRGB);

	bool ok1 = capCheck.RightLab(capLab)  && bodyCheck.RightLab(bodyLab);
	bool ok2 = capCheck.RightLab(bodyLab) && bodyCheck.RightLab(capLab);

	size_t colorCode = Translation::LabCoding(capLab, bodyLab);
//	m_curData.push_back(colorCode);
	m_curData.BodyColor(bodyLab);
	m_curData.CapColor(capLab);
	m_sortObserver.ObserverParam(SortObserver::CHue, colorCode);

	return ok1 || ok2;
}

/***************************************************************
函数名称：	BlobAnalysis2

  参数：	
  image		胶囊所在的灰度图像
  midY		胶囊中轴上分界坐标
  返回值：	有斑点 true 无斑点false
  
 功能：	对胶囊所在的灰度图像区域进行斑点分析
****************************************************************/

bool ColorProcess::BlobAnalysis2(IMG	image,
								long    middleEdgeY, 
							    long	wndSize,
								long	dynThres,
								long	blobSize)
{
	long x0 = 0, y0 =0, dx = 0, dy = 0;
	ProfileBlob(image, ImageWidth(image)/2, m_cvbBlob);
	FBlobGetBoundingBox(m_cvbBlob, 0, x0, y0, dx, dy);
	long offsetMid = 5;
	long offsetEnd = dy/8;
	
	TRect2D<long> upRoi		(x0, y0+offsetEnd,			x0+dx, middleEdgeY-offsetMid);
	TRect2D<long> downRoi	(x0, middleEdgeY+offsetMid, x0+dx, y0+dy-offsetEnd);
	IMG		copyImage = NULL;
	CreateDuplicateImage(image, copyImage);
	HorizonExtend::Extend(copyImage);
	bool	succ = true;
	if(false == RectangleBlob(copyImage, upRoi,		wndSize, dynThres, blobSize))
	{	succ =  false;	}
	if(false == RectangleBlob(copyImage, downRoi,	wndSize, dynThres, blobSize))
	{	succ = false;	}
	ReleaseImage(copyImage);
	return succ;
}

/******************************************************
*  Name:    GetProcedureData
*  Input:   void   
*  Output:  m_curHue;
*  Description: 返回当前胶囊的颜色信息 
*******************************************************/
TCapsuleParam ColorProcess::GetProcedureData()
{
	return m_curData;
}




















