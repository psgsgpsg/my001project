// TBmpInfo.cpp	: Imp of the TBmpInfo class
//
// Copyright (c) 2008 zhao_fsh PACS Group
//////////////////////////////////////////////////////////////////////////
// Create Date	: 2008.03.26
// Last	  Data  : 
// A simple tool to handle BMP infomation ver 3 (1,4,8,16,24,32bits)
#include "TBmpInfo.h"

TBmpInfo::TBmpInfo(EPelBit ePelBits)
: m_pBitmapInfo(0)
{
	ReInit(ePelBits);
}

TBmpInfo::~TBmpInfo()
{	}

size_t	TBmpInfo::Width	() const
{
	LONG width = m_pBitmapInfo->bmiHeader.biWidth;
	return width < 0 ? -width : width;
}
size_t	TBmpInfo::Height() const
{
	LONG height = m_pBitmapInfo->bmiHeader.biHeight;
	return height < 0 ? -height : height;
}

TBmpInfo::EPelBit TBmpInfo::PixelBits( ) const
{
	return static_cast<EPelBit>(m_pBitmapInfo->bmiHeader.biBitCount);
}

TImgDim TBmpInfo::Dimension() const
{
	TImgDim	dimension;
	dimension.width			= Width();
	dimension.height		= Height();
	dimension.bitsPerPixel	= PixelBits();
	size_t employBytes		= dimension.bitsPerPixel/8;
	dimension.bytesPerPixel = (employBytes == 0) ? 1 : employBytes;
	dimension.bytesPerRow	= 4*((dimension.width * dimension.bitsPerPixel + 31) / 32);
	return dimension;
}

void TBmpInfo::Width(size_t width)
{
	m_pBitmapInfo->bmiHeader.biWidth = (LONG)width;
}
void TBmpInfo::Height(size_t height)
{
	m_pBitmapInfo->bmiHeader.biHeight = (LONG)height;
}

bool TBmpInfo::ReInit(EPelBit ePelBits, size_t width, size_t height)
{
	if(m_bmpInfoBuff.IsNull() || (PixelBits() != ePelBits))
	{
		size_t colorEntries	= ColorCount(ePelBits);
		size_t allocteSize	= sizeof(BITMAPINFOHEADER) + (colorEntries) * sizeof(RGBQUAD);
		m_bmpInfoBuff		= TAlloc<char> (allocteSize);
		m_pBitmapInfo		= (BITMAPINFO*)m_bmpInfoBuff.Base();

		m_bmpInfoBuff.SetZero();
		m_pBitmapInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
		m_pBitmapInfo->bmiHeader.biWidth			= width;
		m_pBitmapInfo->bmiHeader.biHeight			= height;
		m_pBitmapInfo->bmiHeader.biPlanes			= 1;
		m_pBitmapInfo->bmiHeader.biBitCount			= ePelBits;
		m_pBitmapInfo->bmiHeader.biCompression		= BI_RGB;
		m_pBitmapInfo->bmiHeader.biSizeImage		= 0;
		m_pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		m_pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		m_pBitmapInfo->bmiHeader.biClrUsed			= 0;
		m_pBitmapInfo->bmiHeader.biClrImportant		= 0;
		InitColorTable();
	}
	else
	{
		m_pBitmapInfo->bmiHeader.biWidth			= width;
		m_pBitmapInfo->bmiHeader.biHeight			= height;
	}
	return true;
}

bool TBmpInfo::ReInit(const BITMAPINFO *pBmpInfo, size_t size)
{
	if(m_bmpInfoBuff.Size() != size)
	{	m_bmpInfoBuff = TAlloc<char>(size);	}
	m_pBitmapInfo = (BITMAPINFO*)m_bmpInfoBuff.Base();
	memcpy(m_pBitmapInfo, pBmpInfo, size);
	return true;
}

bool TBmpInfo::ReInit(const TImgDim& dim)
{
	EPelBit ePelBit = static_cast<EPelBit>(dim.bytesPerPixel * 8);
	return ReInit(ePelBit, dim.width, dim.height);
}

bool TBmpInfo::SetColorTable(BYTE index, BYTE red, BYTE green, BYTE blue)
{
	EPelBit ePelBits = PixelBits( );
	bool maxBits8 =((ePelBits == e1Bit) || (ePelBits == e4Bit) || (ePelBits == e8Bit));
	bool inRange  =( index < ColorCount(ePelBits) );
	if(maxBits8 && inRange)
	{
		m_pBitmapInfo->bmiColors[index].rgbBlue		= blue;
		m_pBitmapInfo->bmiColors[index].rgbGreen	= green;
		m_pBitmapInfo->bmiColors[index].rgbRed		= red;
		m_pBitmapInfo->bmiColors[index].rgbReserved = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool TBmpInfo::SetColorTable(BYTE index, COLORREF rgb)
{
	return SetColorTable(index, GetBValue(rgb), GetGValue(rgb), GetRValue(rgb));
}

bool TBmpInfo::SetColorMask (DWORD redMask, DWORD greenMask, DWORD blueMask)
{
	EPelBit ePelBits = PixelBits( );
	if( (ePelBits == e16Bit) ||	(ePelBits == e32Bit) )
	{
		m_pBitmapInfo->bmiHeader.biCompression	= BI_BITFIELDS;
		(DWORD&)(m_pBitmapInfo->bmiColors[0])	= redMask;
		(DWORD&)(m_pBitmapInfo->bmiColors[1])	= greenMask;
		(DWORD&)(m_pBitmapInfo->bmiColors[2])	= blueMask;
		return true;
	}
	else
	{
		return false;
	}
}

size_t	TBmpInfo::ColorCount(const EPelBit ePixelBits) const
{
	size_t colorEntries = 0;
	switch(ePixelBits)
	{
	case e1Bit:
	case e4Bit:
	case e8Bit:
		colorEntries = 1 << ePixelBits;
		break;
	case e16Bit:
	case e32Bit:
		colorEntries = 3;
		break;
	default:
		colorEntries = 0;
	}
	return colorEntries;
}

void	TBmpInfo::InitColorTable( )
{
	EPelBit ePelBits	 = PixelBits	( );
	size_t	colorEntries = ColorCount	(ePelBits);
	switch(ePelBits)
	{
	case e1Bit:		// 2   color(gray) entries;
		{
			(DWORD&)(m_pBitmapInfo->bmiColors[0]) = 0x00000000;
			(DWORD&)(m_pBitmapInfo->bmiColors[1]) = 0x00FFFFFF;
		}
		break;

	case e4Bit:		// 16  color entries;
		{
			for (size_t i = 0 ; i < colorEntries ; i++)
			{
				m_pBitmapInfo->bmiColors[i].rgbBlue	 = i*255/colorEntries;
				m_pBitmapInfo->bmiColors[i].rgbGreen = i*255/colorEntries;
				m_pBitmapInfo->bmiColors[i].rgbRed	 = i*255/colorEntries;
				m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
			}
		}
		break;

	case e8Bit:		// 256 color entries;
		{
			for (size_t i = 0 ; i < colorEntries ; i++)
			{
				m_pBitmapInfo->bmiColors[i].rgbBlue	 = i;
				m_pBitmapInfo->bmiColors[i].rgbGreen = i;
				m_pBitmapInfo->bmiColors[i].rgbRed	 = i;
				m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
			}
		}
		break;

	case e16Bit:	//5-5-5 mask
		{
			(DWORD&)(m_pBitmapInfo->bmiColors[0]) = 0x00007C00;
			(DWORD&)(m_pBitmapInfo->bmiColors[1]) = 0x000003E0;
			(DWORD&)(m_pBitmapInfo->bmiColors[2]) = 0x0000001F;
		}
		break;

	case e32Bit:	//8-8-8 mask
		{
			(DWORD&)(m_pBitmapInfo->bmiColors[0]) = 0x00FF0000;
			(DWORD&)(m_pBitmapInfo->bmiColors[1]) = 0x0000FF00;
			(DWORD&)(m_pBitmapInfo->bmiColors[2]) = 0x000000FF;
		}
		break;

	default:
		;
	}	
}

void TBmpInfo::Flip()
{
	m_pBitmapInfo->bmiHeader.biHeight = - (m_pBitmapInfo->bmiHeader.biHeight);
}

//显示或绘图与内存排列一致
void TBmpInfo::AccordMem()
{
	if(0 < m_pBitmapInfo->bmiHeader.biHeight) {	Flip();	}
}

void* TBmpInfo::BmpInfoPoint(size_t* pByteCount) const
{
	if(pByteCount)
	{ 
		*pByteCount = m_bmpInfoBuff.Size();
	}
	return (void*)(m_bmpInfoBuff.Base());
}
