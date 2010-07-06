// TImageTmpl.h	: Interface of the TImage class
//
// Copyright (c) 2008 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :Image
// Create Date	: 2008.03.03
// Last   Date  : 2008.08.27
// A simple tool for Store Image Data

#ifndef TIMAGETMPL_H
#define TIMAGETMPL_H

#include "PixelType.h"
#include "TAllocTmpl.h"
#include "TImgDim.h"
#include "TRect2D.h"
#include "TImgRowIter.h"

template<class T>
class TImage
{
public:
	typedef TImgRowIter<T>  RowIter;
	typedef TRect2D<int>	ImgRect;

	TImage();
	TImage(const TAlloc<PixelMem>& imgBuff, const TImgDim& dim);
	TImage(const ImgRect& boundary, eAlignment lineAlign = e16ByteAlign);
	virtual ~TImage();
	
public:
	bool	ReInit		(const ImgRect& boundary, eAlignment lineAlign = e16ByteAlign);
	ImgRect Window		(const ImgRect& window);
	ImgRect Window		( ) const;
	ImgRect	Maximize	( );

	bool	SetImage	(const void *pImageData, const TImgDim& dim, bool VReverse = false);
	bool	SetImage	(const T& val);
	bool	SetPixel	(int x, int y, const T& val);
	bool	GetPixel	(int x, int y, T& val);
	T		GetPixel	(int x, int y);
	void	SetZero		( );
	T*		Base		( ) const;

	T& operator			() (int x, int y);
	const T& operator	() (int x, int y) const;

	bool	IsNull		() const { return m_roi.IsNull();		}
	size_t	RowSpace	() const { return m_rowBytes;			}
	size_t	Width		() const { return m_roi.Width();		}
	size_t  Height		() const { return m_roi.Height();		}
	TImgDim Dimension	() const;

	RowIter	RowBegin	() const;
	RowIter RowEnd		() const;

	TImage<T> Duplicate	() const;

private:
	T*		RowAddr		(int y) const;

private:
	size_t				m_rowBytes;
	TAlloc<PixelMem>	m_imgBuff;
	TRect2D<int>		m_boundary;
	TRect2D<int>		m_roi;
};

template<class T>
TImage<T>::TImage()
: m_rowBytes(0)
{	}

template<class T>
TImage<T>::TImage(const ImgRect& boundary, eAlignment lineAlign)
: m_rowBytes(0)
{
	ReInit(boundary, lineAlign);
}
template<class T>
TImage<T>::TImage(const TAlloc<PixelMem>& imgBuff, const TImgDim& dim)
{
	if( (dim.bytesPerPixel == sizeof(T)) &&
		(imgBuff.Size() == dim.bytesPerRow * dim.height) )
	{
		TRect2D<int> rect  = TRect2D<int>(TPoint2D<int>(0,0), dim.width, dim.height);
		m_roi		= m_boundary = rect;
		m_rowBytes	= dim.bytesPerRow;
		m_imgBuff	= imgBuff;
	}
	else
	{
		ReInit(TRect2D<int>(), eNoAlign);
	}
}

template<class T>
TImage<T>::~TImage()
{   }


template<class T>
bool TImage<T>::ReInit(const ImgRect& boundary, eAlignment lineAlign)
{
	m_roi = boundary;
	if( (m_boundary != boundary) || 
		(m_rowBytes%lineAlign != 0) )
	{
		m_boundary = boundary;
		m_rowBytes = m_boundary.Width () * sizeof(T);
		if (lineAlign != eNoAlign)
		{
			int alignment = static_cast<int>(lineAlign) - 1;
			m_rowBytes = (m_rowBytes + alignment) & (~alignment);
		}
		int frameBytes	= m_boundary.Height() * m_rowBytes;
		m_imgBuff		= TAlloc<PixelMem> (frameBytes, e16ByteAlign);
	}
 	return true;
}

template<class T>
typename TImage<T>::ImgRect TImage<T>::Window(const ImgRect& window)
{
	m_roi = m_boundary.Intersect(window);
	return m_roi;
}

template<class T>
typename TImage<T>::ImgRect TImage<T>::Window() const
{
	return m_roi;
}

template<class T>
typename TImage<T>::ImgRect TImage<T>::Maximize()
{
	return m_roi = m_boundary;
}

template<class T>
typename TImage<T>::RowIter TImage<T>::RowBegin() const
{
	if(!IsNull())
	{
		T *pBegin  =  RowAddr(m_roi.y0()) + (m_roi.x0() - m_boundary.x0());
		return RowIter(reinterpret_cast<T*>(pBegin), m_rowBytes, m_roi.x0(), m_roi.y0());
	}
	else
	{
		return RowIter();
	}
}

template<class T>
typename TImage<T>::RowIter TImage<T>::RowEnd() const
{
	if(!IsNull())
	{
		PixelMem *pBegin =	reinterpret_cast<PixelMem *>(RowBegin().Base()); 
		PixelMem *pEnd   =  pBegin + m_roi.Height() * m_rowBytes;
		return RowIter(reinterpret_cast<T*>(pEnd), m_rowBytes, m_roi.x0(), m_roi.y1());
	}
	else
	{
		return RowIter();
	}
}

template<class T>
bool TImage<T>::SetImage(const void *pImageData, const TImgDim& dim, bool VReverse)
{
	bool success = false;
	if((Dimension() == dim) && (false == VReverse))
	{
		memcpy(Base(), pImageData, dim.bytesPerRow*dim.height);
		success = true; 
	}
	else if((sizeof(T)==dim.bytesPerPixel))
	{
		size_t h = dim.height< Height()? dim.height: Height();	
		size_t w = dim.width < Width() ? dim.width : Width();
		size_t bytes = w * dim.bytesPerPixel;
		PixelMem* pBase	= (PixelMem*)(Base());
		PixelMem* pData = (PixelMem*)(pImageData);
		int  srcRowSpace= dim.bytesPerRow; 
		if(false != VReverse)
		{
			pData += (dim.height-1)*dim.bytesPerRow;
			srcRowSpace = -srcRowSpace;
		}
		for(size_t row = 0; row < h; ++row)
		{
			memcpy(pBase, pData, bytes);
			pBase	+= m_rowBytes;
			pData	+= srcRowSpace;
		}
		success = true;
	}
	return success;
}

template<class T>
bool TImage<T>::SetImage(const T& val)
{
	RowIter iter	= RowBegin();
	RowIter iterEnd = RowEnd();
	size_t  w		= Width();
	for( ; iter != iterEnd; ++iter)
	{
		T* pPixel = iter.Base();
		for(size_t x=0; x<w; ++x, ++pPixel)
		{
			*pPixel = val;
		}
	}
	return true;
}

template<class T>
T* TImage<T>::Base() const
{
	return IsNull() ? 0 : RowAddr(m_roi.y0()) + (m_roi.x0() - m_boundary.x0());
}

template<class T>
void TImage<T>::SetZero( )
{
	m_imgBuff.SetZero();
}

template<class T>
bool TImage<T>::SetPixel(int x, int y, const T& val)
{
	bool success = false;
	if(m_boundary.Within(x, y))
	{
		*(RowAddr(y) + x) = val;
		success = true;
	}
	return true;
}

template<class T>
bool TImage<T>::GetPixel(int x, int y, T& val)
{
	bool success = false;
	if(m_boundary.Within(x,y))
	{
		val = *(RowAddr(y) + x);
		success = true;
	}
	return success;
}

template<class T>
T TImage<T>::GetPixel(int x, int y)
{
	T val = false;
	if(m_boundary.Within(x,y))
	{
		val = *(RowAddr(y) + x);
	}
	return val;
}


template<class T>
T& TImage<T>::operator	() (int x, int y)
{
	return *(RowAddr(y) + x);
}

template<class T>
const T& TImage<T>::operator() (int x, int y) const
{
	return *(RowAddr(y) + x);
}

template<class T>
T* TImage<T>::RowAddr(int y) const
{
	const PixelMem* addr = m_imgBuff.Base() + (y-m_boundary.y0())*m_rowBytes ;
	return (T*)(addr);
}

template<class T>
TImgDim TImage<T>::Dimension() const
{
	TImgDim dim;
	dim.bytesPerPixel	= sizeof(T);
	dim.bitsPerPixel	= dim.bytesPerPixel*8;
	dim.bytesPerRow		= RowSpace();
	dim.width			= Width();
	dim.height			= Height();
	return dim;
}

template<class T>
TImage<T> TImage<T>::Duplicate() const
{
	TImage<T> dup;
	if(!IsNull())
	{
		dup.m_rowBytes = m_rowBytes;
		dup.m_imgBuff  = m_imgBuff.Duplicate();
		dup.m_boundary = m_boundary;
		dup.m_roi	   = m_roi;
	}
	return dup;
}

#endif //TIMAGETMPL_H