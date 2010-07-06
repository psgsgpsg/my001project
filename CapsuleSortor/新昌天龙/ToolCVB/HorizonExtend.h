#ifndef HORIZONEXTEND_H
#define HORIZONEXTEND_H

#include "windows.h"
#include "iCVCImg.h"
#include "iCVFoundation.h"

class HorizonExtend
{
public:
	~HorizonExtend();
	static bool Extend(IMG image);	

private:
	HorizonExtend();

private:
	struct TwoEndInfo
	{
		unsigned char	beginVal;
		long			beginPos;
		unsigned char	endVal;
		long			endPos;
	};

	static void 
	SetPixelValue(	unsigned char*	pLineAddr,
					PVPAT			VPAT,
					long			width,
					const TwoEndInfo& endInfo);

	static TwoEndInfo 
	GetEndInfo	(	unsigned char*	pLineAddr,
					PVPAT			VPAT,
					long			width);	
};

#endif //HORIZONEXTEND_H
