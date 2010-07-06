// ImageCard.cpp	: Implement of the ImageCard class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2006.11.9
//
// A simple tool for frame grab card

#include "TImageCard.h"
#include <memory.h>		// for memset()

TImageCard::TImageCard()
 :	m_opened(false),
	m_living(false),
	m_eGrabMode(FREERUN),
	m_imageSignal(false, false)
{
	memset(&m_roi, 0, sizeof(m_roi));
}

TImageCard::~TImageCard()
{	}
/*************************************************
*	函数名称：StartLive
*	
*	参数	：	无
*
*	功能	：相机开始采集图像
*
*
****************************************************/
bool TImageCard::StartLive()
{
	if(m_opened && !m_living)
	{
		m_living = true;
	}
	return m_living;
}
/*************************************************
*	函数名称：StopLive
*	
*	参数	：	无
*
*	功能	：相机暂停
*
*
****************************************************/
bool TImageCard::StopLive()
{
	if(m_living)
	{
		m_living = false;
	}
	return m_living;
}
/*************************************************
*	函数名称：snap
*	
*	参数	：	无
*
*	功能	：采集
*
*
****************************************************/
bool TImageCard::Snap()
{
	return false;
}

/*************************************************
*	函数名称：SetGrabMode
*	
*	参数	：	eMode	设置触发模式
*
*	功能	：采集
*
*
****************************************************/
bool TImageCard::SetGrabMode(EnumGrabMode eMode)
{
	m_eGrabMode = eMode;
	return true;
}
/*************************************************
*	函数名称：SoftTriggering
*	
*	参数	：无
*
*	功能	：软触发
*
*
****************************************************/

bool TImageCard::SoftTriggering()
{
	return false;
}

/*************************************************
*	函数名称：GetPixelMem
*	
*	参数	：无
*
*	功能	：得到图像内存区句柄
*
*
****************************************************/
TAlloc<TImageCard::PixelMem> TImageCard::GetPixelMem( )
{
	return m_pixelMem;
}

bool	TImageCard::RegisterProcFunc(REGCallbackFunc* func, void *param)
{
//	OutputDebugString("Reg!");
	return true;
}

bool	TImageCard::SetROI(	size_t startX,
					   size_t startY,
					   size_t width,
					   size_t height)
{
	return true;
}

bool	TImageCard::SetExpTime	(	size_t expTime)
{
	return true;
}

bool	TImageCard::SetGain	(	size_t gain)
{
	return true;
}

bool	TImageCard::SetWhiteBal	(	size_t redVal,
					size_t greenVal,
					size_t blueVal)
{
	return true;
}

bool	TImageCard::GetExpTime (	size_t &expTime)
{
	return true;
}

bool	TImageCard::GetWhiteBal(	size_t &redVal,
					size_t &greenVal,
					size_t &blueVal)
{
	return true;
}

bool	TImageCard::GetGain	(	size_t &gain)
{
	return true;
}

bool	TImageCard::GetROI	(	size_t &startX,
					 size_t &startY,
					 size_t &width,
					size_t &height)
{
	return true;
}
/*************************************************
*	函数名称：GetPixelMemBase
*	
*	参数	：无
*
*	功能	：得到图像内存区句柄
*
*
****************************************************/
void*	TImageCard::GetPixelMemBase() const
{
	return NULL;
}

bool	TImageCard::Open( const char * camTag)
{
	return true;
}
size_t	TImageCard::FrameInterval()
{
	return 0;
}


	
