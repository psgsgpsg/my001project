#include "ImgProjection.h"

ImgProjection::ImgProjection()
{ }

ImgProjection::~ImgProjection()
{ }

bool ImgProjection::SetImage(const TImage<PelGray8>& img)
{
	m_img = img;
	return (!m_img.IsNull());
}

bool ImgProjection::Project (const TRect2D<int>& rect, DIRECT dir)
{
	bool success = false;
	switch(dir)
	{
	case DirNegX:
	case DirPosX:
		success = ProjectX(rect);
		break;
	case DirNegY:
	case DirPosY:
		success = ProjectY(rect);
		break;
	default:
		success = false;
	}
	return success;
}

bool ImgProjection::ProjectX(TRect2D<int> rect)
{
	bool success = false;
	rect  = m_img.Window(rect);
	if(!m_img.IsNull())
	{
		size_t height = m_img.Height();
		if(m_project.Size() != height)
		{
			m_project = TAlloc<int>(height);
		}
		m_project.SetZero();

		int width		= m_img.Width();
		int *pProject	= m_project.Base();
		
		TImage<PelGray8>::RowIter iterBegin = m_img.RowBegin();
		TImage<PelGray8>::RowIter iterEnd	= m_img.RowEnd();
		for(; iterBegin != iterEnd; ++iterBegin)
		{
			PelGray8 *pData = iterBegin.Base();
			for(int x=0; x<width; ++x)
			{
				*pProject += *pData++;
			}
			pProject++;
		}
		success = false;
	}
	return success;
}

bool ImgProjection::ProjectY(TRect2D<int> rect)
{
	bool success = false;
	rect  = m_img.Window(rect);
	if(!m_img.IsNull())
	{
		int width	  = m_img.Width();
		if(m_project.Size() != width)
		{
			m_project = TAlloc<int>(width);
		}
		m_project.SetZero();

		int *pProject = m_project.Base();

		TImage<PelGray8>::RowIter iterBegin = m_img.RowBegin();
		TImage<PelGray8>::RowIter iterEnd	= m_img.RowEnd();
		for(; iterBegin != iterEnd; ++iterBegin)
		{
			PelGray8 *pData = iterBegin.Base();
			int		 *pProj = pProject;
			for(int x=0; x<width; ++x)
			{
				*pProj += *pData;
				++pProj;
				++pData;
			}
		}
		success = false;
	}
	return success;
}

bool ImgProjection::ProjectAdd(const ImgProjection& project)
{
	bool success = false;
	int size = ProjectSize();
	if(project.ProjectSize() == size)
	{
		for(size_t i=0; i<size; i++)
		{
			m_project[i] += project.m_project[i];
		}
		success = true;
	}
	return success;
}

size_t ImgProjection::ProjectSize() const
{
	return m_project.Size();
}

std::vector<ImgProjection::Space> ImgProjection::BlankArea(size_t minBlank, size_t maxIgnorNoBlank)
{
	std::vector<Space> result;
	size_t pos			= 0;
	size_t length		= m_project.Size();

	size_t blankFirst		= 0;
	size_t noBlankFirstPos	= 0;
	size_t noBlankSpace		= 0;
	while(pos < length)
	{
		size_t first = FindBlankBegin	(pos, length);
		size_t last  = FindBlankEnd		(first+1, length);

		if( -1 == first)	{	break;	}

		noBlankSpace	= first - noBlankFirstPos;
		noBlankFirstPos	= last + 1;

		if((0==blankFirst) || (noBlankSpace>maxIgnorNoBlank))
		{
			blankFirst = first;
		}

		if(noBlankSpace<=maxIgnorNoBlank)
		{
			pos = last+1;		continue;
		}

		size_t blankSpace = last - blankFirst + 1;
		if(blankSpace >= minBlank)
		{
			result.push_back(Space(blankFirst, last));
			blankFirst = 0;
		}
		pos = last+1;
	}
	return result;
}
std::vector<ImgProjection::Space> ImgProjection::NoBlankArea(size_t minNoBlank, size_t maxIgnorBlank)
{
	std::vector<Space> result;
	size_t pos			= 0;
	size_t length		= m_project.Size();

	//fill IgnorBlank;
	while(pos < length)
	{
		size_t first = FindBlankBegin(pos,	 length);
		size_t last  = FindBlankEnd  (first+1, length);

		if( -1 == first ) {		break;	}

		size_t blankSpace = last - first + 1;
		if(blankSpace <= maxIgnorBlank)
		{
			memset(m_project.Base() + first, 1, blankSpace*sizeof(m_project[0]));
		}
		pos = last + 1;
	}

	//search NoBlank;
	pos = 0;
	while(pos < length)
	{
		size_t first = FindNoBlankBegin(pos,	 length);
		size_t last  = FindNoBlankEnd  (first+1, length);

		if( -1 == first )	{	break;	}

		size_t noBlankSpace = last - first + 1;
		if(noBlankSpace >= minNoBlank)
		{
			result.push_back(Space(first, last));
		}
		pos = last+1;
	}
	return result;
}

size_t ImgProjection::FindBlankBegin(size_t pos, size_t length)
{
	if((0 == pos) && (0 == m_project[pos]))	return pos;

	while(0 != m_project[pos] && (pos<length) ) ++pos;

	return pos < length ? pos : -1;
}

size_t ImgProjection::FindBlankEnd(size_t pos, size_t length)
{
	while(0 == m_project[pos] && (pos<length) ) ++pos;

	return --pos;
}

size_t ImgProjection::FindNoBlankBegin(size_t pos, size_t length)
{
	if((0 == pos) && (0 != m_project[pos]))	return pos;

	while(0 == m_project[pos] && (pos<length) ) ++pos;

	return pos < length ? pos : -1;
}

size_t ImgProjection::FindNoBlankEnd(size_t pos, size_t length)
{
	while(0 != m_project[pos] && (pos<length) ) ++pos;

	return --pos;
}