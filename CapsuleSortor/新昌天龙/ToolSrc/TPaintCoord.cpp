
#include "TToInteger.h"
#include "TPaintCoord.h"


TPaintCoord::TPaintCoord(const FPoint& origin, float scaleX, float scaleY)
:m_origin(origin), m_scaleX(scaleX), m_scaleY(scaleY)
{	}

void TPaintCoord::SetOrigin	(const FPoint& origin)
{
	m_origin = ToCanvasSys(origin);
}

void TPaintCoord::SetZoom (float scaleX, float scaleY)
{
	m_scaleX = scaleX;
	m_scaleY = scaleY;
}

float TPaintCoord::ScaleX( )	{ return m_scaleX;	}

float TPaintCoord::ScaleY( )	{ return m_scaleY;	}

FPoint TPaintCoord::ToActualSys(const IPoint& position) const
{
	IPoint zoomXY = position - m_origin;
	return FPoint(zoomXY.x()/m_scaleX, zoomXY.y()/m_scaleY);
}

IPoint TPaintCoord::ToCanvasSys(const FPoint& position) const
{
	int zoomX = TToInteger::Round(position.x() * m_scaleX);
	int zoomY = TToInteger::Round(position.y() * m_scaleY);
	return m_origin + IPoint(zoomX, zoomY);
}

