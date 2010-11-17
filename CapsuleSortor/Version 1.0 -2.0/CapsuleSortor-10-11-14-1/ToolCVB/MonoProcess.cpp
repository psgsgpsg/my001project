
#include "MonoProcess.h"
#include "iCVCEdge.h"
#include "../ToolSrc/TTimeDiff.h"
#include "TSIMDChecker.h"
#include "Translation.h"
#include <sstream>
using namespace std;

MonoProcess::MonoProcess()
{ }
MonoProcess::~MonoProcess()
{

}
/*******************************************************************************
*	函数名称：	Init
*	
*	参数	：	width		图像的宽度
*			  	height		图像的高度
*			  
*  返回值	:	成功	true
*				失败	false
*
*	功能	：	初始化单色图像处理类
*
*
********************************************************************************/
bool MonoProcess::ReInit(const TImgDim& imgDimension)
{
	if(imgDimension == Dimension())	{	return true;	}
	
	bool success = false;
	if(imgDimension.bytesPerPixel == 1)
	{
		CapsuleProc::ReInit(imgDimension);
		TImgDim dim = Dimension();
//		m_remainTool.ReInit(m_cvbImg);
		m_transRemain.ReInit(m_cvbImg);
		//m_remainTool.CapsuleMaxVal(m_segmentParam.maxValue);
		//m_remainTool.FluteMinWidth(m_capsuleParam.capsuleDim.width/4);
		success = true;
	}
	return success;
}



/*******************************************************************************
*	函数名称：	Process
*	
*	参数	：无
*			  
*
*	功能	：单色图像的主入口函数
*
*
********************************************************************************/
unsigned int MonoProcess::Process( WORKMODE mode )
{
	if (m_segmentParam.transparent)
	{
		return TransProcess(mode );
	}
	else
	{
		return ComProcess(mode );
	}
}

/*******************************************************************************
*	函数名称：	ComProcess
*	
*	参数	：无
*			  
*
*	功能	：通常处理
*
*
********************************************************************************/
unsigned int MonoProcess::ComProcess( WORKMODE mode)
{
	TTimeDiff td;
	td.Reset();

	m_curData.Clear();
	m_sortResult.NewPeriod();
	m_sortObserver.NewPeriod(SortObserver::Mono);

	TImgDim dim = Dimension();
	if (!m_transRemain.RemainCapsule(	m_rawImage, dim, m_profileImage, m_remainParam, TTransRemain::OPAQUEMODE)) 
	{
		return 0;
	}
	
	m_sortObserver.ObserverIMG(SortObserver::MAll, m_cvbProfileImg);

	long profileBlobCount = 0;
	long minWidth = 1 + m_capsuleParam.capsuleDim.width/8;

	ProfileBlob		(	m_cvbProfileImg,	minWidth, m_profileBlob);	
	FBlobGetNumBlobs(	m_profileBlob,		profileBlobCount);	
	
	int counter = 0;
	for(int i = 0; i < profileBlobCount;i++)
	{
		IMG		proSubRot	= NULL;
		size_t  posIndex	= 0;
		if(RotateIMG(i, m_profileBlob, m_cvbProfileImg, proSubRot, posIndex))
		{
			if (posIndex == 0 || posIndex == 2)
			{
				counter++;
			}
			m_sortObserver.ObserverIMG(SortObserver::MSub, proSubRot);

			const long width = ImageWidth (proSubRot);
			const long height= ImageHeight(proSubRot);
			//m_curData.push_back( Translation::DimCoding(width, height) );
			m_curData.WidthAndHeight(width, height);

			//all short
			if( IsCapsuleShort(	height, width,
								m_capsuleParam.capsuleDim.height, 
								m_capsuleParam.capsuleDim.tolerance))
			{
				if (REALTIME == mode) 
				{					
					m_sortResult.SetResult(SortResult::ShortErr, posIndex);
					ReleaseImage(proSubRot);
		//			OutputDebugString("Short!");
					continue;
				}
			}

			//Soble Analysis
			if( false == SobelAnalysis(proSubRot))
			{
				if (REALTIME == mode) 
				{
					m_sortResult.SetResult(SortResult::SobelErr, posIndex);
					ReleaseImage(proSubRot);
		//			OutputDebugString("Sobel Analysis!");
					continue;
				}
			}
			
			// Search Edge;
			long  upEdge = 0, downEdge = 0;
			if (FindUpDownEdge(	proSubRot,
								m_segmentParam.edgeDensity,
								m_segmentParam.edgeThres,
								upEdge,
								downEdge))
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
			
			//分析表面斑点，划痕等。
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

	if (Valve::eFirst == GetProcessIndex())
	{
		CapsuleProc::AddToAllCount(counter);
	}
	
	if(profileBlobCount > 0)
	{
		RecordingImage();
	}
	return badResult;
}

/***********************************************************
*  Name:    GetProcedureData 
*  Input:   void
*  Output:  m_curDim
*  Description: 返回胶囊的当前胶囊的尺寸信息 
************************************************************/
TCapsuleParam MonoProcess::GetProcedureData()
{
	return m_curData;
}

/***********************************************************
*  Name:    IsPartShort 
*  Input:   height      upEdge    downEdge
*  Output:  result
*  Description: 判断胶囊的体或者帽的尺寸是否异常 
************************************************************/

bool	MonoProcess::IsPartShort(	const size_t height, 
									size_t upEdge, 
									size_t downEdge)const
{
	if (downEdge < upEdge)
	{
		size_t exchange = downEdge;
		downEdge	= upEdge;
		upEdge		= exchange;
	}
	
	static float	lengthMean = static_cast<float>(height + downEdge - upEdge);
	static float	changeMean = 15.0f;
	float			newLength  = static_cast<float>(height + downEdge - upEdge);
	float			newChange  = fabs(newLength - lengthMean);
	lengthMean		= 0.9f * lengthMean + 0.1f * newLength;
	changeMean		= 0.9f * changeMean + 0.1f * newChange;
	return newLength < lengthMean - 2.0f * changeMean;
				
}