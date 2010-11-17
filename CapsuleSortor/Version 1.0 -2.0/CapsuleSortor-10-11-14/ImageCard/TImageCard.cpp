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
*	�������ƣ�StartLive
*	
*	����	��	��
*
*	����	�������ʼ�ɼ�ͼ��
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
*	�������ƣ�StopLive
*	
*	����	��	��
*
*	����	�������ͣ
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
*	�������ƣ�snap
*	
*	����	��	��
*
*	����	���ɼ�
*
*
****************************************************/
bool TImageCard::Snap()
{
	return false;
}

/*************************************************
*	�������ƣ�SetGrabMode
*	
*	����	��	eMode	���ô���ģʽ
*
*	����	���ɼ�
*
*
****************************************************/
bool TImageCard::SetGrabMode(EnumGrabMode eMode)
{
	m_eGrabMode = eMode;
	return true;
}
/*************************************************
*	�������ƣ�SoftTriggering
*	
*	����	����
*
*	����	������
*
*
****************************************************/

bool TImageCard::SoftTriggering()
{
	return false;
}

/*************************************************
*	�������ƣ�GetPixelMem
*	
*	����	����
*
*	����	���õ�ͼ���ڴ������
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
*	�������ƣ�GetPixelMemBase
*	
*	����	����
*
*	����	���õ�ͼ���ڴ������
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


	
