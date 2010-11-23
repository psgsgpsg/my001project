// TImageCard.h	: Interface of the TImageCard class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date		: 2006.11.09
// Last Update Date	: 2007.05.07
//
// ע�ͣ�
// TImageCard ������ͼ��ɼ����Ļ��࣬
// �������������Open��Close��...
//
////////////////////////////////////////////////////////////////////
// Example begin
//*** XXXCard reference to TEmulationCard ***
//
// class XXXCard : public TImageCard
// {    };
//
// XXXCard xcard;
// xcard.Open();
// xcard.Live();
// xcard.GetImagePixels(); 
// then process the image pixel;
// xcard.UnLive();
// xcard.Close();
// 
// Example end
/////////////////////////////////////////////////////////////////////
// A simple tool for frame grab card, 

#ifndef TIMAGECARD_H
#define TIMAGECARD_H

#include "TAllocTmpl.h"
#include "PixelType.h"
#include "TImgDim.h"
#include "TEvent.h"

typedef void REGCallbackFunc(void* param);

class TImageCard
{
public:
	typedef unsigned char PixelMem;
	typedef enum { FREERUN, SOFTTRG, HARDTRG} EnumGrabMode;

public:
	TImageCard();
	virtual ~TImageCard();

public:
	virtual TAlloc<PixelMem> GetPixelMem() = 0; //Get Pixels memery
	bool		IsLiving	() const;
	bool		IsOpen		()	const;
	virtual bool	Open		() = 0;				//open the card
	virtual bool	Close		() = 0;				//close the card
	virtual bool	Open		(	const char * camTag);
	virtual bool	StartLive	();				//start acquistion
	virtual bool	StopLive	();				//stop acquistion
	virtual bool	SetGrabMode	(	EnumGrabMode eMode);
	EnumGrabMode	GetGrabMode	() const;
	virtual bool	SoftTriggering	();		//valid when grab mode is SOFTTRG
	virtual bool	Snap		();		//grab image for a frame
	virtual bool	SetROI		(	const size_t	startX,
						const size_t	startY,
						const size_t	width,
						const size_t	height);
	virtual bool	SetExpTime	(	const size_t	expTime);
	virtual bool	SetGain		(	const size_t	gain);
	virtual bool	SetWhiteBal	(	const size_t	redVal,
						const size_t	greenVal,
						const size_t	blueVal);
	virtual bool	GetExpTime	(	size_t	&expTime);
	virtual bool	GetWhiteBal	(	size_t	&redVal,
						size_t	&greenVal,
						size_t	&blueVal);
	virtual bool	GetGain		(	size_t	&gain);
	virtual bool	GetROI		(	size_t	&startX,
						size_t	&startY,
						size_t	&width,
						size_t	&height);
	virtual	void*	GetPixelMemBase	() const;
	virtual size_t	FrameInterval	();

	void	Wait()	{	m_imageSignal.Wait(-1);		};
	
	int		Width		() const;
	int		Height		() const;
	int		BytesPerPixel	() const;
	int		BytesPerRow	() const;
	int		BitsPerPixel	() const;
	TImgDim		ImageDimension	() const;

	virtual	bool	RegisterProcFunc(REGCallbackFunc* func, void *param);	

protected:
	TImageCard(const TImageCard& src);
	TImageCard& operator=(const TImageCard& src);

protected:
	TAlloc<PixelMem>	m_pixelMem;
	bool			m_opened;
	bool			m_living;
	EnumGrabMode		m_eGrabMode;
	TImgDim			m_roi;

	TEvent				m_imageSignal;
};
/*******************************************************************************
*	�������ƣ�	IsOpen
*	
*	����	��	��			  
*			  
*	���	��	�Ѵ�	true
*				δ��	false
*
*	����	��	����Ƿ��
*
********************************************************************************/
inline bool TImageCard::IsOpen() const
{	
	return m_opened; 
}

/*******************************************************************************
*	�������ƣ�	IsLiving
*	
*	����	��	��			  
*			  
*	���	��	�Ѽ���	true
*				δ����	false
*
*	����	��	����Ƿ��Ѽ���
*
********************************************************************************/
inline bool TImageCard::IsLiving() const
{	return m_living; }

/*******************************************************************************
*	�������ƣ�	Width
*	
*	����	��	��			  
*			  
*	���	��	int		����ɼ�����ͼ��Ŀ��
*
*	����	��	
*
********************************************************************************/
inline int TImageCard::Width() const
{	return m_roi.width;	}

/*******************************************************************************
*	�������ƣ�	Height
*	
*	����	��	��			  
*			  
*	���	��	int		����ɼ�����ͼ��ĸ߶�
*
*	����	��	
*
********************************************************************************/
inline int TImageCard::Height() const
{	return m_roi.height; }

/*******************************************************************************
*	�������ƣ�	BytesPerPixel
*	
*	����	��	��			  
*			  
*	���	��	int		����ɼ�����ͼ��ĸ߶�
*
*	����	��	��ʾÿ�����ص��ֽ���
*
********************************************************************************/
inline int TImageCard::BytesPerPixel() const
{	return m_roi.bytesPerPixel; }

/*******************************************************************************
*	�������ƣ�	BytesPerRow
*	
*	����	��	��			  
*			  
*	���	��	int		��ʾÿ�����ص��ֽ���
*
*	����	��	��ʾÿ�����ص��ֽ���
*
********************************************************************************/
inline int TImageCard::BytesPerRow() const
{	return m_roi.bytesPerRow; }

/*******************************************************************************
*	�������ƣ�	BitsPerPixel
*	
*	����	��	��			  
*			  
*	���	��	int		��ʾÿ�����ص�λ��
*
*	����	��	��ʾÿ�����ص��ֽ���
*
********************************************************************************/
inline int TImageCard::BitsPerPixel() const
{	return m_roi.bitsPerPixel; }

/*******************************************************************************
*	�������ƣ�	ImageDimension
*	
*	����	��	��			  
*			  
*	���	��	TImgDim		ͼ���ʽ����
*
*	����	��	�õ�ͼ���ʽ����
*
********************************************************************************/
inline TImgDim TImageCard::ImageDimension () const
{
	return m_roi;
}

/*******************************************************************************
*	�������ƣ�	GetGrabMode
*	
*	����	��	��			  
*			  
*	���	��	EnumGrabMode	�������ģʽ
*
*	����	��	�õ��������ģʽ
*
********************************************************************************/
inline TImageCard::EnumGrabMode TImageCard::GetGrabMode() const
{	return m_eGrabMode;	}

#endif //TIMAGECARD_H