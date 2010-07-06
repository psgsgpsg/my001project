#ifndef TIMGDIM_H
#define TIMGDIM_H

class TImgDim
{
public:
	size_t width;
	size_t height;
	size_t bitsPerPixel;
	size_t bytesPerPixel;
	size_t bytesPerRow;
	

public:
	TImgDim(size_t Width			= 0, 
			size_t Height			= 0,
			size_t BytesPerPixel	= 1,
			size_t BytesPerRow		= -1,
			size_t BitsPerPixel		= -1);

	bool operator == (const TImgDim& src) const;
	bool operator != (const TImgDim& src) const;
};

#endif //TIMGDIM_H