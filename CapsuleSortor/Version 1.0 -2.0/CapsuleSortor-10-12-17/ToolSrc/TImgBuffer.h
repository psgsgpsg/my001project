// TImageBuffer.h	: Interface of the TImageBuffer class
//
// Copyright (c) 2008 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImgBuff
// Create Date	: 2008.03.03
// Last   Date  : 2008.03.27
// A simple tool for Store Image Data

#ifndef TIMGBUFFER_H
#define TIMGBUFFER_H

#include "PixelType.h"
#include "TAllocTmpl.h"
#include "TImgDim.h"

class TImgBuffer
{
public:
	TImgBuffer();
	TImgBuffer(const TAlloc<PixelMem>& imgBuff, const TImgDim& dim, size_t frameCount);
	virtual ~TImgBuffer();
	
public:
	bool ReInit		(const TImgDim& dimension, size_t frameCount = 1);
	bool PushData	(void *pImageData, const TImgDim& dim);
	void SetZero	();	

	bool		GoNextFrame		();
	PixelMem*	CurrentFrameData() const;
	PixelMem*	FrameData		(size_t frameIndex = 0) const;

	bool	IsNull				() const { return (m_frameCount<=0);	}
	TImgDim Dimension			() const { return m_imgDim;				}
	int		CurrentFrameIndex	() const { return m_frameIndex;			}
	size_t	FrameCount			() const { return m_frameCount;			}
	size_t	FrameBytes			() const { return m_frameBytes;			}

private:
	enum				{ eInvalidIndex = -1 };
	int					m_frameIndex;
	size_t				m_frameBytes;
	size_t				m_frameCount;
	TAlloc<PixelMem>	m_imgBuff;
	TImgDim				m_imgDim;
};

#endif //TIMGBUFFER_H