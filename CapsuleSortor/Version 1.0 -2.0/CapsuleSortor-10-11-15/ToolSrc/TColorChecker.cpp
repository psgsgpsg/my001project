// TColorChecker.cpp: implementation of the TColorChecker class.

#include "TColorChecker.h"
#include "ToInteger.h"
#include <memory.h>
#include <cmath>

TColorChecker::TColorChecker()
{
	memset(&m_stdHSL, 0, sizeof(m_stdHSL));
	memset(&m_tolHSL, 0, sizeof(m_tolHSL));
}

TColorChecker::~TColorChecker()
{ }

bool TColorChecker::RightRGB(const PelRGB24& color)
{
	PelHSL24 hls = CvtRGBToHSL(color);
	return	RightHSL(hls);
}

PelHSL24 TColorChecker::CvtRGBToHSL(const PelRGB24& color)
{
	float	r = float(color.R)/RGBMAX;
	float	g = float(color.G)/RGBMAX;
	float	b = float(color.B)/RGBMAX;
	float	maxVal = Max(Max(r, g), b);
	float	minVal = Min(Min(r, g), b);
	float	delt   = maxVal - minVal;

	float	l = (maxVal + minVal)/2;
	float	s = 0.0f;
	float	h = 0.0f;
		
	if (maxVal != minVal)	//作为unsigned char,浮点数是可以精确表示。
	{
		if (l < 0.5f)
		{	s = delt / (maxVal + minVal);			}
		else
		{	s = delt / (2.0f - maxVal - minVal);	}

		unsigned char maxV = Max(color.R, Max(color.G, color.B));
		if(color.R == maxV)
		{
			h = (g - b)/delt;
		}
		else if (color.G == maxV)
		{
			h = 2.0f + (b - r)/delt;
		}
		else if (color.B == maxV)
		{
			h = 4.0f + (r - g)/delt;
		}
		h  /= 6.0f;
		if		(h < 0.0f )	h += 1.0f;
		else if (h > 1.0f )	h -= 1.0f;
	}	

	PelHSL24 resultHLS;
	resultHLS.H = RoundToInt(h*HSLMAX);
	resultHLS.S = RoundToInt(s*HSLMAX);
	resultHLS.L = RoundToInt(l*HSLMAX);
	return resultHLS;
}

PelLab24 TColorChecker::CvtRGBToLab	(const PelRGB24& color)
{
	double X = 0.412453*color.R + 0.357580*color.G + 0.180423*color.B;
	double Y = 0.212671*color.R + 0.715160*color.G + 0.072169*color.B;
	double Z = 0.019334*color.R + 0.119193*color.G + 0.950227*color.B;

	X /= RGBMAX;
	Y /= RGBMAX;
	Z /= RGBMAX;
	
	double Xn = 0.950455, Yn = 1.0, Zn = 1.088753; // The coordinates of the D65 white point
	double X1 = X/Xn;
	double Y1 = Y/Yn;
	double Z1 = Z/Zn;

	double exponent	= 1.0/3.0;
	double offset	= 16.0/116.0;

	X1 = (X1>0.008856) ? pow(X1, exponent) : 7.787*X1 + offset;
	Y1 = (Y1>0.008856) ? pow(Y1, exponent) : 7.787*Y1 + offset;
	Z1 = (X1>0.008856) ? pow(Z1, exponent) : 7.787*Z1 + offset;

	double L = 116.0*Y1 - 16.0;
	double a = 500.0*(X1 - Y1);
	double b = 200.0*(Y1 - Z1);
	return PelLab24(static_cast<char>(L), static_cast<char>(a), static_cast<char>(b));
}

bool TColorChecker::SetStandard(	const PelHSL24& hslValue, 
									const PelHSL24& hslTolerance)
{
	m_stdHSL = hslValue;
	m_tolHSL = hslTolerance;
	return true;
}

bool TColorChecker::SetStandard	(	const PelLab24& LabValue, 
									const char		distance)
{
	m_stdLab	= LabValue;
	m_distLab	= distance * distance;
	return true;
}

bool TColorChecker::RightColor(long realColor, long stdColor, long tolerance, long period)
{
	bool right = false;
	long minVal = stdColor - tolerance;
	long maxVal = stdColor + tolerance;
	if(minVal <= 0)
	{
		right = (( realColor >= (period+minVal) ) || ( realColor <= (stdColor+tolerance)));
	}
	else if(maxVal >= period)
	{
		right = (( realColor <= (maxVal-period) ) || ( realColor >= (stdColor-tolerance)));
	}
	else
	{
		right = (( realColor >= (stdColor-tolerance) ) && ( realColor <= (stdColor+tolerance)));
	}
	return right;
}
bool TColorChecker::RightHSL(const PelHSL24& color)
{
	bool	right = false;
	bool	rightH = RightColor(color.H, m_stdHSL.H, m_tolHSL.H);
	bool	rightL = RightColor(color.L, m_stdHSL.L, m_stdHSL.L);
	bool	rightS = RightColor(color.S, m_stdHSL.S, m_stdHSL.S);

	if (m_stdHSL.S < LOWSAT) {	right = rightL && rightS;	}
	else					 {	right = rightH;				}
	return right;
}

bool TColorChecker::RightLab(const PelLab24& color)
{
	size_t distance =	(color.a-m_stdLab.a)*(color.a-m_stdLab.a) + 
						(color.b-m_stdLab.b)*(color.b-m_stdLab.b);
	return distance <= m_distLab;
}


