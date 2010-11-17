#include "TPoint2D.h"

template<> static bool TPoint2DEq(const TPoint2D<float>& point0, const TPoint2D<float>& point1)
{
	double stdDiff = 1e-5;
	double diff0 = fabs(point1.x() - point0.x());
	double diff1 = fabs(point1.y() - point0.y());
	return ( (diff0<stdDiff) && (diff1<stdDiff) );
}

template<> static bool TPoint2DEq(const TPoint2D<double>& point0, const TPoint2D<double>& point1)
{
	double stdDiff = 1e-10;
	double diff0 = fabs(point1.x() - point0.x());
	double diff1 = fabs(point1.y() - point0.y());
	return ( (diff0<stdDiff) && (diff1<stdDiff) );
}

template<>  double TPoint2DCvt(const double value, const double flag)
{
	return value;
}

template<>  float  TPoint2DCvt(const double value, const float flag)
{
	return static_cast<float>(value);
}

