#ifndef TCOORDINATE_H
#define TCOORDINATE_H

#include "TPoint2D.h"

class TPaintCoord
{
public:
	TPaintCoord(const FPoint& origin = FPoint(0.0f, 0.0f),
				float scaleX = 1.0f,
				float scaleY = 1.0f);
	//default copy construct
	//default operator=();
	//default destruct;
public:
	void SetOrigin	(const FPoint& origin);
	void SetZoom	(float scaleX, float scaleY);
	float ScaleX	( );
	float ScaleY	( );

	FPoint ToActualSys(const IPoint& position) const;
	IPoint ToCanvasSys(const FPoint& position) const;
private:
	TPoint2D<int>	m_origin;
	float			m_scaleX;
	float			m_scaleY;
};

#endif //TCOORDINATE_H