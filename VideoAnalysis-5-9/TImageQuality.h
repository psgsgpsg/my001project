// TImageQuality.h: interface for the TImageQuality class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMAGEQUALITY_H__FAE0DCCF_2BA7_48C9_8219_09CF48C004C6__INCLUDED_)
#define AFX_TIMAGEQUALITY_H__FAE0DCCF_2BA7_48C9_8219_09CF48C004C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

class TImageQuality  
{
public:
	TImageQuality();
	virtual ~TImageQuality();
public:
	static int GetImageMeanValue(IplImage *pImage);

};

#endif // !defined(AFX_TIMAGEQUALITY_H__FAE0DCCF_2BA7_48C9_8219_09CF48C004C6__INCLUDED_)
