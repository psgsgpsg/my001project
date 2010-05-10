// TImageQuality.cpp: implementation of the TImageQuality class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoAnalysis.h"
#include "TImageQuality.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TImageQuality::TImageQuality()
{

}

TImageQuality::~TImageQuality()
{

}

int TImageQuality::GetImageMeanValue(IplImage *pImage)
{
	if (pImage == NULL)
	{
		return 0;
	}
	CvScalar meanValue = cvAvg(pImage);
	return (int)meanValue.val[0];
}