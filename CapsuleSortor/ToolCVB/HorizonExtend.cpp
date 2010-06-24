#include "HorizonExtend.h"

bool HorizonExtend::Extend(IMG image)
{
	unsigned char*	pImgBase = NULL; 
	PVPAT			VPAT 	 = NULL; 
	GetImageVPA (image, 0, (LPVOID *)&pImgBase, (PVPAT*)&VPAT); 
	
	long width	= ImageWidth	(image);
	long height = ImageHeight	(image);
	
	for (long y=0; y< height; ++y) 
	{ 
		unsigned char*	pLineAddr = pImgBase + VPAT[y].YEntry; 
		TwoEndInfo endInfo = GetEndInfo(pLineAddr, VPAT, width);
		SetPixelValue(pLineAddr, VPAT, width, endInfo);
	} 
	
	return true;
}

void HorizonExtend::SetPixelValue(	unsigned char*	pLineAddr,
									PVPAT			VPAT,
									long			width,
									const TwoEndInfo& endInfo)
{
	if(endInfo.endPos >= endInfo.beginPos)
	{
		for(long x = 0; x<endInfo.beginPos; ++x)
		{
			*(pLineAddr + VPAT[x].XEntry) = endInfo.beginVal;
		}
		for(x = endInfo.endPos; x<width; ++x)
		{
			*(pLineAddr + VPAT[x].XEntry) = endInfo.endVal;
		}
	}
}

HorizonExtend::TwoEndInfo 
HorizonExtend::GetEndInfo(	unsigned char*	pLineAddr,
							PVPAT			VPAT,
							long			width)
{
	TwoEndInfo endInfo;
	endInfo.beginPos	= width -1;
	endInfo.endPos		= 0;
	
	long x = 0;
	while ((x<width) && (0 == *(pLineAddr + VPAT[x].XEntry)))	{	x++;	}
	if(x<width)
	{
		endInfo.beginPos = x;
		endInfo.beginVal = *(pLineAddr + VPAT[x].XEntry);
	}
	else
	{
		return endInfo;
	}
	
	x = width -1;
	while ((x>=0)  && (0 == *(pLineAddr + VPAT[x].XEntry)))		{	--x;	}
	
	endInfo.endPos = x;
	endInfo.endVal = *(pLineAddr + VPAT[x].XEntry);
	
	return endInfo;
}