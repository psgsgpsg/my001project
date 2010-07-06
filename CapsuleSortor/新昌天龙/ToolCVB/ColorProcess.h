#ifndef COLORPROCESS_H
#define COLORPROCESS_H

#include "CapsuleProc.h"
#include "iLightMeter.h"

#include <string>
#include <vector>

#pragma comment(lib, "cvcedge.lib")

#include "TImageTmpl.h"

class ColorProcess : public CapsuleProc
{
public:
	ColorProcess();
	~ColorProcess();

	virtual bool				ReInit	(const TImgDim& imgDimension);
	virtual unsigned int		Process	( WORKMODE mode = REALTIME );
	virtual TCapsuleParam GetProcedureData();
	

private:
	unsigned int FrontLightProcess();
	unsigned int BackLightProcess ();
	unsigned int ComProcess(WORKMODE mode);
	
	bool		BlobAnalysis2(	IMG		image,
								long    middleEdgeY, 
								long	wndSize,
								long	dynThres,
								long	blobSize);

	bool		FindMiddleEdge	(	IMG image, 
									long density,
									double threshold, 
									long &middlePos);
	
	PelRGB24	GetRGBMean		(	IMG image,	LMH lightMeter, TArea area	);

	bool		CapsuleColor	(	size_t		blobIndex,
									FBLOB		profileBlob,
									IMG			rawIMG,
									long		upEdge, 
									long		downEdge, 
									PelRGB24&	capRGB, 
									PelRGB24&	bodyRGB);

	bool		RightColor		(	const PelRGB24& rgb1, 
									const PelRGB24& rgb2);

};

#endif //COLORPROCESS_H
