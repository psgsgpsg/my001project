// TColorChecker.h: interface for the TColorChecker class.
//
//////////////////////////////////////////////////////////////////////
#ifndef TCOLORCHECKER_H
#define TCOLORCHECKER_H

#include "PixelType.h"

class TColorChecker  
{
public:
	TColorChecker();
	~TColorChecker();

	bool		RightRGB	(	const PelRGB24& color);
	bool		RightHSL	(	const PelHSL24& color);
	bool		RightLab	(	const PelLab24& color);
	PelHSL24	CvtRGBToHSL	(	const PelRGB24&	color);
	PelLab24	CvtRGBToLab	(	const PelRGB24& color);
	bool		SetStandard	(	const PelHSL24& hslValue, 
								const PelHSL24& hslTolerance);
	bool		SetStandard	(	const PelLab24& LabValue, 
								const char		distance);
private:
	template<typename T> T Max(T a, T b) { return a > b ? a : b; }
	template<typename T> T Min(T a, T b) { return a < b ? a : b; }

	bool		RightColor	(	long	realColor,
								long	stdColor,
								long	tolerance,
								long	period = HSLMAX);

private:
	enum {	HSLMAX = 240, RGBMAX = 255,	LOWSAT	= 20 };
	PelHSL24	m_stdHSL;
	PelHSL24	m_tolHSL;
	PelLab24	m_stdLab;
	size_t		m_distLab;	//the square of Lab color space distance; 
};

#endif //TCOLORCHECKER_H

