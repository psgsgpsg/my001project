#include "TRect2D.h"

template<>  double TRect2DPos(double pos0, double distance)
{
	return pos0 + distance;
}

template<>  float  TRect2DPos(float  pos0, float  distance)
{
	return pos0 + distance;
}

template<>  double TRect2DDist(double pos0, double pos1)
{
	double dist = pos1 - pos0;
	return (dist>=0) ? dist : -dist;
}

template<>  float  TRect2DDist(float  pos0, float  pos1)
{
	float dist = pos1 - pos0;
	return (dist>=0) ? dist : -dist;
}
