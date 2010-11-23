#include "ArcTemple.h"
#include <vector>

ArcTemple::EArcPos	ArcTemple::GetArcPos() const
{
	return m_eArcPos;
}
IRect ArcTemple::GetArcRange () const
{
	return m_range;
}
CoorArray ArcTemple::GetCoorArray() const
{
	return m_coorArray;
}

ArcTemple ArcCreator::CreateArc(size_t radius, ArcTemple::EArcPos ePos)
{
	int width = (radius*2 +3) & (~3);
	int height= width/2;
	
	TBmpInfo info;
	info.Width(width);
	info.Height(height);
	info.AccordMem();

	TBmpBoard bmpBoard;
	bmpBoard.SetBmpInfo(info);
	HDC memHDC = bmpBoard.GetMemDC();

	TImgPainter painter(memHDC);
	painter.SetPenParam(RGB(255,255,255), 3);
	
	IPoint center;
	if(ArcTemple::eUpArc == ePos)
	{
		center = IPoint(width/2, height);
		painter.Arc(center, static_cast<float>(radius), static_cast<float>(radius), FPoint(100,-30), FPoint(-100,-30));
	}
	else if(ArcTemple::eDownArc == ePos)
	{
		center = IPoint(width/2, 0);
		painter.Arc(center, static_cast<float>(radius),static_cast<float>(radius), FPoint(-100, 30), FPoint(100, 30));
	}

	PelGray8 *pPel = bmpBoard.GetPixelBase();
	std::vector<IPoint> coorVect;
	for(int j=0; j<height; ++j)
	{
		for(int i=0; i<width; ++i)
		{
			if(*(pPel+j*width +i))
			{
				coorVect.push_back(IPoint(i,j)-center);
			}
		}
	}

	IRect range(IPoint(0,0),width, height);
	range.TranslateCenterTo(IPoint(0,0));

	size_t count = coorVect.size();
	CoorArray array(count);
	for(size_t m=0; m<count; ++m)
	{
		array[m] = coorVect[m];
	}
	ArcTemple arcTemple;
	arcTemple.m_eArcPos		= ePos;
	arcTemple.m_range		= range;
	arcTemple.m_coorArray	= array;
	return arcTemple;
}

void ArcMatchor::SetImage(const TImage<PelGray8>& image)
{
	m_image = image;
}
int ArcMatchor::GetMatchValue(const CoorArray& coorArray, const IPoint& center)
{
	int cenX = center.x();
	int cenY = center.y();

	size_t count = coorArray.Size();
	IPoint* pArray = (IPoint*)coorArray.Base();
	
	int sum = 0;
	for(size_t i=0; i<count; ++i, ++pArray)
	{
		int val = m_image((*pArray).x()+cenX, (*pArray).y()+cenY);
		if(val > 5)
		{
			sum += val;
		}
	}
	return sum;
}

IRect ArcMatchor::GetCenRange(const ArcTemple& arc, const IRect& ROI)
{
	IRect maxWnd = m_image.Maximize();
	IRect imgROI = maxWnd.Intersect(ROI);
	IRect arcRng = arc.GetArcRange();

	IRect cenRng;
	int diffWidth= imgROI.Width()	- arcRng.Width();
	int diffHeight=imgROI.Height()	- arcRng.Height();
	if((diffWidth>0) && (diffHeight>0))
	{
		cenRng = IRect(IPoint(imgROI.x0(), imgROI.y0()), diffWidth, diffHeight);
		if(ArcTemple::eUpArc == arc.GetArcPos())
		{
			cenRng.Translate(arcRng.Width()/2, arcRng.Height());
		}
		else if(ArcTemple::eDownArc == arc.GetArcPos())
		{
			cenRng.Translate(arcRng.Width()/2, 0);
		}
	}
	return cenRng;
}

bool ArcMatchor::GetMaxMatchPos(const ArcTemple& arcTmpl, const IRect& range, IPoint& pos)
{
	bool success = false;
	IRect cenRng = GetCenRange(arcTmpl, range);
	if(!cenRng.IsNull())
	{
		pos = GetMaxMatchPos(arcTmpl.GetCoorArray(), cenRng, 2);
		/*IRect  newRng(pos, 1, 1);
		newRng.Expand(2,2);
		newRng	= newRng.Intersect(cenRng);
		if(!newRng.IsNull())
		{
			pos	= GetMaxMatchPos(arcTmpl.GetCoorArray(), newRng, 1);
		}*/
		success = true;
	}
	return success;
}

IPoint ArcMatchor::GetMaxMatchPos(const CoorArray& coorArray, const IRect& range, int step)
{
	int x0 = range.x0();
	int y0 = range.y0();
	int x1 = range.x1();
	int y1 = range.y1();
	int maxSum = 0;
	IPoint pos(0,0);
	for(int y=y0; y<=y1; y+=step)
	{
		for(int x=x0; x<=x1; x+=step)
		{
			int curSum = GetMatchValue(coorArray, IPoint(x, y));
			if(curSum > maxSum)
			{
				maxSum = curSum;  pos = IPoint(x, y);
			}
		}
	}
	return pos;
}

bool ArcMatchor::GetMaxMatchPos(const vector<POINT>& arcTmpl, const IRect& range, IPoint& pos)
{
	bool success = false;
	//起始点位置区域
	IRect cenRng = GetCenRange(arcTmpl, range);
	if(!cenRng.IsNull())
	{
		pos = GetMaxMatchPos(arcTmpl, cenRng, 2);
		IRect  newRng(pos, 1, 1);
		newRng.Expand(2,2);
		newRng	= newRng.Intersect(cenRng);
		if(!newRng.IsNull())
		{
			pos	= GetMaxMatchPos(arcTmpl, newRng, 1);
		}
		success = true;
	}
	return success;
}

IPoint ArcMatchor::GetMaxMatchPos(const vector<POINT>& coorArray, const IRect& range, int step)
{
	int x0 = range.x0();
	int y0 = range.y0();
	int x1 = range.x1();
	int y1 = range.y1();
	int maxSum = 0;
	IPoint pos(0,0);
	for(int y=y0; y<=y1; y+=step)
	{
		for(int x=x0; x<=x1; x+=step)
		{
			int curSum = GetMatchValue(coorArray, IPoint(x, y));
			if(curSum > maxSum)
			{
				maxSum = curSum;  pos = IPoint(x, y);
			}
		}
	}
	return pos;
}

int ArcMatchor::GetMatchValue(const vector<POINT>& coorArray, const IPoint& center)
{
	int cenX = center.x();
	int cenY = center.y();

	size_t count = coorArray.size();
	//IPoint* pArray = (IPoint*)coorArray.Base();

	int sum = 0;
	for(size_t i=0; i<count; ++i)
	{
		int val = m_image.GetPixel(coorArray.at(i).x + cenX, coorArray.at(i).y +cenY);
		if(val > 5) 
		{
			sum += val;
		}
	}
	return sum;
}

IRect ArcMatchor::GetCenRange(const vector<POINT>& arc, const IRect& ROI)
{
	IRect maxWnd = m_image.Maximize();
	IRect imgROI = maxWnd.Intersect(ROI);

	int maxWidth = 0;
	int maxHeight = 0;
	for (size_t i = 0; i < arc.size(); i++)
	{
		if (arc.at(i).x > maxWidth)
		{
			maxWidth = arc.at(i).x;
		}
		if (arc.at(i).y > maxHeight)
		{
			maxHeight = arc.at(i).y;
		}
	}
	
	
	IRect cenRng(0, 0, ROI.Width() - maxWidth, ROI.Height() - maxHeight);
	
	return cenRng;
}