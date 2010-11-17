#ifndef TBMPDRAWER_H
#define TBMPDRAWER_H

#include "TBmpInfo.h"

class TBmpBoard
{
public:
	TBmpBoard();
	~TBmpBoard();
	bool			SetBmpInfo(const TBmpInfo& info);
	HDC				GetMemDC()	const;
	const TImgDim&	Dimension() const;
	PixelMem*		GetPixelBase() const;

private:
	HDC			m_hdcMem;
	HBITMAP		m_hBmp;
	TImgDim		m_dimension;
	void*		m_memBase;
};



#endif // TBMPDRAWER_H