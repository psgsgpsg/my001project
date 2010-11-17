#ifndef IMGMASKCREATOR_H
#define IMGMASKCREATOR_H

#include "ImgProjection.h"
#include "ImgPointPair.h"
#include "TImageTmpl.h"

class ImgMaskCreator
{
public:
	typedef ImgProjection::DIRECT	DIRECT;
	typedef ImgProjection::Space	Space;
	typedef ImgPointPair::PointPair PointPair;

	ImgMaskCreator();
	~ImgMaskCreator();

public:
	bool	SetImage(	const TImage<PelGray8>& img );

	void	DrawLine(	const std::vector<PointPair>&	pairVect, 
						const std::vector<Space>&		position,
						PelGray8						lineVal);

	void	ScanFill(	PelGray8 fillVal );

private:
	void	DrawLine(	TPoint2D<long> point0,
						TPoint2D<long> point1);

	void	DrawArc(	TPoint2D<long> point0, 
						TPoint2D<long> point1, 
						size_t radius);

	size_t						GetRadius	( );
	long						GetXCenter	(long x0, long x1);
	std::vector< TRect2D<int> > GetFillRectV(size_t index);
	void						ScanFill	(const TRect2D<int>& range, PelGray8 fillVal);

private:
	TImage<PelGray8>	m_img;
	std::vector<Space>	m_position;
	PelGray8			m_boundLineVal;
};

#endif //IMGMASKCREATOR_H

