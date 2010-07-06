
#include "ImgMaskCreator.h"
#include "ToInteger.h"
ImgMaskCreator::ImgMaskCreator()
:m_boundLineVal(255)
{ }

ImgMaskCreator::~ImgMaskCreator()
{ }

bool ImgMaskCreator::SetImage(const TImage<PelGray8>& img)
{
	m_img = img;
	return !m_img.IsNull();
}

void ImgMaskCreator::DrawLine(	const std::vector<PointPair>&	pairVect, 
								const std::vector<Space>&		position,
								PelGray8						lineVal)
{
	static const double PIdiv180 = 3.1415926/180.0;

	if(m_img.IsNull())	return;

	m_position		= position;
	m_boundLineVal	= lineVal;

	m_img.Maximize();

	size_t radius	= GetRadius();
	size_t size		= pairVect.size();
	for(size_t i=0; i<size; i++)
	{
		TPoint2D<long> point0 = pairVect[i].first;
		TPoint2D<long> point1 = pairVect[i].last;

		TPoint2D<long> diff   = point1 - point0;

		if(8 * abs(diff.x()) < abs(diff.y()) || diff.Mag()<radius/3 ) 
		{
			DrawLine(point0, point1);
		}
		else
		{
			DrawArc(point0, point1, radius);
		}
	}
}

void ImgMaskCreator::DrawArc(TPoint2D<long> point0, TPoint2D<long> point1, size_t radius)
{
	static const double PIdiv180 = atan(1.0)/45.0;
	long halfHight = m_img.Height()/2;
	long xCenter   = GetXCenter(point0.x(), point1.x());

	if( (point0.y()+point1.y())/2 < halfHight)
	{
		if(point0.x() > point1.x()) std::swap(point0, point1);
	}
	else
	{
		if(point0.x() < point1.x()) std::swap(point0, point1);
	}
	
	TPoint2D<long> diff   = point1 - point0;
	double diffMag = diff.Mag();
	double diffArg = diff.Arg();
	
	TPoint2D<long>  lineCenter   = (point0 + point1)*0.5;
	double rotRadius = sqrt(radius*radius + diffMag*diffMag*0.25);
	TPoint2D<long>	rotCenter;
	rotCenter.Polar(rotRadius, diffArg);
	if( (point0.y()+point1.y())/2 < halfHight)
	{
		rotCenter = lineCenter + rotCenter.Rotate(90.0*PIdiv180);
	}
	else
	{
		rotCenter = lineCenter - rotCenter.Rotate(-90.0*PIdiv180);
	}
	TPoint2D<long>  temp1	 = point0 - rotCenter;
	TPoint2D<long>  temp2    = point1 - rotCenter;
	double arg1 = temp1.Arg();
	double arg2 = temp2.Arg();
	const double PIdiv2 = 90*PIdiv180;
	if(arg1 < -PIdiv2 && arg2 > PIdiv2) arg1 = 360*PIdiv180 + arg1;
	if(arg2 < -PIdiv2 && arg1 > PIdiv2) arg2 = 360*PIdiv180 + arg2;
	double arg = arg2 -arg1;
	size_t steps	= size_t( abs(arg/ (5 * PIdiv180)) );
	steps			= (0==steps) ? 1 : steps;
	double stepLen	= arg/steps;
	double rotAngle = stepLen;
	TPoint2D<long>  insert0 = point0;
	for(int i=0; i<steps; ++i)
	{
		
		TPoint2D<long> insert1 = point0.Rotate(rotCenter, rotAngle);;
		DrawLine(insert0, insert1);
		rotAngle += stepLen;
		insert0   = insert1;
	}
	DrawLine(insert0, point1);
}
  
void ImgMaskCreator::DrawLine(TPoint2D<long> point0, TPoint2D<long> point1)
{
	if(m_img.IsNull()) return;
	//Î¢·Ö·¨
	long dx = point1.x() - point0.x();
	long dy = point1.y() - point0.y();
	long t  = max(abs(dx), abs(dy));
	if(t>0)
	{
		double ddx = dx*1.0/t;
		double ddy = dy*1.0/t;
		double x   = point0.x();
		double y   = point0.y();
		m_img.SetPixel(point0.x(), point0.y(), m_boundLineVal);
		while(t>=0)
		{
			--t;
		    x = x+ddx;
		    y = y+ddy;
			int x0 = RoundToInt (x);
			int y0 = RoundToInt (y);
		    m_img.SetPixel(x0, y0, m_boundLineVal);
		}
		m_img.SetPixel(point1.x(), point1.y(), m_boundLineVal);
	}
}


void ImgMaskCreator::ScanFill(PelGray8 fillVal)
{
	size_t size = m_position.size();
	for(size_t index =0; index < size; ++index)
	{
		std::vector<TRect2D<int> > rectV = GetFillRectV(index);
		size_t rectNum = rectV.size();
		for(size_t i=0; i<rectNum; ++i)
		{
			ScanFill(rectV[i], fillVal);
		}
	}
}

void ImgMaskCreator::ScanFill(const TRect2D<int>& range, PelGray8 fillVal)
{
	TRect2D<int> window = TRect2D<int>(	range.Center().x(), 
										range.y0(),
										range.x1(),
										range.y1());
	window = m_img.Window(window);
	TImage<PelGray8>::RowIter iterBegin = m_img.RowBegin();
	TImage<PelGray8>::RowIter iterEnd	= m_img.RowEnd();
	size_t w = window.Width();
	size_t x = 0;
	for(; iterBegin != iterEnd; ++iterBegin)
	{
		PelGray8 *pDataM = iterBegin.Base();
		PelGray8 *pDataL = pDataM-1;
		PelGray8 *pDataR = pDataM+1;
		*pDataM = fillVal;
		x = 0;
		while(*pDataL != m_boundLineVal && x<w)	{ *pDataL-- = fillVal; ++x; }
		x = 0;
		while(*pDataR != m_boundLineVal && x<w)	{ *pDataR++ = fillVal; ++x; }
	}
}

std::vector< TRect2D<int> > ImgMaskCreator::GetFillRectV(size_t index)
{
	std::vector<TRect2D<int> > rectV;
	
	m_img.Maximize();
	Space	xPos    = m_position[index];
	size_t height	= m_img.Height();
	TRect2D<int>	window ( TPoint2D<int>((xPos.first+xPos.last)/2+1, 0), 1, height);
	
	ImgProjection project;
	project.SetImage(m_img);
	project.Project(window, ImgProjection::DirNegX);
	
	std::vector<Space> yPosV = project.NoBlankArea(1);
	size_t size = yPosV.size();
	
	if(0 == size)
	{
		m_img.Maximize();
		Space	xPos    = m_position[index];
		TRect2D<int>	window ( TPoint2D<int>((xPos.first+xPos.last)/2-2, 0), 1, height);
		
		project.SetImage(m_img);
		project.Project(window, ImgProjection::DirNegX);
		
		yPosV = project.NoBlankArea(1);
		size = yPosV.size();	
	}
	
	if(false)
	{
		if(size > 1)
		{
			for(size_t i=1; i<size; ++i)
			{
				TRect2D<int> rect(	xPos.first,
					yPosV[i-1].last + 1,
					xPos.last,
					yPosV[i  ].first -1);
				rectV.push_back(rect);
			}
		}
	}
	else
	{
		if(size > 1)
		{
			long blackFirst = yPosV[0].last;
			long blackLast  = yPosV[1].first;
			for(size_t i=1; i<size-1; ++i)
			{
				if(blackLast - blackFirst < yPosV[i+1].first - yPosV[i].last)
				{
					blackFirst = yPosV[i].last;
					blackLast  = yPosV[i+1].first;
				}
			}
			TRect2D<int> rect(	xPos.first,
				blackFirst + 1,
				xPos.last,
				blackLast  - 1);
			rectV.push_back(rect);		
		}
	}
	return rectV;
}


size_t	ImgMaskCreator::GetRadius( )
{
	size_t size		= m_position.size();
	if(0 == size)		return 0;

	size_t sumSpace = 0;
	for(size_t index=0; index<size; ++index)
	{
		sumSpace += (m_position[index].last - m_position[index].first);
	}
	size_t radius =  sumSpace/(size*3);
	return radius;
}

long ImgMaskCreator::GetXCenter(long x0, long x1)
{
	long x = (x0+x1)/2;
	size_t size		= m_position.size();
	for(size_t index=0; index<size; ++index)
	{
		long first	= m_position[index].first;
		long last	= m_position[index].last;
		if( (x>=first) && (x<=last) )
		return (first + last)/2;
	}
	return x;
}

