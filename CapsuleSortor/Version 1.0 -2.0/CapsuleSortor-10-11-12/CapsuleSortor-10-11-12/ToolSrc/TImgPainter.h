#ifndef TIMGPAINTER_H
#define TIMGPAINTER_H

#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "TString.h"
#include "TPaintCoord.h"
#include "TRect2D.h"

class TImgPainter
{
public:
	TImgPainter(HDC hdc);
	~TImgPainter();

public:
	void	Coordinate(const TPaintCoord& coord);
	const	TPaintCoord& Coordinate() const;
	void	SetOrigin	(const FPoint& point);
	void	SetZoom		(float scaleX,	float scaleY);

	void SetPenParam(COLORREF color, size_t width = 1, int style = PS_SOLID);

public:
	void MoveTo  (const FPoint& point);
	void LineTo  (const FPoint& point);
	void SetPixel(const FPoint& point, const COLORREF color);
	void SetText (const FPoint& point, const TString& text);
	void DrawRect(const FRect&  rect);
	void Arc	 (	const FPoint& center, 
					float axisH,
					float axisV,
					const FPoint& beginPos,
					const FPoint& endPos,
					bool  toLink = false);

private:
	HDC			m_hDC;
	HPEN		m_hOldPen;
	TPaintCoord m_paintCoord;	
};

#endif //TIMGPAINTER_H

