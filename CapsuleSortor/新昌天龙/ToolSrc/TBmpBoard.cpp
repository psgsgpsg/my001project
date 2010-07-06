
#include "TBmpBoard.h"

TBmpBoard::TBmpBoard()
:m_hdcMem(NULL),
 m_hBmp(NULL),
 m_memBase(0)
{
	m_hdcMem = CreateCompatibleDC(NULL);
}

TBmpBoard::~TBmpBoard()
{
	if(m_hdcMem) { DeleteDC(m_hdcMem);		}
	if(m_hBmp)	 { DeleteObject(m_hBmp);	}
}

bool TBmpBoard::SetBmpInfo(const TBmpInfo& info)
{
	m_dimension = info.Dimension();

	if(m_hBmp)	{	DeleteObject(m_hBmp);	}

	BITMAPINFO* pInfo = (BITMAPINFO *)(info.BmpInfoPoint());
	m_hBmp = CreateDIBSection(
				NULL,			// handle to device context
				pInfo,			// pointer to structure BITMAPINFO, 
				DIB_RGB_COLORS,	// color type: RGB or palette indexes
				&m_memBase,		// pointer to the pointer to bit values
				NULL,			// optional handle to a file mapping object
				0 );			// offset to the bit values within the mapping object
	if(m_hdcMem && m_hBmp)
	{
		memset(m_memBase, 0, m_dimension.bytesPerRow * m_dimension.height);
		SelectObject(m_hdcMem, m_hBmp);
	}
	return m_hdcMem && m_hBmp;
}

HDC TBmpBoard::GetMemDC() const
{
	return m_hdcMem;
}
const TImgDim& TBmpBoard::Dimension() const
{
	return m_dimension;
}

PixelMem* TBmpBoard::GetPixelBase() const
{
	return (PixelMem*)m_memBase;
}