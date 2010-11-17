#ifndef IMGPOINTPAIR_H
#define IMGPOINTPAIR_H

#include "ImgProjection.h"

class ImgPointPair
{
public:
	typedef ImgProjection::DIRECT DIRECT;
	typedef ImgProjection::Space  Space;
	typedef struct { TPoint2D<long> first; TPoint2D<long> last; } PointPair;

	ImgPointPair();
	~ImgPointPair();

public:
	bool			SetImage(const TImage<PelGray8>& img);
	bool			SearchPointPair(size_t index,	std::vector<PointPair>&	pairVect); 
	void			SearchAllPtPair(std::vector<PointPair>&	pairVect);

	std::vector<Space>	Locate(size_t minNoBlank=1, size_t ignorBlank=0);

private:
	bool PartPointPair(	size_t					index,
						DIRECT					dir, 
						TPoint2D<long>&			up, 
						TPoint2D<long>&			down,  
						std::vector<PointPair>&	pairVect);

	TPoint2D<long>	GetVertex(const TPoint2D<long> &start, DIRECT dir);

private:
	TImage<PelGray8>	m_img;
	long				m_imgWidth;
	long				m_imgHeight;

	ImgProjection		m_project;
	std::vector<Space>	m_position;
};

#endif //IMGPOINTPAIR_H

