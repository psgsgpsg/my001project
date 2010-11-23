#ifndef IMGPROJECTION_H
#define IMGPROJECTION_H

#include "TRect2D.h"
#include "TAllocTmpl.h"
#include "TImageTmpl.h"
#include <vector>

class ImgProjection
{
public:
	typedef enum	{ DirNegX, DirPosX, DirNegY, DirPosY } DIRECT;
	typedef struct SpaceStruct
	{
		long first; 
		long last;
		SpaceStruct(long pos0 = 0, long pos1 = 0)
		: first(pos0), last(pos1) 
		{	}
	} Space;

	ImgProjection();
	~ImgProjection();

public:
	bool SetImage(const TImage<PelGray8>& img);
	bool Project (const TRect2D<int>& rect, DIRECT dir);
	bool ProjectX(TRect2D<int> rect);
	bool ProjectY(TRect2D<int> rect);
	bool ProjectAdd(const ImgProjection& project);
	std::vector<Space>	 BlankArea	(	size_t minBlank			= 1, 
										size_t maxIgnorNoBlank	= 0);
	std::vector<Space>	 NoBlankArea(	size_t minNoBlank		= 1,
										size_t maxIgnorBlank	= 0);
	size_t ProjectSize() const;

private:
	size_t FindBlankBegin	(size_t pos, size_t length);
	size_t FindBlankEnd		(size_t pos, size_t length);
	size_t FindNoBlankBegin	(size_t pos, size_t length);
	size_t FindNoBlankEnd	(size_t pos, size_t length);

private:
	TImage<PelGray8>	m_img;
	TAlloc<int>			m_project;
};

#endif //IMGPROJECTION_H