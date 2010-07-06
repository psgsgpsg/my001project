// TImgBuffer.cpp	: the TImgBuffer class
//
// Copyright (c) 2008 zhao_fsh
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageBuffer
// Create Date	: 2008.03.03
// Last   Date  : 2008.08.26
// A simple tool for Store Image Data
 
#include "TImgBuffer.h"


TImgBuffer::TImgBuffer()
: m_frameIndex(eInvalidIndex),
  m_frameBytes(0),
  m_frameCount(0)
{	}

TImgBuffer::TImgBuffer(const TAlloc<PixelMem>&	imgBuffer,
					   const TImgDim&			dimension,
					   size_t					frameCount)
{
	if( imgBuffer.Size() == dimension.bytesPerRow * dimension.height * frameCount )
	{
		m_frameIndex = eInvalidIndex;
		m_frameCount = frameCount;
		m_imgDim	 = dimension;
		m_frameBytes = dimension.height * dimension.bytesPerRow;
		m_imgBuff	 = imgBuffer;
	}
	else
	{
		ReInit(dimension, 0);
	}
}

TImgBuffer::~TImgBuffer()
{   }


bool TImgBuffer::ReInit(const TImgDim& dimension, size_t frameCount)
{
	m_frameIndex = eInvalidIndex;
	m_frameCount = frameCount;
	m_imgDim	 = dimension;
	m_frameBytes = dimension.height * dimension.bytesPerRow;
	m_imgBuff	 = TAlloc<PixelMem> (m_frameBytes * frameCount, e16ByteAlign);
 	return true;
}

void TImgBuffer::SetZero()
{
	m_imgBuff.SetZero();
	m_frameIndex = eInvalidIndex;	
}

bool TImgBuffer::PushData(void *pImageData, const TImgDim& dim)
{
	bool success = false;
	if((m_imgDim.bytesPerPixel==dim.bytesPerPixel) && GoNextFrame( ))
	{
		size_t h = dim.height< m_imgDim.height ? dim.height: m_imgDim.height;	
		size_t w = dim.width < m_imgDim.width  ? dim.width : m_imgDim.width;

		PixelMem* pBase	= CurrentFrameData();
		PixelMem* pData = (PixelMem*)(pImageData);
	
		size_t bytes = w * dim.bytesPerPixel;

		for(size_t row = 0; row < h; ++row)
		{
			memcpy(pBase, pData, bytes);
			pBase	+= m_imgDim.bytesPerRow;
			pData	+= dim.bytesPerRow;
		}
		success = true;
	}
	return success;
}

PixelMem* TImgBuffer::FrameData(size_t frameIndex) const
{
	if((m_frameCount > 0) && (frameIndex < m_frameCount))
	{
		const PixelMem*	pBase	= m_imgBuff.Base();
		size_t			offset	= frameIndex * m_frameBytes;
		return (PixelMem*)(pBase + offset);
	}
	else
	{
		return 0;
	}
}


PixelMem *TImgBuffer::CurrentFrameData() const
{
	return (eInvalidIndex != m_frameIndex) ? FrameData(m_frameIndex) : NULL;
}


bool TImgBuffer::GoNextFrame( )
{
	bool success = false;
	if(!IsNull())
	{
		m_frameIndex = (m_frameIndex + 1) % m_frameCount;
		success = true;
	}
	return success;
}