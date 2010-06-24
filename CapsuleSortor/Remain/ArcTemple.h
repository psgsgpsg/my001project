#ifndef ARCTEMPLE_H
#define ARCTEMPLE_H

#include <vector>

#include "TAllocTmpl.h"
#include "TRect2D.h"
#include "TPoint2D.h"

#include "TImageTmpl.h"
#include "TBmpBoard.h"
#include "TImgPainter.h"

typedef TAlloc<IPoint> CoorArray;

class ArcTemple
{
public:
	typedef enum {eUpArc, eDownArc } EArcPos;

private:
	friend class ArcCreator;
	EArcPos		m_eArcPos;
	IRect		m_range;
	CoorArray	m_coorArray;

public:
	EArcPos		GetArcPos	() const;
	IRect		GetArcRange () const;
	CoorArray	GetCoorArray() const;
};

class ArcCreator
{
public:
	static ArcTemple CreateArc(size_t radius, ArcTemple::EArcPos ePos);
};

class ArcMatchor
{
public:
	void	SetImage	  (const TImage<PelGray8>& image);
	bool	GetMaxMatchPos(const ArcTemple& arcTmpl, const IRect& range, IPoint& pos);

private:
	IPoint	GetMaxMatchPos	(const CoorArray& coorArray,	const IRect&  range, int step);
	int		GetMatchValue	(const CoorArray& coorArray,	const IPoint& center);
	IRect	GetCenRange		(const ArcTemple& arc,			const IRect&  ROI);

private:
	TImage<PelGray8>	m_image;
	std::vector<int>	m_matchVect;
};


#endif //ARCTEMPLE_H
