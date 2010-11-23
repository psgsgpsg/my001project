
#include "ImgPointPair.h"

ImgPointPair::ImgPointPair()
 :	m_imgWidth(0),
	m_imgHeight(0)
{ }

ImgPointPair::~ImgPointPair()
{ }

bool ImgPointPair::SetImage(const TImage<PelGray8>& img)
{
	m_img		= img;
	m_imgWidth  = img.Width();
	m_imgHeight = img.Height();

	m_project.SetImage(m_img);

	return (!m_img.IsNull()); 
}

std::vector<ImgPointPair::Space> ImgPointPair::Locate(size_t minNoBlank, size_t ignorBlank)
{
	TRect2D<int> rect(TPoint2D<int>(0,0), m_imgWidth, m_imgHeight);

	m_project.Project(rect, ImgProjection::DirPosY);
	m_position = m_project.NoBlankArea(minNoBlank, ignorBlank);
	return m_position;
}

TPoint2D<long> ImgPointPair::GetVertex(const TPoint2D<long> &start, DIRECT dir)
{
	long x = start.x();
	long y = start.y();
	switch(dir)
	{
	case ImgProjection::DirNegX:
		while(m_img.GetPixel(x, y) ==0)	--x;
		break;
	case ImgProjection::DirPosX:
		while(m_img.GetPixel(x, y) ==0)	++x;
		break;
	case ImgProjection::DirNegY:
		while(m_img.GetPixel(x, y) ==0)	--y;
		break;
	case ImgProjection::DirPosY:
		while(m_img.GetPixel(x, y) ==0)	++y;
		break;
	default:
		;
	}
	return TPoint2D<long>(x, y);
}

bool ImgPointPair::SearchPointPair(size_t index, std::vector<PointPair>& pairVect) 
{
	if(index >= m_position.size())	return false;

	//Left part
	TPoint2D<long> leftUpPoint, leftDownPoint;
	bool leftSuccess =	PartPointPair(	index,
						ImgProjection::DirNegX, 
						leftUpPoint, 
						leftDownPoint,  
						pairVect);

	//Right part
	TPoint2D<long> rightUpPoint, rightDownPoint;
	bool rightSuccess=	PartPointPair(	index,
						ImgProjection::DirPosX, 
						rightUpPoint, 
						rightDownPoint,  
						pairVect);

	if(leftSuccess && rightSuccess)
	{
		PointPair pair;
		if(leftUpPoint != rightUpPoint)
		{
			pair.first = leftUpPoint;
			pair.last  = rightUpPoint;
			pairVect.push_back(pair);
		}
		if(leftDownPoint != rightDownPoint)
		{
			pair.first = leftDownPoint;
			pair.last  = rightDownPoint;
			pairVect.push_back(pair);
		}
	}
	return (leftSuccess && rightSuccess);
}

bool ImgPointPair::PartPointPair(	size_t					index,
									DIRECT					dir, 
									TPoint2D<long>&			up, 
									TPoint2D<long>&			down,  
									std::vector<PointPair>&	pairVect)
{
	bool success = false;
	Space pos	= m_position[index];
	long  midX	= (pos.last+pos.first)/2;
	long  leftX  = 0, rightX = 0;
	switch(dir)
	{
	case ImgProjection::DirNegX:
		leftX	= pos.first;
		rightX	= midX;
		break;
	case ImgProjection::DirPosX:
		leftX	= midX;
		rightX	= pos.last;
		break;
	default:
		leftX	= 0;
		rightX	= 0;
	}

	TRect2D<int> rect(leftX, 5, rightX, m_imgHeight-5); 

	m_project.Project(rect, dir);
	std::vector<Space> vect = m_project.NoBlankArea(1);
	if(!vect.empty())
	{
		TPoint2D<long> upPoint		= TPoint2D<long>(midX, vect.front().first);
		TPoint2D<long> downPoint	= TPoint2D<long>(midX, vect.back().last);

		up	= GetVertex(upPoint,   dir);
		down= GetVertex(downPoint, dir); 

		ImgPointPair::PointPair pair;
		int size = vect.size();
		for(int i=0; i<size-1; i++)
		{
			pair.first = TPoint2D<long>(midX,	vect[i  ].last);
			pair.last  = TPoint2D<long>(midX,	vect[i+1].first);
			pair.first = GetVertex(pair.first,	dir);
			pair.last  = GetVertex(pair.last,	dir);
			pairVect.push_back(pair);
		}
		success = true;
	}
	return success;
}

void ImgPointPair::SearchAllPtPair(std::vector<PointPair>&	pairVect)
{
	pairVect.clear();
	size_t size = m_position.size();
	for(size_t i = 0; i<size; ++i)
	{
		SearchPointPair(i, pairVect);
	}
}

