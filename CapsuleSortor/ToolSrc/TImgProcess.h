#ifndef TIMGPROCESS_H
#define TIMGPROCESS_H

#include "TImageTmpl.h"

class TImgProcess
{
public:
	static bool RGBToGray(	TImage<PelRGB24> inImg, 
							TImage<PelGray8> outImg);

	static bool RGBToGray2(	TImage<PelRGB24> inImg, 
							TImage<PelGray8> outImg);

	static bool BinaryGray(	TImage<PelGray8> inImg,		
							TImage<PelGray8> outImg,
							PelGray8		 bandMin,
							PelGray8		 bandMax);

	static bool AndGrayImg(	TImage<PelGray8> inImg0,		
							TImage<PelGray8> inImg1,
							TImage<PelGray8> outImg);

	static bool SwapBmpRB(	TImage<PelRGB24> inImg,
							TImage<PelRGB24> outImg);
	static bool	Filter_MeanTwoLine	(TImage<PelRGB24> imImg,
									 TImage<PelRGB24> outImg);
	static bool ProjectYDir(TImage<PelGray8> inImg, 
							TImage<PelGray8> &outProject);

	static bool SobelFilter(const TImage<PelGray8>& image,
							TImage<PelGray8>& dstImg);

private:
	static bool RGBToGraySSE (	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg);

	static bool RGBToGray2SSE (	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg);
	

	static bool RGBToGrayCOM (	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg);

	static bool RGBToGray2COM (	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg);

	static bool BinaryGraySSE(	TImage<PelGray8> inImg,		
								TImage<PelGray8> outImg,
								PelGray8		 bandMin,
								PelGray8		 bandMax);
	
	static bool BinaryGrayCOM(	TImage<PelGray8> inImg,		
								TImage<PelGray8> outImg,
								PelGray8		 bandMin,
								PelGray8		 bandMax);

	static bool AndGrayImgCOM(	TImage<PelGray8> inImg0,		
								TImage<PelGray8> inImg1,
								TImage<PelGray8> outImg);
	
	static bool AndGrayImgSSE(	TImage<PelGray8> inImg0,		
								TImage<PelGray8> inImg1,
								TImage<PelGray8> outImg);

	static bool	Filter_MeanTwoLineCOM(TImage<PelRGB24> inImg,
									  TImage<PelRGB24> outImg);
	static bool Filter_MeanTwoLineSSE(TImage<PelRGB24> inImg,
									  TImage<PelRGB24> outImg);
	
	static bool ProjectYDirCOM(	TImage<PelGray8> inImg, 
								TImage<PelGray8> &outProject);
	static bool ProjectYDirSSE(	TImage<PelGray8> inImg, 
								TImage<PelGray8> &outProject);
private:
	const static bool m_supportSSE2;
};

#endif //TIMGPROCESS_H
