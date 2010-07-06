
#include "TImgProcess.h"
#include <emmintrin.h>
#include "TSIMDChecker.h"

const bool TImgProcess::m_supportSSE2 = TSIMDChecker::IsSupportSSE2();

bool TImgProcess::RGBToGray(	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg)
{
	return (m_supportSSE2) ? RGBToGraySSE(inImg, outImg)	\
							: RGBToGrayCOM(inImg, outImg);
}

bool TImgProcess::RGBToGray2(	TImage<PelRGB24> inImg, 
								TImage<PelGray8> outImg)
{
	return (m_supportSSE2) ? RGBToGray2SSE(inImg, outImg)	\
							: RGBToGray2COM(inImg, outImg);
}

bool TImgProcess::BinaryGray(	TImage<PelGray8> inImg,		
								TImage<PelGray8> outImg,
								PelGray8		 bandMin,
								PelGray8		 bandMax)
{
	return (m_supportSSE2) ? BinaryGraySSE(inImg, outImg, bandMin, bandMax)		\
							: BinaryGrayCOM(inImg, outImg, bandMin, bandMax);
}

bool TImgProcess::AndGrayImg(	TImage<PelGray8> inImg0,		
								TImage<PelGray8> inImg1,
								TImage<PelGray8> outImg)
{
	return (m_supportSSE2) ? AndGrayImgSSE(inImg0, inImg1, outImg)		\
							: AndGrayImgCOM(inImg0, inImg1, outImg);
}

bool TImgProcess::ProjectYDir(	TImage<PelGray8> inImg, 
								TImage<PelGray8> &outProject)
{
	return (m_supportSSE2) ? ProjectYDirSSE(inImg, outProject)	\
							: ProjectYDirCOM(inImg, outProject);
}

bool TImgProcess::SwapBmpRB(	TImage<PelRGB24> inImg,
								TImage<PelRGB24> outImg)
{
	TImage<PelRGB24>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelRGB24>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelRGB24>::RowIter inIterBegin  = inImg.RowBegin();

	const size_t repeatCount = outImg.Width();
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelRGB24*	RGBi	= inIterBegin.Base();  
		PelRGB24*	RGBo	= outIterBegin.Base();
		for(size_t index=0; index < repeatCount; ++index)
		{
			PelGray8 red = RGBi->R;
			RGBo->R = RGBi->B;
			RGBo->G = RGBi->G;
			RGBo->B = red;
			++RGBi;
			++RGBo;
		}
	}
	return true;
}

bool TImgProcess::RGBToGrayCOM(	TImage<PelRGB24> inImg,
								TImage<PelGray8> outImg)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelRGB24>::RowIter inIterBegin  = inImg.RowBegin();

	const size_t repeatCount = outImg.Width();
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelRGB24*	RGB		= inIterBegin.Base();  
		PelGray8*	Gray	= outIterBegin.Base();
		for(size_t index=0; index < repeatCount; ++index)
		{
			*Gray = max(max(RGB->R, RGB->G), RGB->B);
			++Gray;
			++RGB;
		}
	}
	return true;
}

bool TImgProcess::RGBToGray2COM (	TImage<PelRGB24> inImg, 
									TImage<PelGray8> outImg)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelRGB24>::RowIter inIterBegin  = inImg.RowBegin();
	
	const size_t repeatCount = outImg.Width();
	PelGray8 maxVal = 0, minVal =0, midVal = 0;
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelRGB24*	RGB		= inIterBegin.Base();  
		PelGray8*	Gray	= outIterBegin.Base();
		for(size_t index=0; index < repeatCount; ++index)
		{
			if(RGB->R > RGB->G)
			{	maxVal = RGB->R;	minVal = RGB->G;	}
			else
			{	maxVal = RGB->G;	minVal = RGB->R;	}

			if     (RGB->B > maxVal)
			{	midVal = maxVal;	maxVal = RGB->B;	}
			else if(RGB->B < minVal)
			{	midVal = minVal;	minVal = RGB->B;	}
			else
			{	midVal = RGB->B;						}

			*Gray = (((maxVal+minVal)>>1) + ((maxVal+midVal)>>1) )>>1;
			++Gray;
			++RGB;
		}
	}
	return true;	
}


bool TImgProcess::BinaryGrayCOM(	TImage<PelGray8>	inImg, 
									TImage<PelGray8>	outImg,
									PelGray8			bandMin,
									PelGray8			bandMax)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelGray8>::RowIter inIterBegin  = inImg.RowBegin();

	const size_t repeatCount = outImg.Width();
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelGray8*	pSrc	= inIterBegin.Base();  
		PelGray8*	pDst	= outIterBegin.Base();
	
		for(size_t index=0; index < repeatCount; ++index)
		{
			if( (*pSrc>=bandMin) && (*pSrc<=bandMax))
			{
				*pDst = 255;
			}
			else
			{
				*pDst = 0;
			}
		
			++pSrc;
			++pDst;
		}
	}
	return true;
}

bool TImgProcess::AndGrayImgCOM(TImage<PelGray8> inImg0,		
								TImage<PelGray8> inImg1,
								TImage<PelGray8> outImg)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelGray8>::RowIter inIterBegin0 = inImg0.RowBegin();
	TImage<PelGray8>::RowIter inIterBegin1 = inImg1.RowBegin();
	
	const size_t countPerTime= sizeof(unsigned int);
	const size_t repeatCount = (outImg.Width() + countPerTime -1) / countPerTime;
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin0, ++inIterBegin1)
	{
		unsigned int *pSrc0 = (unsigned int*)inIterBegin0.Base();
		unsigned int *pSrc1 = (unsigned int*)inIterBegin1.Base();
		unsigned int *pDst  = (unsigned int*)outIterBegin.Base();		
		for(size_t index=0; index < repeatCount; ++index)
		{
			*pDst = *pSrc0 & *pSrc1;
			++pSrc0;
			++pSrc1;
			++pDst;
		}
	}
	return true;
}

bool TImgProcess::RGBToGraySSE(	TImage<PelRGB24> inImg,
								TImage<PelGray8> outImg)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelRGB24>::RowIter inIterBegin  = inImg.RowBegin();

	const size_t countPerTime= 16;
	const size_t repeatCount = (outImg.Width() + countPerTime -1) / countPerTime;

	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelRGB24*	RGB		= inIterBegin.Base();  
		__m128i*	Gray	= (__m128i*)outIterBegin.Base();
		for(size_t index=0; index < repeatCount; ++index)
		{
			//R128 : RRR...RRR
			__m128i R128 = _mm_set_epi8(
					RGB[15].R,	RGB[14].R,	RGB[13].R,	RGB[12].R,
					RGB[11].R,	RGB[10].R,	RGB[9].R,	RGB[8].R,
					RGB[7].R,	RGB[6].R,	RGB[5].R,	RGB[4].R,
					RGB[3].R,	RGB[2].R,	RGB[1].R,	RGB[0].R);
			//G128 : GGG...GGG
			__m128i G128 = _mm_set_epi8(
					RGB[15].G,	RGB[14].G,	RGB[13].G,	RGB[12].G,
					RGB[11].G,	RGB[10].G,	RGB[9].G,	RGB[8].G,
					RGB[7].G,	RGB[6].G,	RGB[5].G,	RGB[4].G,
					RGB[3].G,	RGB[2].G,	RGB[1].G,	RGB[0].G);
			//B128 : BBB...BBB
			__m128i B128 = _mm_set_epi8(
					RGB[15].B,	RGB[14].B,	RGB[13].B,	RGB[12].B,
					RGB[11].B,	RGB[10].B,	RGB[9].B,	RGB[8].B,
					RGB[7].B,	RGB[6].B,	RGB[5].B,	RGB[4].B,
					RGB[3].B,	RGB[2].B,	RGB[1].B,	RGB[0].B);
		
			__m128i value =  _mm_max_epu8(R128,_mm_max_epu8(G128,B128));
			_mm_store_si128(Gray, value);

			++Gray;
			RGB += countPerTime;
		}
	}
	return true;
}

bool TImgProcess::RGBToGray2SSE (	TImage<PelRGB24> inImg, 
									TImage<PelGray8> outImg)

{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelRGB24>::RowIter inIterBegin  = inImg.RowBegin();
	
	const size_t countPerTime= 16;
	const size_t repeatCount = (outImg.Width() + countPerTime -1) / countPerTime;
	
	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		PelRGB24*	RGB		= inIterBegin.Base();  
		__m128i*	Gray	= (__m128i*)outIterBegin.Base();
		for(size_t index=0; index < repeatCount; ++index)
		{
			//R128 : RRR...RRR
			__m128i R128 = _mm_set_epi8(
				RGB[15].R,	RGB[14].R,	RGB[13].R,	RGB[12].R,
				RGB[11].R,	RGB[10].R,	RGB[9].R,	RGB[8].R,
				RGB[7].R,	RGB[6].R,	RGB[5].R,	RGB[4].R,
				RGB[3].R,	RGB[2].R,	RGB[1].R,	RGB[0].R);
			//G128 : GGG...GGG
			__m128i G128 = _mm_set_epi8(
				RGB[15].G,	RGB[14].G,	RGB[13].G,	RGB[12].G,
				RGB[11].G,	RGB[10].G,	RGB[9].G,	RGB[8].G,
				RGB[7].G,	RGB[6].G,	RGB[5].G,	RGB[4].G,
				RGB[3].G,	RGB[2].G,	RGB[1].G,	RGB[0].G);
			//B128 : BBB...BBB
			__m128i B128 = _mm_set_epi8(
				RGB[15].B,	RGB[14].B,	RGB[13].B,	RGB[12].B,
				RGB[11].B,	RGB[10].B,	RGB[9].B,	RGB[8].B,
				RGB[7].B,	RGB[6].B,	RGB[5].B,	RGB[4].B,
				RGB[3].B,	RGB[2].B,	RGB[1].B,	RGB[0].B);
			
			__m128i maxVal = _mm_max_epu8(R128,_mm_max_epu8(G128,B128));

			__m128i meanVal= _mm_avg_epu8(_mm_avg_epu8(R128, G128), _mm_avg_epu8(B128, maxVal));

			_mm_store_si128(Gray, meanVal);
			
			++Gray;
			RGB += countPerTime;
		}
	}
	return true;
}						   

bool TImgProcess::BinaryGraySSE(	TImage<PelGray8>	inImg, 
									TImage<PelGray8>	outImg,
									PelGray8			bandMin,
									PelGray8			bandMax)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelGray8>::RowIter inIterBegin  = inImg.RowBegin();

	const size_t countPerTime= 16;
	const size_t repeatCount = (outImg.Width() + countPerTime -1) / countPerTime;

	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin)
	{
		__m128i*	pSrc128	= (__m128i*)inIterBegin.Base();  
		__m128i*	pDst128	= (__m128i*)outIterBegin.Base();
	
		__m128i		zero		= _mm_setzero_si128();
		__m128i		thresMin	= _mm_set1_epi8(bandMin);
		__m128i		thresMax	= _mm_set1_epi8(bandMax);
	
		for(size_t index=0; index < repeatCount; ++index)
		{
			__m128i pixel128 = _mm_load_si128(pSrc128);
			//(gray<maxthres)?1:0
			__m128i Mask1 = _mm_cmpeq_epi8(_mm_subs_epu8(pixel128, thresMax), zero);
			//(gray>minthres)?1:0
			__m128i Mask2 = _mm_cmpeq_epi8(_mm_subs_epu8(thresMin, pixel128), zero);
		
			_mm_store_si128(pDst128, _mm_and_si128(Mask1,Mask2));
		
			++pSrc128;
			++pDst128;
		}
	}
	return true;
}

bool TImgProcess::AndGrayImgSSE(	TImage<PelGray8> inImg0,		
									TImage<PelGray8> inImg1,
									TImage<PelGray8> outImg)
{
	TImage<PelGray8>::RowIter outIterBegin = outImg.RowBegin();
	TImage<PelGray8>::RowIter outIterEnd   = outImg.RowEnd();
	TImage<PelGray8>::RowIter inIterBegin0 = inImg0.RowBegin();
	TImage<PelGray8>::RowIter inIterBegin1 = inImg1.RowBegin();

	const size_t countPerTime= 16;
	const size_t repeatCount = (outImg.Width() + countPerTime -1) / countPerTime;

	for(; outIterBegin != outIterEnd; ++outIterBegin, ++inIterBegin0, ++inIterBegin1)
	{
		__m128i*	pSrc1280= (__m128i*)inIterBegin0.Base();
		__m128i*	pSrc1281= (__m128i*)inIterBegin1.Base();
		__m128i*	pDst128	= (__m128i*)outIterBegin.Base();
	
		for(size_t index=0; index < repeatCount; ++index)
		{
			__m128i pixel1280 = _mm_load_si128(pSrc1280);
			__m128i pixel1281 = _mm_load_si128(pSrc1281);
					
			_mm_store_si128(pDst128, _mm_and_si128(pixel1280, pixel1281));
		
			++pSrc1280;
			++pSrc1281;
			++pDst128;
		}
	}
	return true;
}

bool TImgProcess::Filter_MeanTwoLineCOM(TImage<PelRGB24> inImg,
									  TImage<PelRGB24> outImg)
{
	return false;
}
bool TImgProcess::Filter_MeanTwoLineSSE(TImage<PelRGB24> inImg,
									  TImage<PelRGB24> outImg)
{
	return false;
}

bool TImgProcess::ProjectYDirSSE(TImage<PelGray8> inImg,
								 TImage<PelGray8> &outProject)
{
	return ProjectYDirCOM(inImg, outProject);
}

bool TImgProcess::ProjectYDirCOM(TImage<PelGray8> inImg,
								 TImage<PelGray8> &outProject)
{
	bool success = false;
	if(!inImg.IsNull())
	{
		size_t width = inImg.Width();
		TRect2D<int> projectBound(TPoint2D<int>(0,0), width, 1);

		outProject.ReInit(projectBound);
		TImage<int> project(projectBound);
		project.SetZero();

		TImage<PelGray8>::RowIter iterBegin = inImg.RowBegin();
		TImage<PelGray8>::RowIter iterEnd	= inImg.RowEnd();
		TImage<  int   >::RowIter iterProj	= project.RowBegin();
		TImage<PelGray8>::RowIter iterOut	= outProject.RowBegin();
		for(; iterBegin != iterEnd; ++iterBegin)
		{
			PelGray8 *pData = iterBegin.Base();
			int		 *pProj = iterProj.Base();
			for(size_t x=0; x<width; ++x, ++pData, ++pProj)
			{
				*pProj += *pData;
			}
		}
		size_t height = inImg.Height();
		PelGray8 *pOut = iterOut.Base();
		int		 *pProj= iterProj.Base();
		for(size_t x=0; x<width; ++x, ++pOut, ++pProj)
		{
			*pOut = (*pProj)/height;
		}

		success = true;
	}
	return success;
}

bool TImgProcess::SobelFilter(const TImage<PelGray8>& srcImg, TImage<PelGray8>& dstImg)
{
	bool			success = false;
	TImage<PelGray8> image  = srcImg;
	IRect			imageWnd= image.Window();
	IRect			dstWnd	= dstImg.Window();

	imageWnd.Expand(-1, -1);
	imageWnd = imageWnd.Intersect(dstWnd);

	if(!imageWnd.IsNull())
	{
		image.Window(imageWnd);
		dstImg.Window(imageWnd);
		
		TImage<PelGray8>::RowIter iterD	= dstImg.RowBegin();
		TImage<PelGray8>::RowIter iter	= image.RowBegin();
		TImage<PelGray8>::RowIter iterPre = iter--;
		TImage<PelGray8>::RowIter iterNext= iter++;
		TImage<PelGray8>::RowIter iterEnd = image.RowEnd();
		size_t width = image.Width();
		
		for( ; iter != iterEnd; ++iter, ++iterPre, ++iterNext, ++iterD)
		{
			PelGray8 *pPre = iterPre.Base();
			PelGray8 *pNext= iterNext.Base();
			PelGray8 *pCur = iter.Base();
			PelGray8 *pDst = iterD.Base();
			for(size_t w=0; w<width; ++w, ++pPre, ++pNext, ++pCur, ++pDst)
			{
				int sumPre = *(pPre-1)  + *(pPre)*3  + *(pPre+1);
				int sumNext= *(pNext-1) + *(pNext)*3 + *(pNext+1);
				int sumV = abs(sumPre - sumNext);
				
				int sumLeft = *(pPre-1) + *(pCur-1)*3  + *(pNext-1);
				int sumRigh = *(pPre+1) + *(pCur+1)*3  + *(pNext+1);
				int sumH = abs(sumLeft - sumRigh);
				
				int sum = (sumV +sumH);
				
				*pDst = sum>255 ? 255 : sum;
			}
		}
		dstImg.Window(dstWnd);
		success = true;
	}
	return success;
}


