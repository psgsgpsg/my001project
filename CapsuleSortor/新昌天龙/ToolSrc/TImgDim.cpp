// struct of image dimension;
#include "TImgDim.h"

TImgDim::TImgDim(size_t Width,
				 size_t Height,
				 size_t BytesPerPixel,
				 size_t BytesPerRow,
				 size_t BitsPerPixel)
:width		(Width),
height		(Height), 
bytesPerPixel(BytesPerPixel),
bytesPerRow	(BytesPerRow),
bitsPerPixel(BitsPerPixel)
{
	size_t rowBytes = bytesPerPixel * width;
	if(-1 == bytesPerRow || bytesPerRow < rowBytes)
	{	bytesPerRow = rowBytes;	}

	if(-1 == bitsPerPixel)
	{	bitsPerPixel= bytesPerPixel * 8;	}
}

bool TImgDim::operator == (const TImgDim& src) const
{
	return( (width			== src.width)			&& 
			(height			== src.height)			&&
			(bytesPerPixel	== src.bytesPerPixel)	&&
			(bytesPerRow	== src.bytesPerRow)		&&
			(bitsPerPixel	== src.bitsPerPixel)	);
}
	
bool TImgDim::operator != (const TImgDim& src) const
{
	return( !(this->operator==(src)));
}
